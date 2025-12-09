#include "class.hpp"

using namespace cv;

// --- 전처리 ---
void LineTracker::preprocess(const Mat& frame)
{
    // BGR -> Gray
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    // 밝기 평균 맞추기
    Scalar bright_avg = mean(gray);
    gray = gray + (target_brightness - bright_avg[0]);

    // 이진화
    threshold(gray, thresh, bin_threshold, 255, THRESH_BINARY);

    // 하단 1/4만 ROI로 사용
    int r_pts = thresh.rows / 4 * 3;
    Rect roi(0, r_pts, thresh.cols, thresh.rows - r_pts);
    thresh = thresh(roi).clone();   // ROI 복사본으로 저장

    // 첫 프레임이면 기준점 중앙으로 초기화
    if (!first_pt) {
        pt = Point(thresh.cols / 2, thresh.rows / 2);
        first_pt = true;
    }
}

// 라벨링 함수
void LineTracker::computeConnectedComponents()
{
    components = connectedComponentsWithStats(
        thresh, labels, stats, centroids);
}

//  기준점(pt)에 가장 가까운 객체 찾는 함수
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

// 박스/점 그리는 함수
void LineTracker::drawObjects(bool found_target)
{
    // 1채널 ROI에서 3채널 BGR로 변환
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
                // 추적중인 선은 빨간색
                rectangle(color, box, Scalar(0, 0, 255), 2);
                circle(color, Point(x, y), 5, Scalar(0, 0, 255), -1);
            } else {
                // 나머지 후보인 선은 파란색
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

// 에러 계산 함수
int LineTracker::computeError() const
{
    if (color.empty()) {
        return 0;
    }
    return (color.cols / 2) - pt.x;
}

// 처리된 ROI 반환
const Mat& LineTracker::getThreshColor() const
{
    return color;
}

// 비디오 저장 함수
void LineTracker::writeFrame(const Mat& frame)
{
    if (!video_writer_.isOpened()) {
        int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G');
        double fps = 30.0;
        Size size = frame.size();
        std::string filename = "output.avi";

        video_writer_.open(filename, fourcc, fps, size, true);
        if (!video_writer_.isOpened()) {
            std::cerr << "⚠️ VideoWriter open 실패: " << filename << std::endl;
            return;
        }
    }
    video_writer_.write(frame);
}
