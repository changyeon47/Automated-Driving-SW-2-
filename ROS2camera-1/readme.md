### Jetson nano, wsl2-ubuntu, windows의 ip주소를 출력하라.

## jetson nano
<img width="695" height="177" alt="image" src="https://github.com/user-attachments/assets/abfdd89b-0b0f-46d9-9e96-eaa25402f460" />

## WSL2
<img width="743" height="169" alt="image" src="https://github.com/user-attachments/assets/2f378e3d-67fa-4e36-a717-5ec0843b7cb9" />

## Windows
<img width="566" height="150" alt="image" src="https://github.com/user-attachments/assets/cfbebf16-f90a-432e-9726-cb5cbf412a0f" />

### 같은 네트워크에 있는지 설명하라.
ip주소 앞자리 192.168.0.* 이런식으로 앞 3자리가 같으면 같은 네트워크에 연결되어있다
또한 ROS2 Ros2는 13326-5GHz에 연결되어있고
windows에도 밑에 사진을 보면 13326-5GHz에 연결되어있다

<img width="845" height="67" alt="image" src="https://github.com/user-attachments/assets/cf9cbc57-b520-4855-971b-6c3f6760baad" />

### Jetson nano 에서 환경변수 ROS_DOMAIN_ID값을 확인하라.
<img width="362" height="66" alt="image" src="https://github.com/user-attachments/assets/ada24708-5712-441d-8e5d-dabeede61193" />

### wsl2-ubuntu 에서 환경변수 ROS_DOMAIN_ID값을 확인하라.
<img width="490" height="88" alt="image" src="https://github.com/user-attachments/assets/6b37dd5b-e193-45a2-bc7f-6328f8258a95" />

### 2개가 로봇번호와 같은지 확인하라.
wsl2-ubuntu 에서 환경변수 ROS_DOMAIN_ID값과 Jetson nano 에서 환경변수 ROS_DOMAIN_ID값을 확인해보면 9로 같은걸 볼수있다.
