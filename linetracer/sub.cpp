#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include "class.hpp"
#include <memory>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;
using namespace cv;
using std::placeholders::_1;
VideoWriter recorder("save.mp4",VideoWriter::fourcc('X','2','6','4'),10,Size(640, 360));

void mysub_callback(rclcpp::Node::SharedPtr node,
                    const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    static LineTracker tracker;          // 라인 트래커 상태 유지용

    auto startTime = std::chrono::steady_clock::now();

    // 1) 압축 이미지 → BGR Mat 복원
    cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
    if (frame.empty()) {
        RCLCPP_WARN(node->get_logger(), "Empty frame");
        return;
    }
    tracker.writeFrame();
    tracker.preprocess(frame);
    tracker.computeConnectedComponents();
    bool found = tracker.updateTrackingPoint();
    tracker.drawObjects(found);
    int error = tracker.computeError();

    // 4) 두 개의 창에 각각 출력
    cv::imshow("raw",  frame);                    // 원본 영상
    cv::imshow("proc", tracker.getThreshColor()); // 이진 + 사각형/점

    cv::waitKey(1);

    // 5) 시간 측정 및 로그
    auto endTime = std::chrono::steady_clock::now();
    float totalTime_ms = std::chrono::duration<float, std::milli>(endTime - startTime).count();

    RCLCPP_INFO(node->get_logger(), "err:%d, time:%.5f", error, totalTime_ms);
}



int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = make_shared<rclcpp::Node>("camsub_wsl9");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //TCP
    function<void(const sensor_msgs::msg::CompressedImage::SharedPtr msg)> fn;
    fn = bind(mysub_callback, node, _1);
    auto mysub = node->create_subscription<sensor_msgs::msg::CompressedImage>("image/compressed_9",qos_profile,fn);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
