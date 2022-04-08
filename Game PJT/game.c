#include <pthread.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h> // 한글 및 특수기호 출력을 위한 LIB
#include <unistd.h>
#include <time.h> 
#include <stdlib.h>

#define N 10
char map[N][N + 1] = {  // 10 x 10 게임
    "##########",
    "#   # ^ a#",
    "#  ^  ####",
    "##^ ^^#a #",
    "### #  ^ #",
    "#^ ^ ^# ^#",
	"# #  ^ ^##",
	"## ^^ ^#Y#",
	"#  ##    #",
    "##########",
};

// 주인공 위치
int ny = 1;
int nx = 1;

// 몬스터 위치들
int m1y = 4;
int m1x = 9;

int m2y = 8;
int m2x = 6;

int hp = 100;
 
void *M1() // rand 함수를 통해 몬스터1 1칸씩 무작위 이동
{
    int direct[4][2] = 
	{
		-1,0,
		1,0,
		0,1,
		0,-1
	};

    while(1) {

        int t = (int)rand()%4;
        int sely = m1y + direct[t][0];
        int selx = m1x + direct[t][1];

        if (map[sely][selx] == '#') continue; // 벽 만날시 벽 통과 안되도록

        m1y = sely;
        m1x = selx;
        usleep(700 * 1000); // 0.7초에 한번 이동
    }

    return 0;
}

void *M2() // rand 함수를 통해 몬스터2 1칸씩 무작위 이동
{
    int direct[4][2] = 
	{
		-1,0,
		1,0,
		0,1,
		0,-1
	};

    while(1) {

        int t = (int)rand()%4;
        int sely = m2y + direct[t][0];
        int selx = m2x + direct[t][1];

        if (map[sely][selx] == '#') continue;

        m2y = sely;
        m2x = selx;
        usleep(300*1000); // 0.3초에 한번 이동
    }

    return 0;
}

void print() // 화면에 보여주기 위한 함수
{
    clear(); 
    for (int y = 0; y<N; y++) { // 특수문자를 사용하면 공간이 2Byte이므로 빈칸을 사용한다.
        for (int x = 0 ; x<N; x++) {
            if (y == ny && x == nx) {
                printw("♘ ");  // 주인공 출력
            }
            else if (y == m1y && x == m1x) {
                printw("☠ "); // 몬스터 1
            }
			else if (y == m2y && x == m2x){
				printw("☣ "); // 몬스터 2
			}
            else if (map[y][x] == '#') {
                printw("▇▇"); // 벽
            }
            else if (map[y][x] == '^') {
                printw("^^"); // 가시
            }
            else if (map[y][x] == 'Y') {
                printw("♔ "); // 도착지
            }
            else if (map[y][x] == 'a') {
                printw("♡ "); // 약
            }
            else if (map[y][x] == ' ') {
                printw("  ");
            }
        }
        printw("\n");
    }
    printw("HP : %d\n", hp);  // 하단 HP 표시
    refresh(); // 화면에 띄우는 함수
}

void intro() // 게임 시작전 실행되는 게임 소개 함수
{
	clear();
	printw("====================================================================\n");
	printw("                              게임설명\n");
	printw("====================================================================\n\n");
	printw("주인공 ♘ 으로 ☠를 피해 ♔ 도달하면 승리하는 게임입니다\n");
	printw("☠을 만나면 주인공은 죽게 되고 HP가 0이 되면 역시 죽게 됩니다\n");
	printw("^^을 밞으면 HP가 10줄게 되고 ♡을 먹으면 체력이 30 증가 합니다. \n\n");
	printw("====================================================================\n");
	printw("오른쪽 방향키를 입력하면 3초후 게임이 시작됩니다.\n");

}

int main()
{
    srand(time(NULL));

    setlocale(LC_CTYPE, "ko_KR.utf8"); // 한글 입력 및 특수문자 사용하기 위해
    initscr();

	
    keypad(stdscr, TRUE); // 방향키 입력이 가능하도록 세팅
	curs_set(0); // 커서 지우기
	
	while(1){
		intro(); // 게임 설명 실행
		int enter = getch(); // 키 입력 받기
		if(enter == KEY_RIGHT) // 오른쪽 방향키 입력시 게임 실행 카운트
		{
			clear();
			mvprintw(5,5,"3");
			refresh();
			sleep(1);
			clear();
			mvprintw(5,5,"2");
			refresh();
			sleep(1);
			clear();
			mvprintw(5,5,"1");
			refresh();
			sleep(1);
			clear();
			mvprintw(5,5,"START");
			refresh();
			sleep(1);
			clear();
			break;
		}
	}
	

   	nodelay(stdscr, TRUE); // 키를 입력받을 때까지 대기하는게 아니도록 설정하기 위해

    pthread_t tid[2]; // thread 2개 추가 생성
    pthread_create(&tid[0], NULL, M1, NULL);
    pthread_create(&tid[1], NULL, M2, NULL);

    while(1) {
        print();
        int ch = getch();
        if (ch == ERR) ch = 0; // 입력받는게 없다면 0으로 설정

        // 방향키 입력에 따라 주인공 이동, 가시 밞을시 체력 10 감소
        if (ch == KEY_LEFT) {
            if (map[ny][nx - 1] != '#') nx--;
			if (map[ny][nx] == '^') hp -= 10;
        }
        if (ch == KEY_RIGHT) {
            if (map[ny][nx + 1] != '#') nx++;
			if (map[ny][nx] == '^') hp -= 10;
        }
        if (ch == KEY_UP) {
            if (map[ny - 1][nx] != '#') ny--;
			if (map[ny][nx] == '^') hp -= 10;
        }
        if (ch == KEY_DOWN) {
            if (map[ny + 1][nx] != '#') ny++;
			if (map[ny][nx] == '^') hp -= 10;
        }
		
	    // 하트 먹을 시 체력 30 증가 및 하트 삭제
        if (map[ny][nx] == 'a') {
            hp += 30;
            map[ny][nx] = ' ';
        }

        // 몬스터 만나거나 hp 0일시 게임 종료
        if ((ny == m1y && nx == m1x) ||(ny == m2y && nx == m2x)|| hp == 0) {
            print();
            usleep(500000);
            clear();
            mvprintw(10,30,"GAME OVER");
            refresh();
            sleep(1);
            break;
        }
        // 목적지 도착시 게임 승리
        if (map[ny][nx] == 'Y') {
            print();
            usleep(500000);
            clear();
            mvprintw(10,30,"YOU WIN");
            refresh();
            sleep(1);
            break;
        }
  }

  // thread 종료 시 데이터 정리
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
	
  getch();
  endwin();
  return 0;
}
