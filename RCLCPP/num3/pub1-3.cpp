#include "rclcpp/rclcpp.hpp"//헤더파일 포함
#include "geometry_msgs/msg/twist.hpp"//헤더파일 포함
#include <memory> //헤더파일 포함
#include <chrono> //헤더파일 포함
#include <iostream> //헤더파일 포함
using namespace std;//std생략
int main(int argc, char*argv[]) {//메인함수
    char a ;//변수 선언
    rclcpp::init(argc, argv);//ROS2 통신 초기화
    auto node = std::make_shared<rclcpp::Node>("node_pub1");//노드 객체 생성  
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //객체 생성 및 초기화
    auto mypub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", qos_profile);//topic_pub1토픽으로 퍼블리시하는 퍼블리셔 생성
    geometry_msgs::msg::Twist message; //메시지 객체 생성
    while(rclcpp::ok()) { //무한루프
        message.linear.x = 0;//맴버변수 값 변경
        message.angular.z = 0;//맴버변수 값 변경
        cin >> a;//변수에 값을 대입
        if(a == 'f'){//조건문
            message.linear.x = 10;//맴버변수 값 변경
        }
        else if(a == 'b'){//조건문
            message.linear.x = -10;//맴버변수 값 변경
        }
        else if(a == 'r'){//조건문
            message.angular.z = -10;//맴버변수 값 변경
        }
        else if(a == 'l'){//조건문
            message.angular.z = 10;//맴버 변수값 변경
        }
        RCLCPP_INFO(node->get_logger(), "Publish: %c", a);//퍼블리시할 데이터를 콘솔에 입력 
        mypub->publish(message); //메시지를 토픽에 퍼블리시
    } 
    rclcpp::shutdown(); //ROS2 통신 종료 
    return 0; //프로그램 종료
}
