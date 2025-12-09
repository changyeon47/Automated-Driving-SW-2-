#include <opencv2/opencv.hpp>
class LineTracker
{

private:
    bool  first_pt = false;
    cv::Point pt;

    cv::Mat gray;       // 그레이스케일
    cv::Mat thresh;     // ROI 이진화 결과 (1채널)
    cv::Mat labels;
    cv::Mat stats;
    cv::Mat centroids;
    cv::Mat color;      // 색 입힌 ROI (3채널)

    int components = 0;

    // 파라미터들
    int target_brightness = 100;   // 목표 밝기
    int bin_threshold     = 130;   // 이진화 임계값
    int min_area          = 100;   // 최소 영역
    int max_track_dist    = 150;   // 최대 추적 거리

    cv::VideoWriter video_writer_; // 영상 저장용 (원하면 사용)

public:
    LineTracker() = default;

    //전처리: BGR frame -> gray + 밝기 보정 + TH + 하단 1/4 ROI
    void preprocess(const cv::Mat& frame);

    // --- 라벨링 ---
    void computeConnectedComponents();

    //기준점(pt)에 가장 가까운 객체 찾기
    bool updateTrackingPoint();

    // 박스/점 그리기 (color Mat 안에 그려짐)
    void drawObjects(bool found_target);

    // 에러 계산: 중앙 - pt.x
    int computeError() const;

    // 처리된 ROI (색 입힌 이진 영상) 얻기
    const cv::Mat& getThreshColor() const;

    //(옵션) 원본 프레임을 비디오로 저장하고 싶을 때
    void writeFrame();

};
