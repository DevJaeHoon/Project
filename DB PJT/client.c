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

// AWS���� ���� IP �� ��Ʈ
const char *IP = "3.36.74.128";
const char *PORT = "12345";

pthread_t send_tid;
pthread_t receive_tid;
int sock;

void *receiveMsg() {
	char buf[100]; // 100�� ����

	while(1) {
		memset(buf, 0, 100);
		int len = read(sock, buf, 99);
		if (len == 0) 
		{
			printf("[INFO] Server Disconnected\n");
			kill(0, SIGINT);  // ��� ���μ����� SIGINT ��ȣ ������
			break;
		}

		printf("(server) %s\n", buf);
	}
}

void *sendMsg() {
	char str[100]; // 100�� ����
	sock = 0;

	while(1) {
		printf("SHELL > ");
		fgets(str, 100, stdin);
		
		str[strlen(str) - 1] = '\0'; // �������� \0�� �ٿ��־� ������ ���� �˱� ����

		if (strlen(str) == 0)  continue;  // �Է� ������ �ѱ��

		else if (!strcmp(str, "exit"))
		{
			write(sock, str, strlen(str)); // exit ���� ������ ����
			break;
		}
		else if (!strcmp(str, "connect")) // ���� ����
		{
			if (sock != 0)
			{	
				printf("[INFO] Already connected\n");
				continue;
			}

			//Socket Create
			sock = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ����
			if (sock == -1) 
			{
				printf("[ERROR] 1_Socket Create Error\n");
				exit(1);
			}

			// ����ü ������ ������ IP�� PORT ���� �ʱ�ȭ
			struct sockaddr_in addr = {0};
			addr.sin_family = AF_INET; // IPv4
			addr.sin_addr.s_addr = inet_addr(IP); // IP ���ڸ� int32�� �� ����� ������� ��ȯ
			addr.sin_port = htons(atoi(PORT));  // Port�� short int�� �� ����� ������� ��ȯ
		
			// ������ ���� ��û
			if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
			{
				printf("[ERROR] 2_Connect Error\n");
				exit(1);
			}
	
			pthread_create(&receive_tid, NULL, receiveMsg, NULL); // ���� ������ ���� - ȣ���Լ� : receiveMsg
		}

		// save key:value�� ���⸦ ������ �� 5���ڰ� save�ϰ�� ����
		else if (!strncmp(str, "save ", 5) && strlen(str) >= 8) // ������ key�� value ����
		{
			if (sock == 0) 
			{
				printf("[INFO] PLEASE, CONNECT WITH SERVER\n");
				continue;
			}
			
			char buf[100] = "save_";
			strcat(buf, &str[5]); // key�� value �����̱�
			
			write(sock, buf, strlen(buf)); // save_[key][value] ���� ������ ����
		}
		// read key�� ���⸦ ������ �� 5���ڰ� read�ϰ�� ����
		else if (!strncmp(str, "read ", 5) && strlen(str) >= 6) // key�� �ش��ϴ� value �б�
		{
			if (sock == 0) 
			{
				printf("[INFO] PLEASE, CONNECT WITH SERVER\n");
				continue;
			}
			char buf[100] = "read_";
			strcat(buf, &str[5]); // key �����̱�
				
			write(sock, buf, strlen(buf)); // read_[key] ������ ����
			usleep(100 * 1000);
		}
		else if (!strcmp(str, "close"))
		{
			if (sock == 0) 
			{
				printf("[INFO] PLEASE, CONNECT WITH SERVER\n");
				continue;
			}
			
			pthread_cancel(receive_tid); // ���� ������ ����
			pthread_join(receive_tid, 0); // ���� ������ ����
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

	// ����,���� ������ ����
	pthread_cancel(send_tid); 
	pthread_cancel(receive_tid); 

	// ����,���� ������ ����
	pthread_join(send_tid, 0); 
	pthread_join(receive_tid, 0); 

	close(sock);
	exit(1);
}

int main()
{
	signal(SIGINT, interrupt); // SIGINT ��ȣ �߻��� interrupt ����

	pthread_create(&send_tid, NULL, sendMsg, NULL); // ���� ������ ���� - ���� �Լ� : sendMsg

	pthread_join(send_tid, 0);
	pthread_join(receive_tid, 0);
	
	close(sock);

	return 0;
}