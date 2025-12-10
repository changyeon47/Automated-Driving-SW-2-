#include "class.hpp"
#include <iostream>
using namespace cv;


LineTracker::LineTracker(){
    std::cout << "생성자" << std::endl;
} 

// 전처리 함수: 이미지를 흑백 변환, 밝기 보정, 이진화, ROI 설정 수행
void LineTracker::preprocess(Mat& frame)
{
    cvtColor(frame, gray, COLOR_BGR2GRAY);// 입력된 컬러(BGR) 이미지를 그레이스케일 이미지로 변환
    Scalar bright_avg = mean(gray); // 변환된 그레이스케일 이미지의 평균 밝기 계산
    gray = gray + (target_brightness - bright_avg[0]);// 현재 이미지 밝기를 목표 밝기에 맞추어 보정 (조명
    threshold(gray, thresh, bin_threshold, 255, THRESH_BINARY);// 그레이스케일 이미지를 흑백(Binary) 이미지로 변환 (임계값 기준)
    int r_pts = thresh.rows / 4 * 3;// 이미지의 하단 1/4 지점의 Y 좌표 계산 
    Rect roi(0, r_pts, thresh.cols, thresh.rows - r_pts);// 하단 1/4 영역을 관심 영역(ROI)으로 설정 
    thresh = thresh(roi).clone();// 설정한 ROI 영역만 잘라내어 복사본으로 저장 

    
    if (!first_pt) {// 추적 기준점(pt)이 아직 초기화되지 않았다면 (첫 프레임인 경우)
        pt = Point(thresh.cols / 2, thresh.rows / 2);// ROI 이미지의 정중앙 좌표를 초기 기준점으로 설정
        first_pt = true;// 초기화 완료 플래그 설정
    }
}


void LineTracker::computeConnectedComponents()// 라벨링 함수: 이진화된 이미지에서 객체를 찾아 번호를 매김
{
    components = connectedComponentsWithStats(thresh, labels, stats, centroids);// 연결된 성분을 분석하여 레이블, 통계 정보, 무게중심 좌표를 계산
}

bool LineTracker::updateTrackingPoint()// 추적 점 업데이트 함수: 이전 기준점(pt)과 가장 가까운 객체를 찾아 새로운 기준점으로 갱신
{
    int min_index = -1;// 가장 가까운 객체의 인덱스 초기화 (-1은 찾지 못함을 의미)
    int min_dist  = thresh.cols;// 최소 거리 초기값 설정 (이미지 가로 폭으로 설정하여 충분히 큰 값 부여)

    
    for (int i = 1; i < components; ++i) {// 검출된 객체 수만큼 반복 
        int area = stats.at<int>(i, CC_STAT_AREA);// i번째 객체의 면적 가져오기
        
        if (area > min_area) {// 면적이 설정한 최소 크기보다 큰 경우에만 유효한 라인으로 간주
            int x = cvRound(centroids.at<double>(i, 0));// i번째 객체의 무게중심 X 좌표
            int y = cvRound(centroids.at<double>(i, 1));// i번째 객체의 무게중심 Y 좌표
            int dist = norm(Point(x, y) - pt);// 현재 객체 위치와 이전 프레임의 기준점 사이의 거리 계산
            
            if (dist < min_dist && dist <= max_track_dist) {// 거리가 현재 최소 거리보다 작고, 추적 허용 범위 내에 있다면
                min_dist  = dist;// 최소 거리 갱신
                min_index = i;// 가장 유력한 객체의 인덱스 갱신
            }
        }
    }

    if (min_index != -1 && min_dist <= max_track_dist) { // 유효한 객체를 찾았고, 거리가 허용 범위 내라면
        pt.x = cvRound(centroids.at<double>(min_index, 0));// 기준점(pt)의 X 좌표를 찾은 객체의 무게중심으로 업데이트
        pt.y = cvRound(centroids.at<double>(min_index, 1));// 기준점(pt)의 Y 좌표를 찾은 객체의 무게중심으로 업데이트
        return true;// 추적 성공 반환
    }
    
    return false;// 추적 실패 반환
}

// 시각화 함수: 객체 박스와 추적 점을 화면에 그림
void LineTracker::drawObjects(bool found_target)
{
    cvtColor(thresh, color, COLOR_GRAY2BGR);// 결과를 컬러로 보기 위해 1채널 ROI를 3채널(BGR)로 변환
    for (int i = 1; i < components; ++i) {// 검출된 모든 객체에 대해 반복 
        int area = stats.at<int>(i, CC_STAT_AREA);// 객체의 면적 가져오기
        if (area > min_area) {// 노이즈가 아닌 유효한 크기의 객체만 시각화
            int x = cvRound(centroids.at<double>(i, 0));// 객체의 무게중심 X 좌표
            int y = cvRound(centroids.at<double>(i, 1)); // 객체의 무게중심 Y 좌표

            
            Rect box(stats.at<int>(i, CC_STAT_LEFT), stats.at<int>(i, CC_STAT_TOP), // 객체를 감싸는 바운딩 박스 정보 생성
            stats.at<int>(i, CC_STAT_WIDTH),  stats.at<int>(i, CC_STAT_HEIGHT)  ); // 좌측 상단 X, 좌측 상단 Y, 폭, 높이

            
            if (x == pt.x && found_target) {// 현재 그리는 객체가 추적 중인 타겟(pt)과 위치가 같고, 타겟을 찾은 상태라면
                rectangle(color, box, Scalar(0, 0, 255), 2);// 추적 중인 선이므로 빨간색 박스 그리기
                circle(color, Point(x, y), 5, Scalar(0, 0, 255), -1);// 무게중심에 빨간색 점 찍기
            } 
            else {
                rectangle(color, box, Scalar(255, 0, 0), 2);// 추적 대상이 아닌 다른 후보 객체들은 파란색으로 표시
                circle(color, Point(x, y), 5, Scalar(255, 0, 0), -1);// 무게중심에 파란색 점 찍기
            }
        }
    }
    
    if (!found_target) {// 만약 타겟을 찾지 못했을 때
        circle(color, pt, 5, Scalar(0, 0, 255), -1);// 마지막으로 알려진 기준점 위치에 빨간 점만 표시 (위치 유지)
    }
}


int LineTracker::computeError()// 에러 계산 함수: 영상 중심과 추적 점 사이의 가로 편차 계산
{
    if (color.empty()) {// 처리된 결과 이미지가 비어있으면 에러 0 반환 
        return 0;
    }
    return (color.cols / 2) - pt.x;// 양수면 점이 왼쪽에 있음, 음수면 오른쪽에 있음
}


Mat& LineTracker::getThreshColor()// 결과 이미지 반환 함수: 처리 및 시각화가 완료된 ROI 이미지 반환
{
    return color;// 컬러로 변환되고 박스가 그려진 이미지 반환
}


void LineTracker::writeFrame()// 영상 저장 함수: 처리된 프레임을 동영상 파일로 저장
{
    if (color.empty()) {// 저장할 프레임(color)이 비어있으면 에러 메시지 출력 후 종료
        std::cerr << " 처리된 프레임(color)이 비어 있어서 저장할 수 없습니다.\n";
        return;
    }
    
    if (!video_writer_.isOpened()) {// 비디오 작성기(VideoWriter)가 열려있지 않다면 (최초 1회 실행)
        
        int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');// 코덱 설정 (MJPG)
        
        double fps = 30.0;// FPS 설정 (30 프레임)
        
        cv::Size size = color.size();// 저장할 비디오의 해상도 설정 (현재 프레임 크기와 동일하게)
        
        std::string filename = "output_proc.avi";  // 저장할 파일명 지정 

        
        video_writer_.open(filename, fourcc, fps, size, true);// 설정한 값으로 VideoWriter 파일 열기
        
        
        if (!video_writer_.isOpened()) {// 파일 열기에 실패했을 경우 에러 메시지 출력 후 종료
            std::cerr << " VideoWriter open 실패: " << filename << std::endl;
            return;
        }
    }
    video_writer_.write(color);// 전처리 및 시각화(박스, 점)가 완료된 프레임을 파일에 쓰기
}
