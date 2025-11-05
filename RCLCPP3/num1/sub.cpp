#include"rclcpp/rclcpp.hpp" //헤더파일 포함
#include"std_msgs/msg/int32.hpp" //헤더파일 포함
#include<memory> //헤더파일 포함
#include<functional> //헤더파일 포함
void mysub_callback(rclcpp::Node::SharedPtr node, const std_msgs::msg::Int32::SharedPtr msg) //함수 정의
{ 
    RCLCPP_INFO(node->get_logger(), "Received message: %d", msg->data); //콘솔창에 출력
}
int main(int argc, char*argv[]) //메인함수
{ 
    rclcpp::init(argc, argv); //ros2 시스템 초기화
    auto node = std::make_shared<rclcpp::Node>("node_sub1");//node 객체 스마트 포인터 생성 
    auto qos_profile= rclcpp::QoS(rclcpp::KeepLast(10)); //qos객체 생성
    std::function<void(const std_msgs::msg::Int32::SharedPtr)> fn= std::bind(mysub_callback, node, std::placeholders::_1);// 호출할 콜백함수 생성
    auto mysub= node->create_subscription<std_msgs::msg::Int32>("topic_pub1",qos_profile,fn); //sub객체 생성
    rclcpp::spin(node); //노드 종료 명령까지 무한 반복
    rclcpp::shutdown(); //ros2시스템 종료
    return 0; //함수 종료
}