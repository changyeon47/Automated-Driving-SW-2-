## linetracer inline
https://youtu.be/807pPFbpGvk
## linetracer outline
https://youtu.be/DWen5teSCWc

# 1. 전처리 알고리즘

    void LineTracker::preprocess(const cv::Mat& frame){

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        Scalar bright_avg = mean(gray);
        gray = gray + (target_brightness - bright_avg[0]);
        threshold(gray, thresh, bin_threshold, 255, THRESH_BINARY);
        int r_pts = thresh.rows / 4 * 3;
        Rect roi(0, r_pts, thresh.cols, thresh.rows - r_pts);
        thresh = thresh(roi).clone();   
        if (!first_pt) {
            pt = Point(thresh.cols / 2, thresh.rows / 2);
            first_pt = true;
            }
    }

입력 영상의 하단 1/4 정도를 ROI로 사용
[!TIP]cv::cvtColor로 그레이스케일 변환
cv::mean으로 평균 밝기 구해서 원하는 평균 밝기에 맞게 보정
cv::threshold 함수를 이용하여 이진화

# 2. 라인 검출(레이블링, 이전 위치 기반 추적)

    void LineTracker::computeConnectedComponents()
    {
        components = connectedComponentsWithStats(
            thresh, labels, stats, centroids);
    }

cv:: connectedComponentsWithStats 함수를 이용하여 라인 후보 영역을 찾는다.
찾는 방법은 x,y좌표 폭/높이, 면적, 무게중심을 저장하여 후보를 지정
# 3. 라인 기준 선택


    int area = stats.at<int>(i, CC_STAT_AREA);
    if (area > min_area) {
    
for(int i = 1; i< components; ++i) 0번 라벨은 항상 배경이아 제외
if(area > min_area)보다 작으면 노이즈, 너무 얇은 선등으로 판단하여 라인 후보에서 제외

    int dist = norm(Point(x, y) - pt);
    if (dist < min_dist && dist <= max_track_dist) {
        min_dist  = dist;
        min_index = i;
    }

pt는 이전 프레임에서 선택된 라인의 위치
 이번 프레임에서 나온 후보 중 pt와의 거리가 가장 가깝고(dist < min_dist)
 한프레임에 움직일 수 있는 최대거리(dist <= max_track_dist) 이하인 것만

 조건을 만족하는 라벨이 하나도 없으면 pt를 업데이트하지 않음

 # 4. 디버깅 시각화 
 
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
                    // 선택된 라인 후보 → 빨간 박스 + 빨간 점
                    rectangle(color, box, Scalar(0, 0, 255), 2);
                    circle(color, Point(x, y), 5, Scalar(0, 0, 255), -1);
                } else {
                    // 나머지 후보 → 파란 박스 + 파란 점
                    rectangle(color, box, Scalar(255, 0, 0), 2);
                    circle(color, Point(x, y), 5, Scalar(255, 0, 0), -1);
                }
            }
        }
    
        // 아무 후보도 못 찾았으면, 이전 위치에 점만 유지
        if (!found_target) {
            circle(color, pt, 5, Scalar(0, 0, 255), -1);
        }
    }
모든 라인 후보는 파란색 바운딩 박스
선택된 라인은 빨간 바운딩 박스와 무게 중심에 빨간 점

# 6. 위치오차 계산

    int LineTracker::computeError() const
    {
        if (color.empty()) {
            return 0;
        }
        return (color.cols / 2) - pt.x;
    }
error = 영상중심 x좌표 - 라인의 무게중심 x좌표
RCLCPP_INFO(node->get_logger(), "err:%d, time:%.5f", error, totalTime_s); 이 코드를 통해 터미널에 문구 출력

# 7. 처리 결과 영상 저장

    void LineTracker::writeFrame()
    {
        if (color.empty()) {
            std::cerr << "⚠️ 처리된 프레임(color)이 비어 있어서 저장할 수 없습니다.\n";
            return;
        }
    
        if (!video_writer_.isOpened()) {
            int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
            double fps = 30.0;
            cv::Size size = color.size();
            std::string filename = "output_proc.avi";
    
            video_writer_.open(filename, fourcc, fps, size, true);
            if (!video_writer_.isOpened()) {
                std::cerr << "⚠️ VideoWriter open 실패: " << filename << std::endl;
                return;
            }
        }
    
        video_writer_.write(color);   // bbox/점까지 그려진 처리 영상 저장
    }

위 코드를 통해 원본영상에 전처리 + 라인 검출영상을 저장
