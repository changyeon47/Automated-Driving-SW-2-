#include "rclcpp/rclcpp.hpp"//헤더파일 포함
#include "geometry_msgs/msg/vector3.hpp"//헤더파일 포함
#include <memory> //헤더파일 포함
#include <chrono> //헤더파일 포함
using namespace std;//std생략
int main(int argc, char*argv[]) {//메인함수
    float a = 0;//변수 선언
    rclcpp::init(argc, argv);//ROS2 통신 초기화
    auto node = std::make_shared<rclcpp::Node>("node_pub1");//노드 객체 생성  
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //객체 생성 및 초기화
    auto mypub = node->create_publisher<geometry_msgs::msg::Vector3>("topic_pub1", qos_profile);//topic_pub1토픽으로 퍼블리시하는 퍼블리셔 생성
    geometry_msgs::msg::Vector3 message; //메시지 객체 생성
    rclcpp::WallRate loop_rate(1.0);//주기 설정
    while(rclcpp::ok()) { //무한루프
        RCLCPP_INFO(node->get_logger(), "x");//퍼블리시할 데이터를 콘솔에 입력 
        cin >> a;//변수 a에 값을 대입
        message.x = a;//맴버 변수 x에 값을 대입
        RCLCPP_INFO(node->get_logger(), "y");//퍼블리시할 데이터를 콘솔에 입력 
        cin >> a;//변수 a에 값을 대입
        message.y = a;//맴버 변수 y에 값을 대입
        cin >> a;//변수 a에 값을 대입
        message.z = a;//맴버 변수 z에 값을 대입
        RCLCPP_INFO(node->get_logger(), "Publish: \n x : %f \n y : %f \n z: %f \n", message.x,message.y,message.z);//퍼블리시할 데이터를 콘솔에 입력 
        mypub->publish(message); //메시지를 토픽에 퍼블리시
        loop_rate.sleep(); //설정한 시간이 될때까지 대기
    } 
    rclcpp::shutdown(); //ROS2 통신 종료 
    return 0; //프로그램 종료
}