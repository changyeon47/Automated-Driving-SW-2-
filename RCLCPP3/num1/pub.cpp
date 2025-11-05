#include"rclcpp/rclcpp.hpp" //헤더파일 포함
#include"std_msgs/msg/int32.hpp" //헤더파일 포함
#include"rclcpp/time_source.hpp" //헤더파일 포함
#include<memory> //헤더파일 포함
#include<chrono> //헤더파일 포함
using namespace std::chrono_literals; //std::chrono_literals생략
void callback(rclcpp::Node::SharedPtr node,
rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr mypub)//함수 정의
{
    static int count = 0;//static 변수 선언 및 초기화
    auto message = std_msgs::msg::Int32();//객체 생성
    message.data = count++;//객체 맴버변수 값 초기화
    RCLCPP_INFO(node->get_logger(), "Publish: %d", message.data);//콘솔창에 출력
    mypub->publish(message);//발간
}
int main(int argc, char*argv[]) //메인함수
{
    rclcpp::init(argc, argv); //ros2 시스템 초기화
    auto node=std::make_shared<rclcpp::Node>("node_pub1");//노드 객체 스마트포인터로 생성 
    auto qos_profile=rclcpp::QoS(rclcpp::KeepLast(10)); //qos객체 생성
    auto mypub=node->create_publisher<std_msgs::msg::Int32>("topic_pub1", qos_profile);//pub객체 생성
    std::function<void()> fn = std::bind(callback, node, mypub);//함수호출할 콜백함수 생성
    auto timer = node->create_wall_timer(100ms, fn);//타이머 객체 
    rclcpp::spin(node);//노드 종료까지 무한 반복
    rclcpp::shutdown(); //ros2시스템 종료
    return 0; //함수 종료
}