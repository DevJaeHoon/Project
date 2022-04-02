#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char mem[100][20];

void write(int index, char* value) 
{
	// nand.txt를 읽기 모드로 파일 열기
	FILE* fp = fopen("./nand.txt", "r");
	if (fp == NULL) 
	{
		printf("file not found\n");
	}

	int buf_index = 0;

	// nand.txt에 있는 모든 줄을 가져오고, 입력받은 index의 value만 변경 후 다시 보내는 방식
	while (1) 
	{
		char buf[20];
		char* log = fgets(buf, 20, fp);
		if (log == NULL) break;

		strcpy(mem[buf_index++], buf);
	}
	fclose(fp);

	char value_input[20];
	sprintf(value_input, "%s\n", value);
	strcpy(mem[index], value_input);  // ??? strcpy(mem[index], value);


	fp = fopen("./nand.txt", "w");
	for (int i = 0; i < 100; i++) 
	{
		fputs(mem[i], fp);
	}

	fclose(fp);
}

void read(int index) 
{
	// nand.txt를 읽기 모드로 파일 열기
	FILE* fp = fopen("./nand.txt", "r");
	if (fp == NULL) 
	{
		printf("file not found\n");
	}

	int buf_index = 0;

	while (1) {
		char buf[20];
		char* log = fgets(buf, 20, fp);
		if (log == NULL) break;

		strcpy(mem[buf_index++], buf);
	}
	fclose(fp);

	fp = fopen("./result.txt", "w");
	fputs(mem[index], fp);
	fclose(fp);
}

// 명령어 ssd W/R LBA value로 들어온다.
int main(int argc, char* argv[]) 
{	
	char mode[2];
	strcpy(mode, argv[1]); // W : 쓰기 , R : 읽기
	char LBA[3];
	strcpy(LBA, argv[2]); // LBA 주소

	if (!strcmp(mode, "W"))
	{
		char value[100];
		strcpy(value, argv[3]); // value

		int index = atoi(LBA);
		// 해당 LBA에 value 저장
		write(index, value);
	}
	else if (!strcmp(mode, "R")) 
	{
		int index = atoi(LBA);
		// 해당 LBA의 value 읽기
		read(index);
	}
	else 
	{
		printf("incorrect command\n");
	}

	return 0;
}





