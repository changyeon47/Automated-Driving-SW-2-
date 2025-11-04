#include "rclcpp/rclcpp.hpp"//헤더파일 포함
#include "geometry_msgs/msg/vector3.hpp"//헤더파일 포함
#include <memory>//헤더파일 포함
#include <chrono>//헤더파일 포함
#include <functional>//헤더파일 포함
//#include <string>
using namespace std;
using namespace std::chrono_literals;//std::chrono_literals 생략
void callback(rclcpp::Node::SharedPtr node,
rclcpp::Publisher<geometry_msgs::msg::Vector3>::SharedPtr mypub)//함수 정의
{
    auto message = geometry_msgs::msg::Vector3();//객체 생성
    cin >> message.x;//입력받은 값을 맴버변수에 대입
    cin >> message.y;//입력받은 값을 맴버변수에 대입
    cin >> message.z;//입력받은 값을 맴버변수에 대입
    RCLCPP_INFO(node->get_logger(), "Publish: x : %f , y: %f, z : %f", message.x,message.y,message.z);//콘솔창에 출력
    mypub->publish(message);//발간
}
int main(int argc, char* argv[])//메인함수
{
    rclcpp::init(argc, argv);//ros시스템 초기화
    auto node = std::make_shared<rclcpp::Node>("mynode");//노드객체를 스마트포인터로 생성
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));//qos객체 생성
    auto pub = node->create_publisher<geometry_msgs::msg::Vector3>("mytopic", qos_profile);//pub객체 생성
    std::function<void()> fn = std::bind(callback, node, pub);//함수호출할 콜백함수 생성
    auto timer = node->create_wall_timer(1s, fn);//타이머 객체 생성
    rclcpp::spin(node);//노드 종료 신호를 받을때까지 구독
    rclcpp::shutdown();//ros2시스템 종료
    return 0;//함수종료
}