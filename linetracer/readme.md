#### linetracer inline
https://youtu.be/807pPFbpGvk
#### linetracer outline
https://youtu.be/DWen5teSCWc

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

