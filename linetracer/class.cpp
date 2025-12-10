#include "class.hpp"

using namespace cv;

// --- (A) 전처리 ---
void LineTracker::preprocess(const Mat& frame)
{
    // 1) BGR -> Gray
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    // 2) 밝기 평균 맞추기
    Scalar bright_avg = mean(gray);
    gray = gray + (target_brightness - bright_avg[0]);

    // 3) 이진화
    threshold(gray, thresh, bin_threshold, 255, THRESH_BINARY);

    // 4) 하단 1/4만 ROI로 사용
    int r_pts = thresh.rows / 4 * 3;
    Rect roi(0, r_pts, thresh.cols, thresh.rows - r_pts);
    thresh = thresh(roi).clone();   // ROI 복사본으로 저장

    // 5) 첫 프레임이면 기준점 중앙으로 초기화
    if (!first_pt) {
        pt = Point(thresh.cols / 2, thresh.rows / 2);
        first_pt = true;
    }
}

// --- (B) 라벨링 ---
void LineTracker::computeConnectedComponents()
{
    components = connectedComponentsWithStats(
        thresh, labels, stats, centroids);
}

// --- (C) 기준점(pt)에 가장 가까운 객체 찾기 ---
bool LineTracker::updateTrackingPoint()
{
    int min_index = -1;
    int min_dist  = thresh.cols;

    for (int i = 1; i < components; ++i) {  // 0은 배경
        int area = stats.at<int>(i, CC_STAT_AREA);
        if (area > min_area) {
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));

            int dist = norm(Point(x, y) - pt);
            if (dist < min_dist && dist <= max_track_dist) {
                min_dist  = dist;
                min_index = i;
            }
        }
    }

    if (min_index != -1 && min_dist <= max_track_dist) {
        pt.x = cvRound(centroids.at<double>(min_index, 0));
        pt.y = cvRound(centroids.at<double>(min_index, 1));
        return true;
    }
    return false;
}

// --- (D) 박스/점 그리기 ---
void LineTracker::drawObjects(bool found_target)
{
    // 1채널 ROI -> 3채널 BGR로 변환
    cvtColor(thresh, color, COLOR_GRAY2BGR);

    for (int i = 1; i < components; ++i) {
        int area = stats.at<int>(i, CC_STAT_AREA);
        if (area > min_area) {
            int x = cvRound(centroids.at<double>(i, 0));
            int y = cvRound(centroids.at<double>(i, 1));

            Rect box(
                stats.at<int>(i, CC_STAT_LEFT),
                stats.at<int>(i, CC_STAT_TOP),
                stats.at<int>(i, CC_STAT_WIDTH),
                stats.at<int>(i, CC_STAT_HEIGHT)
            );

            if (x == pt.x && found_target) {
                // 추적중인 선 → 빨간색
                rectangle(color, box, Scalar(0, 0, 255), 2);
                circle(color, Point(x, y), 5, Scalar(0, 0, 255), -1);
            } else {
                // 나머지 후보 → 파란색
                rectangle(color, box, Scalar(255, 0, 0), 2);
                circle(color, Point(x, y), 5, Scalar(255, 0, 0), -1);
            }
        }
    }

    // 근처에 객체 없으면 기존 pt 위치만 표시
    if (!found_target) {
        circle(color, pt, 5, Scalar(0, 0, 255), -1);
    }
}

// --- (E) 에러 계산 ---
int LineTracker::computeError() const
{
    if (color.empty()) {
        return 0;
    }
    return (color.cols / 2) - pt.x;
}

// --- (F) 처리된 ROI 반환 ---
const Mat& LineTracker::getThreshColor() const
{
    return color;
}

// --- (G) (옵션) 비디오 저장용 ---
void LineTracker::writeFrame()
{
    // 아직 drawObjects가 안 돌아간 상태면 color가 비어 있을 수 있음
    if (color.empty()) {
        std::cerr << " 처리된 프레임(color)이 비어 있어서 저장할 수 없습니다.\n";
        return;
    }

    if (!video_writer_.isOpened()) {
        int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
        double fps = 30.0;
        cv::Size size = color.size();
        std::string filename = "output_proc.avi";   // 처리 영상이니까 이름도 바꿔두자

        video_writer_.open(filename, fourcc, fps, size, true);
        if (!video_writer_.isOpened()) {
            std::cerr << " VideoWriter open 실패: " << filename << std::endl;
            return;
        }
    }

    // 전처리 + bbox + 점까지 그려진 Mat 저장
    video_writer_.write(color);
}
