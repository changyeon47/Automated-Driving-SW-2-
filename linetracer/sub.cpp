#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>
#include <iostream>
using std::placeholders::_1;
cv::VideoWriter recorder("save.mp4",cv::VideoWriter::fourcc('X','2','6','4'),10,cv::Size(640, 360));
void mysub_callback(rclcpp::Node::SharedPtr node, const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
    cv::Mat frame2;
    cv::Mat gray,bin;
    recorder.write(frame);
    frame2 = frame(cv::Rect(0,270,640,90));
    cv::cvtColor(frame2, gray, cv::COLOR_RGB2GRAY);
    double total_brightness = 0;
    for (int i = 0; i < gray.rows; ++i) {
        for (int j = 0; j < gray.cols; ++j) {
            total_brightness += gray.at<uchar>(i, j);
        }
    }
    gray = gray + (100 - total_brightness/(gray.rows*gray.cols));
    cv::threshold(gray, bin, 100,255,cv::THRESH_BINARY);
    cv::imshow("wsl",frame);
    cv::imshow("wsl2",bin);
    cv::waitKey(1);
    RCLCPP_INFO(node->get_logger(), "Received Image : %s,%d,%d", msg->format.c_str(),frame.rows,frame.cols);
}
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("camsub_wsl9");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //TCP
    std::function<void(const sensor_msgs::msg::CompressedImage::SharedPtr msg)> fn;
    fn = std::bind(mysub_callback, node, _1);
    auto mysub = node->create_subscription<sensor_msgs::msg::CompressedImage>("image/compressed_9",qos_profile,fn);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
