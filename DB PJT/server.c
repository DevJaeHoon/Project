#include <stdio.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#define MAX_CLIENT_CNT 500
#define MAX_DB_SIZE 10000

const char *PORT = "12345";

int server_sock;
// 500�� �Ѱ�
int client_sock[MAX_CLIENT_CNT];
struct sockaddr_in client_addr[MAX_CLIENT_CNT];

char db_key[MAX_DB_SIZE][100];
char db_value[MAX_DB_SIZE][100];

pthread_t tid[MAX_CLIENT_CNT];
int exitFlag[MAX_CLIENT_CNT];


int getClientID() {
	for (int i = 0; i < MAX_CLIENT_CNT; i++) 
	{
		if (client_sock[i] == 0) return i;
	}

	return -1;
}

void interrupt(int arg) 
{
	printf("\nYou typped Ctrl + C\n");
	printf("Bye\n");

	for (int i = 0; i < MAX_CLIENT_CNT; i++) 
	{
		if (client_sock[i] != 0) 
		{
			pthread_cancel(tid[i]);
			pthread_join(tid[i], 0);
			close(client_sock[i]);
		}
	}
	close(server_sock);
	exit(1);
}

int setKeyValue(char *buf, char *key, char *value) 
{

	//value
	char *p = strstr(buf, ":"); // :[value]�� ù �ּҸ� p�� ����
	if (p == NULL) return -1;
	strcpy(value, p + 1); // value�� ����

	//key
	*p = '\0'; // : �ּҸ� null�� �ٲ� �� key������ ����
	strcpy(key, buf);
}

int getKeyWitch(char *key) 
{
	for (int i = 0; i < MAX_DB_SIZE; i++) 
	{
		if (!strcmp(db_key[i], key)) return i;
	}
	return -1;
}

int getBlankWitch() 
{	
	for (int i = 0; i < MAX_DB_SIZE; i++) 
	{
		if (db_key[i][0] == 0) return i;
	}

	return -1;
}

void saveData(char *buf, char *result)
{
	char key[100] = {0};
	char value[100] = {0};

	if (setKeyValue(buf, key, value) == -1) // key�� value ������
	{
		sprintf(result, "Wrong Command");
		return;
	}

	int idx = getKeyWitch(key); // key�� ��ġ�� index ã��
	if (idx == -1) // ����� key�� ���ٸ�
	{
		idx = getBlankWitch(); // ����� ���� ù��° index ã��
		if (idx == -1)
		{
			sprintf(result, "DB FULL"); // DB ����
			return;
		}
	}
	
	// key�� value ����
	strcpy(db_key[idx], key);
	strcpy(db_value[idx], value);

}

void readData(char *buf, char *result)
{
	char key[100] = {0};
	strcpy(key, buf); // key�� ����

	int idx = getKeyWitch(key); // key�� ��ġ�� index ã��
	if (idx == -1) 
	{
		sprintf(result, "There's no data");
		return;
	}

	strcpy(result, db_value[idx]);
}

void *client_handler(void *arg)
{
	int id = *(int *)arg;

	char name[100];
	strcpy(name, inet_ntoa(client_addr[id].sin_addr)); 
	printf("[INFO] Connect new Client (ID : %d, IP : %s)\n", id, name); // ���ӵ� Ŭ���̾�Ʈ ���� IP ���

	char buf[100];		
	char result[200];

	while(1) 
	{
		memset(buf, 0, 100);
		memset(result, 0, 200);
		int len = read(client_sock[id], buf, 99);
		if (len == 0) 
		{
			printf("[INFO] Disconnect with client.. BYE\n");
			exitFlag[id] = 1;
			break;
		}

		if (!strcmp("exit", buf)) 
		{
			printf("[INFO] Client want close.. BYE\n");
			exitFlag[id] = 1;
			break;
		}
	
		printf("[%d]%s : %s\n", id, name, buf); // ���� �ֿܼ� Ŭ���̾�Ʈ���� ���� ���� ���
	
		if (!strncmp("save_", buf, 5))
		{
			saveData(&buf[5], result); 
		}
		else if (!strncmp("read_", buf, 5)) 
		{
			readData(&buf[5], result);			
		}
		else 
		{
			sprintf(result, "[%s] is Wrong Command...", buf);
			printf("[%d]%s : %s\n", id, name, result);
		}

		write(client_sock[id], result, strlen(result));
	}

	close(client_sock[id]);	
}

int main()
{
	signal(SIGINT, interrupt); 
		
	server_sock = socket(PF_INET, SOCK_STREAM, 0); // TCP ���� ����
	if (server_sock == -1) 
	{
		printf("[ERROR] 1_Socket Create Error\n");
		exit(1);
	}

	// ���� ���� ����� ����
	int optval = 1;
	setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval));

	// IPv4, IP, Port �Ҵ�
	struct sockaddr_in server_addr = {0};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // �� ��ǻ�� IP�� int32�� �򿣵�� ������� ����
	server_addr.sin_port = htons(atoi(PORT));

	// �ּ� ���̱�
	if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) 
	{
		printf("[ERROR] 2_bind Error\n");
		exit(1);
	}

	// ���� ����
	if (listen(server_sock, 5) == -1) 
	{
		printf("[ERROR] 3_listen Error\n");
		exit(1);
	}

	while(1) {

		printf("Wait for next client...\n");

		int id = getClientID(); // ���ӵ� Ŭ���̾�Ʈ �� ���ϱ�
		int id_table[MAX_CLIENT_CNT];
		id_table[id] = id;

		if (id == -1) {
			printf("[WARNING] Client FULL\n");
			sleep(1);
		}

		// accpet : Ŭ���̾�Ʈ�� ���� ��û�� �����Ѵ�.
		socklen_t client_addr_len = sizeof(struct sockaddr_in);
		memset(&client_addr[id], 0, sizeof(struct sockaddr_in));
		client_sock[id] = accept(server_sock, (struct sockaddr *)&client_addr[id], &client_addr_len);
		if (client_sock[id] == -1) 
		{
			printf("[ERROR] 4_accept Error\n");
			break;
		}

		//Create Thread
		pthread_create(&tid[id], NULL, client_handler, (void *)&id_table[id]);


		//check ExitFlag
		for (int i = 0; i < MAX_CLIENT_CNT; i++)
		{
			if (exitFlag[i] == 1) {
				exitFlag[i] = 0;
				pthread_detach(tid[i]);
				pthread_join(tid[i], 0);
				client_sock[i] = 0;
			}
		}
	}

	close(server_sock);
	return 0;
}
