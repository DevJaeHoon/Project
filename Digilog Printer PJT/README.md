# 디지로그 프린터

제작한 웹페이지에 있는 캔버스에 그림을 그리면 웹소켓으로 연결된 PHPoc 칩을 통하여 모터를 제어하여 캔버스에 그려진 그림과 동일하게 그려줍니다.

# HW 구성

- PHPoC 보드 : P4S-342 1EA
  - 동작전압 : 5V
- 스텝모터 제어보드 : PES-2405[S] 2EA
  - 동작전압 : 9V
- XY 플로터 몸체
  - 스텝모터 2EA
  - 서보모터 1EA

# 결과

- 캔버스에 그린 그대로 플로터가 종이에 그려준다.
  ![image](https://user-images.githubusercontent.com/99601412/190901730-6b3fc762-84d7-421c-8d4e-383cbf73d6d7.png)

# 설명

- 실제 동작 알고리즘
  ![image](https://user-images.githubusercontent.com/99601412/190901765-40ca13ce-f078-4d54-a6ae-58b94c2551ca.png)

- 좌표 전송 및 캔버스에 그리기
  ![image](https://user-images.githubusercontent.com/99601412/190901749-4ded0ab3-28de-4363-89a5-ec32607adb15.png)

- 모터 회전 방향에 따른 이동
  ![image](https://user-images.githubusercontent.com/99601412/190901903-33bbb0b4-8f18-412b-8367-30a499418cd9.png)

# 실행방법

1. [PHPoC Debugger](https://ko.phpoc.com/download.php#download_tools)를 다운받습니다.
2. `코드` dir에 있는 코드를 모두 `Debugger`에 업로드합니다.
3. 칩과 디바이스간 IP를 동일하게 합니다.
4. 디바이스에서 해당 IP를 접속하여 `Connect`버튼을 클릭하면 웹소켓이 연결됩니다.
5. 캔버스에 그림을 그리면 HW가 동작하게됩니다.

# 결과물 Directory

- 해당 Dir에서는 결과영상, 포스터, 보고서를 확인할 수 있습니다.
