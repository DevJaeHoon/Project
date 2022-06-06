# 홈 지키미 RC카
Python용 Qt인 Pyside를 통하여 UI를 제작하여 UI를 통해 RC카를 제어하고 RC카에 장착된 센서가 데이터를 측정하여 DB 서버에 업로드하면, 해당 값들을 통해 UI에서 메세지를 띄워거나, 차트로 시각화하여 웹페이지를 따로 제작하였습니다. OLED를 원하는 메세지를 전달할 수 있으며, 장애물과 RC카가 가까워지면 긴급제동 및 부저를 울립니다. 

# 환경
* AWS EC2
* MySQL
* PySide6 & PySide2
* Vue.js & vue-apexcharts
* Express
* Socket.io

`Front-End` 에서는 Local의 Vue.js, socket.io, Chart Library 를 사용하며,
`Back-End` 에서는 Express 서버, socket.io, mysql2 를 주력으로 사용합니다.

1. cors : CORS 브라우저 보안 설정을 쉽게 할 수 패키지, 이 패키지를 사용하면, 보안 설정을 쉽게 할 수 있다.
2. express : Node.js 서버 개발을 위한 프레임워크
3. mysql2 : Promise를 지원하는 MySQL Client 패키지
4. socket.io : Front와 Backend 서버 간 데이터 전달을 위한 패키지
5. morgan : log 이쁘게 찍는 것

# 사용한 부품
* Raspberry Pi 4
* motor driver
* 전원공급장치
* dht11, 초음파센서
* OLED, buzzer
* 18650 battery x 4개

# 설명
![image](https://user-images.githubusercontent.com/99601412/172088049-f9c346f5-e4a3-4df1-9eb1-2a59721f60f4.png)

## RC카 제어
UI를 통해 RC카를 조작할 수 있습니다. `DC모터`를 통해 동작하며, 후륜구동합니다. `서보모터`를 통해 앞바퀴를 좌우로 제어합니다. RC카와 장애물 과의 거리가 가까워지면, 긴급제동과 함께 부저를 울립니다.

|RC카 수동 제어|장애물 접근 시 긴급 제동|
|-|-|
|<img src="https://user-images.githubusercontent.com/99601412/172049682-d1341211-7ff8-4d26-8bf2-152f94088ef8.gif">|<img src="https://user-images.githubusercontent.com/99601412/172049433-85016c8f-2d01-40a1-89ee-4ab5b1d03d4c.gif">|
## OLED 출력
UI를 통해 10자 이하의 텍스트 혹은 정해진 4가지 감정표현을 OLED에 출력할 수 있습니다. 또한, RC카를 제어하고 있다면 RC카 동작 상태를 OLED에 출력합니다.

<p align="center"><img src="https://user-images.githubusercontent.com/99601412/172049261-fae07bd9-cfb9-4651-8958-4bb7862ac5fa.gif"></p>

## 센싱 데이터로 UI에 메세지 띄우기
초음파 센서를 통해 장애물이 가까워진 것을 인식한다면, UI에 `장애물 발견` 메세지를 출력합니다.
날씨가 더워져서, 온도 33도 이상이 되면, `33도 이상` 메세지를 출력합니다.
습도가 낮아지면, `습도 낮음` 메세지를 출력합니다.

<p align="center"><img src="https://user-images.githubusercontent.com/99601412/172048724-cf6548f5-9267-4556-ada1-cd208146df11.gif"></p>

## 센싱 데이터를 그래프로 시각화하기
Node.js로 제작한 백엔드에서 1 초 마다 DB에 저장된 Sensing Data를 받아와 프론트엔드로 넘겨줍니다. 프론트엔드는 해당 데이터를 Vue.js & Apexchart 라이브러리를 이용하여 그래프로 시각화 해줍니다.

<p align="center"><img src="https://user-images.githubusercontent.com/99601412/172048272-b0323ef1-e4ce-4ec4-89ef-eff0df2b10a0.gif"></p>

# 배운점
* Python에서 Timer을 사용하여 Main Thread와 별개로 `Timer Thread`가 작동하여 다중 Thread를 구현할 수 있다.
* AWS와 MySQL을 연동 및 MySQL에서의 Query문을 사용할 수 있습니다.
* Pycharm에서 Pyside를 이용하여 ui파일을 python 파일로 컨버터하여 개발할 수 있고, 개발 PC에서 개발 후 임베디드 시스템으로 옮기는 `크로스 컴파일`을 할 수 있습니다.
* 다양한 센서들을 통해 센싱한 데이터를 DB 서버에 올리고 올라간 데이터를 차트로 시각화하여 표현할 수 있습니다.
* DC모터와 서보모터를 모터 드라이버를 통해 제어하여 RC카를 제어할 수 있습니다.
# 한계점
* 기존 계획한 객체를 탐색하여 해당 객체를 쫒아가는 자동 운행 기능을 OpenCV & Tensorflow 라이브러리를 통해 구현하려 하였지만, 버벅임이 심해 구현하지 못하였습니다.
* UI를 통해 명령어를 전송할 떄, 빠른 주기로 전송하게 되면 간혹 명령어가 동작되지 않는 경우가 있습니다.
