#### linetracer inline
https://youtu.be/807pPFbpGvk
#### linetracer outline
https://youtu.be/DWen5teSCWc

'''C++
void LineTracker::preprocess(const cv::Mat& frame)
{
    // 1) BGR -> Gray
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    // 2) 밝기 평균 맞추기 (슬라이드 10의 식)
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
'''
