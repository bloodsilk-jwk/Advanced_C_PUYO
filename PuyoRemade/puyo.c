#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <Windows.h>

#include "PuyoHeader.h"

struct display {
	int FieldOriginal[18][8];
	int FieldCopy[18][8];
};

#define FieldHeight 18 // 판 세로크기
#define FieldWidth 8 // 판 가로크기
#define MAIN_X_ADJ 3 //게임판 위치조정 
#define MAIN_Y_ADJ 1 //게임판 위치조정 

enum Objects {
	EMPTY,
	CEILLINGS,
	WALL,
	ACTIVEPUYO,
	PUYORED,
	PUYOGREEN,
	PUYOBLUE,
	PUYOYELLOW,
};

enum Keys {
	LEFT = 75,
	RIGHT = 77,
	DOWN = 80,
};

void gotoxy(int x, int y)
{
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main()
{
	srand((unsigned)time(NULL)); // 난수 생성
//	setcursortype(NOCURSOR); //커서 없앰 
	struct display Field;

	int Puyos[2][2] = {
		{ACTIVEPUYO,EMPTY},
		{ACTIVEPUYO,EMPTY}
	};

	ResetField(&Field);

	int key; // 키보드 입력값 저장
	int PuyoX, PuyoY; // 이동 중인 뿌요의 xy좌표 저장
	int CurrentPuyo; // 현재 뿌요
	int NewPuyoRequired = 0; //새로운 뿌요가 필요함을 알리는 flag 
	int isMerged = 0; // 접촉해서 붙었는가? 여부

	while (1)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawField(&Field);
		}
		if (NewPuyoRequired == 1)
		{
			NewPuyo(&PuyoX, &PuyoY, &CurrentPuyo, &NewPuyoRequired);
		}
	}
}

void ResetAll(int* key, int* isMerged)
{
	*key = 0;
	*isMerged = 0;
}

void ResetField (struct display* Field) // 게임판 초기화
{
	for (int y = 0; y < FieldHeight; y++) // 게임판 초기화
	{
		for (int x = 0; x < FieldWidth; x++)
		{
			Field->FieldOriginal[y][x] = 0;
			Field->FieldCopy[y][x] = 100;
		}
	}

	for (int x = 1; x < FieldWidth - 1; x++) // y = 3인 가로줄에 천장
	{
		Field->FieldOriginal[3][x] = CEILLINGS;
	}
	for (int y = 0; y < FieldHeight; y++)// 양 옆 끝의 세로줄에 벽
	{
		Field->FieldOriginal[y][0] = WALL;
		Field->FieldOriginal[y][FieldWidth - 1] = WALL;
	}
	for (int x = 1; x < FieldWidth - 1; x++) // y = 17인 가로줄에 바닥 벽
	{
		Field->FieldOriginal[FieldHeight - 1][x] = WALL;
	}

}

void ResetFieldCopy(struct display* Field) // FieldCopy 초기화
{
	for (int y = 0; y < FieldHeight; y++)
	{
		for (int x = 0; x < FieldWidth; x++)
		{
			Field->FieldCopy[y][x] = 100;
		}
	}
}

void DrawField(struct display* Field) // 게임판 그리기
{
	for (int x = 1; x < FieldWidth; x++) // 천장에는 새로운 블럭이 지나감
	{
		if (Field->FieldOriginal[3][x] == EMPTY)
		{
			Field->FieldOriginal[3][x] = CEILLINGS; // 지워지면 새로 그려줌
		}
	}

	for (int y = 0; y < FieldHeight; y++)
	{
		for (int x = 0; x < FieldWidth; x++)
		{
			if (Field->FieldOriginal[y][x] != Field->FieldCopy[y][x]) // Copy랑 비교해서 달라진 부분만 새로 그리기
			{
				gotoxy(MAIN_X_ADJ + x, MAIN_Y_ADJ + y);
				switch (Field->FieldOriginal[y][x])
				{
				case EMPTY:
					printf("ㆍ");
					break;
				case CEILLINGS:
					printf("--");
					break;
				case WALL:
					printf("▩");
					break;
				}
			}
		}
	}
	for (int y = 0; y < FieldHeight; y++)
	{
		for (int x = 0; x < FieldWidth; x++)
		{
			Field->FieldCopy[y][x] = Field->FieldOriginal[y][x];
		}
	}
}

void NewPuyo(int* PuyoX, int* PuyoY, int* CurrentPuyo, int* NewPuyoRequired)
{
	int NextPuyo = rand() % 4; // 다음 뿌요 만들기
	*PuyoX = (FieldWidth / 2) - 1; // 화면 x좌표의 중앙
	*PuyoY = 0; // y좌표 0에 배치
	*CurrentPuyo = NextPuyo; // 다음 뿌요 가져오기
	NextPuyo = rand() % 4;
	
	*NewPuyoRequired = 0;

	
}

void check_input(int* key, int* isMerged)
{
	*key = 0; // 키 값 초기화

	if (_kbhit()) // 키 입력 있을 때
	{
		*key = _getch(); // 키 값 받기
		if (*key == 224) // 방향키인 경우
		{
			do { *key = _getch(); } while (*key == 224);
			switch (*key)
			{
			case LEFT:
				break;
			case RIGHT:
				break;
			case DOWN:
				while (*isMerged == 0)
				{
					
				}
				break;
			}
		}
		else
		{
			switch (*key)
			{
			case 'A':
			case 'a':
				break;
			case 'D':
			case 'd':
				break;

			}
		}
	}
	while (_kbhit())
	{
		_getch();
	}
}

void DropPuyo(int* isMerged)
{
	if (*isMerged)
	{
		printf("");
	}
}

void RotatePuyo(int direction, int(*Puyos)[2])
{
	int Temp;

	switch (direction)
	{
	case LEFT:
		Temp = Puyos[0][0];
		Puyos[0][0] = Puyos[0][1];
		Puyos[0][1] = Puyos[1][1];
		Puyos[1][1] = Puyos[1][0];
		Puyos[1][0] = Temp;
		break;
	case RIGHT:
		Temp = Puyos[0][0];
		Puyos[0][0] = Puyos[1][0];
		Puyos[1][0] = Puyos[1][1];
		Puyos[1][1] = Puyos[0][1];
		Puyos[0][1] = Temp;
	}
	if (Puyos[1][0] == EMPTY && Puyos[1][1] == EMPTY)
	{
		Temp = Puyos[0][0];
		Puyos[0][0] = Puyos[1][0];
		Puyos[1][0] = Temp;
		Temp = Puyos[0][1];
		Puyos[0][1] = Puyos[1][1];
		Puyos[1][1] = Temp;
	}
}

void MovePuyo(int direction, struct display* Field)
{
	switch (direction)
	{
	case LEFT:
		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < 2; x ++)
			{

			}
		}
	}
}