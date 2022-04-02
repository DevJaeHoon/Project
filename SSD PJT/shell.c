#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int testapp1();
int testapp2();

void help()
{
	system("cat ./help.txt");
}

int Correct_LBA(char* LBA) // LBA 범위 : 0 ~ 99 지키기
{
	int index = atoi(LBA);
	if (index < 0 || index > 99)
	{
		printf("Out of LBA range\n");
		return 0;
	}
	else return 1;
}

int Correct_value(char* value) // 0xAAAA0000 형식 지키기
{
	int len = strlen(value);
	if (value[0] != '0' || value[1] != 'x')
	{
		printf("0x format error\n");
		return 0;
	}
	for (int i = 2; i < len; i++)
	{
		if ((value[i] >= 'A' && value[i] <= 'F') || (value[i] >= '0' && value[i] <= '9'))continue;
		printf("hex format error\n");
		return 0;
	}
	return 1;
}

// user_input : 명령어, 명령내용 순으로 입력됩니다.
int go_cmd(char* user_input)
{
	// 명령어만 뽑아내기
	char* cmd = strtok(user_input, " ");

	// 명령어에 따른 수행
	if (!strcmp(cmd, "exit")) // 쉘 종료
	{
		return 1;
	}
	else if (!strcmp(cmd, "help")) // 명령어 소개
	{
		help();
	}
	else if (!strcmp(cmd, "write")) // 해당 LBA에 value 저장
	{
		// 명령어, LBA, value 순으로 입력받기 때문에, 아까 뽑아낸 명령어 다음부터 또 뽑아낸다.
		char* LBA = strtok(NULL, " ");
		char* value = strtok(NULL, " ");

		if (!Correct_LBA(LBA) || !Correct_value(value)) return 0;

		// ssd W 명령어 실행
		char ssd_cmd[100];
		sprintf(ssd_cmd, "./ssd W %s %s", LBA, value);
		system(ssd_cmd);

	}
	else if (!strcmp(cmd, "read")) // 해당 LBA 출력
	{
		// 명령어, LBA 순으로 입력받기 때문에, LBA 뽑아내기
		char* index = strtok(NULL, " ");

		if (!Correct_LBA(index)) return 0;

		// ssd R 명령어 실행
		char ssd_cmd[100];
		sprintf(ssd_cmd, " ./ssd R %s", index);
		system(ssd_cmd);

	}
	else if (!strcmp(cmd, "fullwrite")) // 해당 value 모든 LBA에 저장
	{
		// 명령어, value 순으로 입력받기 때문에 value 뽑아내기
		char* value = strtok(NULL, " ");

		if (!Correct_value(value)) return 0;

		char ssd_cmd[100];
		for (int i = 0; i < 100; i++)
		{
			sprintf(ssd_cmd, "./ssd W %d %s", i, value);
			system(ssd_cmd);
		}
	}
	else if (!strcmp(cmd, "fullread")) // 모든 value 출력
	{
		char ssd_cmd[100];
		for (int i = 0; i < 100; i++)
		{
			sprintf(ssd_cmd, "./ssd R %d", i);
			system(ssd_cmd);

			FILE* fp = fopen("./result.txt", "r");
			char ReadValue[20];
			fgets(ReadValue, 20, fp);
			printf("%s", ReadValue);
			fclose(fp);
		}
	}
	else if (!strcmp(cmd, "testapp1"))
	{
		int flag = testapp1();
		if (!flag)
		{
			printf("testapp1 failed\n");
		}
		else
		{
			printf("testapp1 succeed\n");
		}
	}
	else if (!strcmp(cmd, "testapp2"))
	{
		int flag = testapp2();
		if (!flag)
		{
			printf("testapp2 failed\n");
		}
		else
		{
			printf("testapp2 succeed\n");
		}
	}
	else
	{
		printf("incorrect command\n");
	}
	return 0;
}


// TestCase

int testapp1() {
	printf("---------------------------\n");
	printf("fullwrite & fullread Test\n");

	char Input[100] = "fullwrite 0xABCD1234";
	go_cmd(Input);

	char compare[20] = "0xABCD1234\n";
	int flag = 0;

	char ssd_cmd[100];
	for (int i = 0; i < 100; i++)
	{
		sprintf(ssd_cmd, "./ssd R %d", i);
		system(ssd_cmd);
		FILE* fp = fopen("./result.txt", "r");
		char ReadValue[20];
		fgets(ReadValue, 20, fp);

		printf("%s", ReadValue);
		if (strcmp(ReadValue, compare)) return 0;
		fclose(fp);
	}

	printf("---------------------------\n");

	return 1;
}


int testapp2() {
	printf("---------------------------\n");
	printf("write & read Test\n");


	char Input[100];

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			sprintf(Input, "write %d 0xAAAABBBB", j);
			go_cmd(Input);
		}
	}

	for (int i = 0; i < 5; i++)
	{
		sprintf(Input, "write %d 0x12345678", i);
		go_cmd(Input);
	}

	char compare[20] = "0x12345678\n";
	int flag = 0;

	for (int i = 0; i < 5; i++)
	{
		char ssd_cmd[100];
		sprintf(ssd_cmd, "./ssd R %d", i);
		system(ssd_cmd);

		FILE* fp = fopen("./result.txt", "r");
		char ReadValue[20];
		fgets(ReadValue, 20, fp);

		printf("%s", ReadValue);
		if (strcmp(ReadValue, compare)) return 0;
		fclose(fp);
	}

	printf("---------------------------\n");

	return 1;
}


int main()
{
	// nand.txt 100줄을 0x00000000으로 초기화
	FILE* fp = fopen("./nand.txt", "w");
	char value[20] = "0x00000000\n";
	for (int i = 0; i < 100; i++)
	{
		fputs(value, fp);
	}
    fclose(fp);

	char input[100];

	while(1)
	{
		printf("SSD SHELL >> ");
		gets(input);  // 띄어쓰기를 포함하여 한 줄을 읽어 input에 저장하기
		int flag = go_cmd(input);
		if(flag) break;
	}
	return 0;
}




