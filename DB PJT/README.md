## DataBase 웹 서버 구축
AWS서버와 클라이언트를 소켓 작성 프레임워크를 통해 연결하였고, 다중 쓰레드를 통해 `500명`까지 동시 접속할 수 있는 데이터베이스 서버를 구축하였습니다.

### 환경 
- AWS
- Ubuntu 20.04 LTS
- GCC
- Thread
- socket
- signal
- NtCat

## 실행방법
1. [AWS](https://signin.aws.amazon.com/signin?redirect_uri=https%3A%2F%2Fap-northeast-2.console.aws.amazon.com%2Fec2%2Fv2%2Fhome%3Fregion%3Dap-northeast-2%26state%3DhashArgs%2523LaunchInstanceWizard%253A%26isauthcode%3Dtrue&client_id=arn%3Aaws%3Aiam%3A%3A015428540659%3Auser%2Fec2&forceMobileApp=0&code_challenge=WrAiITH3zgku5jj4jk7tQa7Dvkeifl00dtT3NL30zBc&code_challenge_method=SHA-256)에서 로그인 및 서버를 만들어줍니다. TCP 연결을 위한 서버로 `포트와 IP`를 기억해둡니다.
2. `MobaXterm`에 AWS에서 생성한 TCP 서버를 연결해주고, `server.c`코드에서 `13번째 줄 port 부분`을 본인이 생성한 포트로 변경해줍니다.
3. `gcc ./server.c -D_REENTRANT -o ./server -lpthread`빌드해주고, `./server`를 통해 실행시켜줍니다.
4. `Ubuntu`와 연결해준 `MobaXterm`에는 `client.c` 코드에서 13, 14번째 줄의 IP와 PORT를 본인이 생성한 것으로 바꿔줍니다.
5. `gcc ./client.c -D_REENTRANT -o ./client -lpthread`빌드해주고, `./client`를 통해 실행시켜줍니다.
6. 더 많은 인원을 접속시켜주기 위해서는 4번과 5번 과정을 수행합니다.

## 프로젝트 설명
위 실행방법을 통해 실행시켰다면, 다음 명령어를 통해 사용할 수 있습니다.
- connect : 서버와 연결한다.
- save [key]:[vale] : key와 value 값을 서버에 저장합니다.
- read [key] : 서버에 저장된 key값의 value를 읽어옵니다
- close : 서버와 통신을 끊습니다.
- exit : 클라이언트를 종료시킵니다.

|클라이언트1|클라이언트2|서버|
|-|-|-|
|![클라이언트1](https://user-images.githubusercontent.com/99601412/164195046-4227adbb-eefa-4ee8-875f-b70374144fa0.png)|![클라이언트2](https://user-images.githubusercontent.com/99601412/164195062-79055c96-b5ef-49ec-9456-ae450d5b25a2.png)|![sever](https://user-images.githubusercontent.com/99601412/164195040-4294ff10-1586-4c8f-beb2-dd2a16d91efe.png)|

멀티 쓰레드를 통해 최대 `500명`까지 다중 접속이 가능하고, DB공간도 최대 `10000개` 저장가능합니다.

- 어느 클라이언트에서 key와 value를 저장하였다면, 다른 클라이언트에서 해당 key의 value값을 읽을 수 있는 DB 시스템입니다.
- 통신이 연결되지 않은 상태에서는 저장과 읽기가 되지 않습니다.
- 서버가 종료되면 Signal 신호로 인해 클라이언트도 종료됩니다. 하지만, 클라이언트가 종료되어도 서버는 종료되지 않습니다.

## 배운 점
- NetCat이라는 가짜 서버와 클라이언트를 통해 코드 유지보수에 큰 이점이 있음을 알았습니다.
- 서버/클라이언트 소켓 작성 프레임워크를 통해 통신 연결을 할 수 있습니다.
- 다중 쓰레드를 구현하여 동시 접속이 가능케 할 수 있습니다.
- Signal을 통해 프로세스를 제어할 수 있습니다.

