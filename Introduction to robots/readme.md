#### 실습과제2

### 1) Nvidia사의 Jetson nano 보드에 대하여 조사하라
NVIDIA가 개발한 엣지 AI 컴퓨팅 보드로, 소형이면서도 강력한 GPU 연산 성능을 갖춘 개발 플랫폼입니다. 인공지능, 딥러닝, 컴퓨터비전, 로보틱스
등의 실험 및 프로토타이핑에 널리 사용됩니다.

### 2) IMX219 카메라의 사양을 조사하라
CPU : Quad-core ARM cortex-A57 @ 1.43GHz
GPU : 128-core NVIDIA Maxwell architecture GPU
RAM : 4GB(LPDDR4)
저장장치 : microSD 카드 슬롯(최대 128GB 이상 지원)
입출력포트 : USB 3.0 x 4, HDMI, DisplayPort, Gigabit Ethernet, GPIO, I2C, I2S, SPI, UART
전원 : Micro-USB(5V 2A)또는 DC 잭(5V 4A)
크기 : 100mm * 80mm
운영체제 : Ubuntu 기반의 JetPack SDK

### 3) 카메라의 리눅스장치 파일 이름은?
/dev/video0

### 4) CSI(camera serial interface)에 대하여 조사하라
CSI는 카메라 모듈과 프로세서를 고속 직렬 통신 방식으로 연결하기 위한 표쥰 인터페이스입니다. 

### 5) Gstreamer에 대하여 자세히 조사하라
오픈 소스 멀티미디어 프레임 워크로, 비디오,오디오,이미지,네트워크 스트리밍 등 다양한 미더어 처리 파이프라인을 구성할 수 있는
플러그인 기반 시스템입니다. 주요 기능으로는 멀티미디어 스트리밍, 코덱 지원, 플러그인 구조, 하드웨어 가속, 파이프라인 기반 처리
등이 있다. 동작원리는 Source -> Filter -> Muxer -> Sink 순으로 파이프라인 기반으로 동작합니다.

구성요소
Source : 입력장치
Filter : 데이터 처리
Sink : 출력 장치
Bus : 파이프라인 내 메시지 전달
Caps : 각 요소 간 데이터 형식 협상

### 6) Dynamixel에 대하여 자세히 조사하라
로보티즈 사에서 개발한 스마트 서보 모터 시리즈입니다. 단순히 회전만 하는 일반 서보와 달리, 위치, 속도, 토크 제어 및 상태 피드백이 
가능한 지능형 액추에이터입니다. 주요 특징으로는 정밀제어, 양방향 통신, 통신 방식, ID 기반 네트워킹, 내장 센서, 펌웨어 기반 
제어 테이블이 있으며, Daisy Chain 형태로 여러 모터를 하나의 통신 라인에 직렬로 연결합니다.

통신 프로토콜 
Protocol : 1.0 / 2.0 (로봇 종류에 따라 다름)
Half-Duplex UART(RS-485 or TTL) 방식 사용
명령 패킷 과 응답 패킷으로 구성

### 7) U2D2 장치의 역할은무엇인가?
U2D2 장치는 Jetson Nano와 Dynamixel 모터 사이에서 통신 신호를 변환해주는 인터페이스 장치이다.
Jetson Nano에서는 USB 포트를 통해 외부 장치와 통신할 수 있다.
Dynamixel에서는 UART 통신으로 외부 제어 신호를 주고 받는다.
### 8) U2D2 리눅스 장치파일의이름은
/dev/ttyUSB0
