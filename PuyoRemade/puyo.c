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

#define FieldHeight 18 // �� ����ũ��
#define FieldWidth 8 // �� ����ũ��
#define MAIN_X_ADJ 3 //������ ��ġ���� 
#define MAIN_Y_ADJ 1 //������ ��ġ���� 

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
	srand((unsigned)time(NULL)); // ���� ����
//	setcursortype(NOCURSOR); //Ŀ�� ���� 
	struct display Field;

	int Puyos[2][2] = {
		{ACTIVEPUYO,EMPTY},
		{ACTIVEPUYO,EMPTY}
	};

	ResetField(&Field);

	int key; // Ű���� �Է°� ����
	int PuyoX, PuyoY; // �̵� ���� �ѿ��� xy��ǥ ����
	int CurrentPuyo; // ���� �ѿ�
	int NewPuyoRequired = 0; //���ο� �ѿ䰡 �ʿ����� �˸��� flag 
	int isMerged = 0; // �����ؼ� �پ��°�? ����

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

void ResetField (struct display* Field) // ������ �ʱ�ȭ
{
	for (int y = 0; y < FieldHeight; y++) // ������ �ʱ�ȭ
	{
		for (int x = 0; x < FieldWidth; x++)
		{
			Field->FieldOriginal[y][x] = 0;
			Field->FieldCopy[y][x] = 100;
		}
	}

	for (int x = 1; x < FieldWidth - 1; x++) // y = 3�� �����ٿ� õ��
	{
		Field->FieldOriginal[3][x] = CEILLINGS;
	}
	for (int y = 0; y < FieldHeight; y++)// �� �� ���� �����ٿ� ��
	{
		Field->FieldOriginal[y][0] = WALL;
		Field->FieldOriginal[y][FieldWidth - 1] = WALL;
	}
	for (int x = 1; x < FieldWidth - 1; x++) // y = 17�� �����ٿ� �ٴ� ��
	{
		Field->FieldOriginal[FieldHeight - 1][x] = WALL;
	}

}

void ResetFieldCopy(struct display* Field) // FieldCopy �ʱ�ȭ
{
	for (int y = 0; y < FieldHeight; y++)
	{
		for (int x = 0; x < FieldWidth; x++)
		{
			Field->FieldCopy[y][x] = 100;
		}
	}
}

void DrawField(struct display* Field) // ������ �׸���
{
	for (int x = 1; x < FieldWidth; x++) // õ�忡�� ���ο� ���� ������
	{
		if (Field->FieldOriginal[3][x] == EMPTY)
		{
			Field->FieldOriginal[3][x] = CEILLINGS; // �������� ���� �׷���
		}
	}

	for (int y = 0; y < FieldHeight; y++)
	{
		for (int x = 0; x < FieldWidth; x++)
		{
			if (Field->FieldOriginal[y][x] != Field->FieldCopy[y][x]) // Copy�� ���ؼ� �޶��� �κи� ���� �׸���
			{
				gotoxy(MAIN_X_ADJ + x, MAIN_Y_ADJ + y);
				switch (Field->FieldOriginal[y][x])
				{
				case EMPTY:
					printf("��");
					break;
				case CEILLINGS:
					printf("--");
					break;
				case WALL:
					printf("��");
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
	int NextPuyo = rand() % 4; // ���� �ѿ� �����
	*PuyoX = (FieldWidth / 2) - 1; // ȭ�� x��ǥ�� �߾�
	*PuyoY = 0; // y��ǥ 0�� ��ġ
	*CurrentPuyo = NextPuyo; // ���� �ѿ� ��������
	NextPuyo = rand() % 4;
	
	*NewPuyoRequired = 0;

	
}

void check_input(int* key, int* isMerged)
{
	*key = 0; // Ű �� �ʱ�ȭ

	if (_kbhit()) // Ű �Է� ���� ��
	{
		*key = _getch(); // Ű �� �ޱ�
		if (*key == 224) // ����Ű�� ���
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