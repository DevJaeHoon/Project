#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>

// AWS에서 만든 IP 및 포트
const char *IP = "3.36.74.128";
const char *PORT = "12345";

pthread_t send_tid;
pthread_t receive_tid;
int sock;

void *receiveMsg() {
	char buf[100]; // 100자 제한

	while(1) {
		memset(buf, 0, 100);
		int len = read(sock, buf, 99);
		if (len == 0) 
		{
			printf("[INFO] Server Disconnected\n");
			kill(0, SIGINT);  // 모든 프로세스에 SIGINT 신호 보내기
			break;
		}

		printf("(server) %s\n", buf);
	}
}

void *sendMsg() {
	char str[100]; // 100자 제한
	sock = 0;

	while(1) {
		printf("SHELL > ");
		fgets(str, 100, stdin);
		
		str[strlen(str) - 1] = '\0'; // 마지막에 \0을 붙여주어 문자의 끝을 알기 위해

		if (strlen(str) == 0)  continue;  // 입력 없을시 넘기기

		else if (!strcmp(str, "exit"))
		{
			write(sock, str, strlen(str)); // exit 구문 서버에 전송
			break;
		}
		else if (!strcmp(str, "connect")) // 서버 접속
		{
			if (sock != 0)
			{	
				printf("[INFO] Already connected\n");
				continue;
			}

			//Socket Create
			sock = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓 생성
			if (sock == -1) 
			{
				printf("[ERROR] 1_Socket Create Error\n");
				exit(1);
			}

			// 구조체 변수에 서버의 IP와 PORT 정보 초기화
			struct sockaddr_in addr = {0};
			addr.sin_family = AF_INET; // IPv4
			addr.sin_addr.s_addr = inet_addr(IP); // IP 문자를 int32형 빅 엔디안 방식으로 변환
			addr.sin_port = htons(atoi(PORT));  // Port를 short int형 빅 엔디안 방식으로 변환
		
			// 서버와 연결 요청
			if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
			{
				printf("[ERROR] 2_Connect Error\n");
				exit(1);
			}
	
			pthread_create(&receive_tid, NULL, receiveMsg, NULL); // 수신 쓰레드 생성 - 호출함수 : receiveMsg
		}

		// save key:value로 띄어쓰기를 포함한 앞 5글자가 save일경우 실행
		else if (!strncmp(str, "save ", 5) && strlen(str) >= 8) // 서버에 key와 value 저장
		{
			if (sock == 0) 
			{
				printf("[INFO] PLEASE, CONNECT WITH SERVER\n");
				continue;
			}
			
			char buf[100] = "save_";
			strcat(buf, &str[5]); // key와 value 덧붙이기
			
			write(sock, buf, strlen(buf)); // save_[key][value] 구문 서버에 전송
		}
		// read key로 띄어쓰기를 포함한 앞 5글자가 read일경우 실행
		else if (!strncmp(str, "read ", 5) && strlen(str) >= 6) // key에 해당하는 value 읽기
		{
			if (sock == 0) 
			{
				printf("[INFO] PLEASE, CONNECT WITH SERVER\n");
				continue;
			}
			char buf[100] = "read_";
			strcat(buf, &str[5]); // key 덧붙이기
				
			write(sock, buf, strlen(buf)); // read_[key] 서버에 전송
			usleep(100 * 1000);
		}
		else if (!strcmp(str, "close"))
		{
			if (sock == 0) 
			{
				printf("[INFO] PLEASE, CONNECT WITH SERVER\n");
				continue;
			}
			
			pthread_cancel(receive_tid); // 전송 쓰레드 삭제
			pthread_join(receive_tid, 0); // 전송 쓰레드 정리
			close(sock);
			sock = 0;
		}
		else 
		{
			printf("[INFO] Wrong Command\n");
		}
	}
}

void interrupt(int arg)
{
	printf("\nYou typped Ctrl + C\n");
	printf("Bye\n");

	// 전송,수신 쓰레드 삭제
	pthread_cancel(send_tid); 
	pthread_cancel(receive_tid); 

	// 전송,수신 쓰레드 정리
	pthread_join(send_tid, 0); 
	pthread_join(receive_tid, 0); 

	close(sock);
	exit(1);
}

int main()
{
	signal(SIGINT, interrupt); // SIGINT 신호 발생시 interrupt 실행

	pthread_create(&send_tid, NULL, sendMsg, NULL); // 전송 쓰레드 생성 - 실행 함수 : sendMsg

	pthread_join(send_tid, 0);
	pthread_join(receive_tid, 0);
	
	close(sock);

	return 0;
}