#include "rclcpp/rclcpp.hpp"//헤더파일 포함
#include "geometry_msgs/msg/twist.hpp"//헤더파일 포함
#include <memory>//헤더파일 포함
#include <chrono>//헤더파일 포함
#include <functional>//헤더파일 포함
using namespace std::chrono_literals;//std::chrono_literals 생략
void callback(rclcpp::Node::SharedPtr node, rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr mypub)//함수 정의
{
    auto message = geometry_msgs::msg::Twist();//객체 생성
    message.linear.x = 2;
    message.angular.z = -2;
    RCLCPP_INFO(node->get_logger(), "Publish:");//콘솔창에 출력
    mypub->publish(message);//발간
}
int main(int argc, char* argv[])//메인함수
{
    rclcpp::init(argc, argv);//ros시스템 초기화
    auto node = std::make_shared<rclcpp::Node>("mynode");//노드객체를 스마트포인터로 생성
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));//qos객체 생성
    auto pub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", qos_profile);//pub객체 생성
    std::function<void()> fn = std::bind(callback, node, pub);//함수호출할 콜백함수 생성
    auto timer = node->create_wall_timer(100ms, fn);//타이머 객체 생성
    rclcpp::spin(node);//노드 종료 신호를 받을때까지 구독
    rclcpp::shutdown();//ros2시스템 종료
    return 0;//함수종료
}