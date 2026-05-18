#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define NUM 6
#define MAX_WATER 100
#define WATER_AMOUNT 3

typedef struct {
	int water;  // 물 높이 (0 ~ 100)
	int isAlive;     // 물고기 생존 여부 (1: 생존, 0: 사망)
	int fishSize;
} FishTank;

//전역변수 선언
int level;
FishTank arrayFishes[NUM];      
long totalElapsedTime;      
long prevElapsedTime;       
long startTime = 0;                 
int position = 0;           //선택된 어항 번호
FishTank* cursor;
long elapsedTime = 0;
long lastDecreaseTime = 0;  // 전역변수 추가

//함수 선언
void initData();
void decreaseWater(long elapsedTime);
int checkFishAlive();
void update();
void render();
int checkWin();
void gotoxy(int x, int y);
int nonBlockingGetch();

// 입력 처리 관련 함수
void processInput();
void moveCursorLeft();
void moveCursorRight();
void giveWater();

int main(void)					//매개변수 받지 않음
{
	int gameOver = 0;
	initData();
	startTime = time(NULL);    //프로그램 실행 이후 현재까지 흐른 시간 저장
	//구조체 포인터 변수 선언
	cursor = arrayFishes;

	while (!gameOver)
	{
		// 키 입력 처리
		processInput();
		//게임 데이터 업데이트
		update();
		position = cursor - arrayFishes;
		//게임 화면 출력
		render();

		//레벨업 체크, 난이도 조정
		if (totalElapsedTime / 20 > level - 1) 
		{
			level++;        //레벨업
			for (int i = 0; i < NUM; i++)
			{ 
				arrayFishes[i].fishSize++;
			}
			gotoxy(10, 3);
			printf("축하합니다. 레벨업이 되었습니다.");
			printf("레벨업을 해서 물고기가 성장했어요!");
		}

		//게임 승패 체크
		if (checkWin()) 
		{
			gameOver = 1;
		}

		if (checkFishAlive() == 0) 
			{
				gotoxy(2, 3);
				printf("모든 물고기가 죽었습니다. 게임을 종료합니다.");
				gameOver = 1;
			}
	
		Sleep(100);     //화면 깜빡임과 CPU 사용량을 줄이기 위한 짧은 대기
	}

	getchar();          //게임 화면 유지
	system("cls");
	return 0;
}

//초기화
void initData() 
{
	level = 1;
	prevElapsedTime = 0;
	totalElapsedTime = 0;

	for (int i = 0; i < NUM; i++)		//물 높이, 생존 여부, 크기 초기화
	{
		arrayFishes[i].water = 100;  
		arrayFishes[i].isAlive = 1;
		arrayFishes[i].fishSize = 1;
	}
}

//입력 처리
void processInput() 
{
	int input;
	input = nonBlockingGetch();     // 키입력이 없으면 -1을 리턴
	if (input == -1) 
	{
		return;                     // 입력이 없으면 아무 일도 하지 않고 종료
	}
	switch (input) 
	{
	case 'j':                       
		moveCursorLeft();			// 왼쪽 이동
		break;
	case 'l':                       
		moveCursorRight();			// 오른쪽 이동
		break;
	case 'k':                       
		giveWater();				// 물주기
		break;
	default:
		gotoxy(2, 12);
		printf("잘못된 입력입니다.");
		break;
	}
}

// 커서를 왼쪽으로 이동
void moveCursorLeft() 
{
	if (cursor > arrayFishes) 
	{
		cursor--;
	}
}

// 커서를 오른쪽으로 이동
void moveCursorRight() 
{
	if (cursor < arrayFishes + NUM - 1) 
	{
		cursor++;
	}
}

// 현재 커서가 가리키는 어항에 물주기
void giveWater() 
{

	if (cursor->isAlive == 0) {
		gotoxy(2, 12);
		printf("이미 죽은 물고기에게는 물을 줄 수 없습니다!");
		return;
	}

	cursor->water += WATER_AMOUNT;

	if (cursor->water > MAX_WATER)
	{
		cursor->water = MAX_WATER;
	}
	prevElapsedTime = totalElapsedTime;
}

// 데이터를 갱신 함수
void update()
{
	totalElapsedTime = time(NULL) - startTime;

	// 물 증발용: 매 루프 간격만 사용
	long diff = totalElapsedTime - lastDecreaseTime;
	if (diff >= 1)
	{
		decreaseWater(diff);
		lastDecreaseTime = totalElapsedTime;
	}

	// 화면 출력용: 물 준 이후 경과 시간
	elapsedTime = totalElapsedTime - prevElapsedTime;
}

// 화면 출력 함수
void render() 
{
	system("cls");

	//게임 이름 표시
	gotoxy(2, 2);
	printf("@물고기 키우기 프로젝트@");

	//게임 레벨 표시
	gotoxy(2, 3);
	printf("Level:%2d", level);

	//총 경과 시간 표시
	gotoxy(2, 4);
	printf("총 경과 시간: %ld초", totalElapsedTime);

	//최근 경과 시간 표시
	gotoxy(2, 5);
	printf("최근 경과 시간: %ld초", elapsedTime);

	//어항 번호 출력
	gotoxy(2, 6);
	for (int i = 0; i < NUM; i++) 
	{
		printf("  [%d] ", i);
	}

	//어항 물높이 출력
	gotoxy(2, 7);
	for (int i = 0; i < NUM; i++) 
	{
		printf(" %4d ", arrayFishes[i].water);
	}

	//현재 커서의 위치 표시
	gotoxy(2, 8);
	printf("   ");
	for (int i = 0; i < position; i++) 
	{
		printf("      ");
	}
	printf("*");

	gotoxy(2, 9);
	for (int i = 0; i < 6; i++)
	{
		if (arrayFishes[i].water >= 70)
			printf(" 건강 ");

		else if (arrayFishes[i].water >= 30)
			printf(" 주의 ");

		else if (arrayFishes[i].water >= 1)
			printf(" 위험 ");

		else
			printf(" 사망 ");
	}

	//키 입력 방법 고지
	gotoxy(2, 10);
	printf("왼쪽(j)  물주기(k)  오른쪽(l)");
}


	void decreaseWater(long elapsedTime) //물 증발시킴, 물 높이 0이면 0으로 고정
	{
		for (int i = 0; i < NUM; i++)
		{
			//성장하고 나서 +증발량 

			if (arrayFishes[i].fishSize > 1)
			{
				arrayFishes[i].water -= ((2 * arrayFishes[i].fishSize) * elapsedTime);
			}
			else
			{
				arrayFishes[i].water -= (2 * elapsedTime);
			}
			if (arrayFishes[i].water <= 0)
			{
				arrayFishes[i].water = 0;
				arrayFishes[i].isAlive = 0;
			}

		}
	}

int checkWin() {
	if (level == 5) {
		gotoxy(2, 3);
		printf("최고 레벨을 달성하셨습니다. 게임을 종료합니다.");
		return 1;
	}
	return 0;
}

int checkFishAlive() //생존 여부 확인
{
	for (int i = 0; i < NUM; i++)
	{
		if (arrayFishes[i].water > 0)
		{
			return 1;
		}
	}
	return 0;
}

//콘솔창 출력 위치 지정
void gotoxy(int x, int y)
{
	COORD CursorPosition = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), CursorPosition);
}

//Non-blocking getch() 구현
int nonBlockingGetch() {
	if (_kbhit()) {
		return _getch();  // 키가 입력되었으면 입력된 키를 반환
	}
	return -1;            // 키가 입력되지 않았다면 -1을 반환
}