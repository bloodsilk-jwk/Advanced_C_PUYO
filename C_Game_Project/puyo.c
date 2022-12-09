#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>

struct PuyoField
{
    int displayBuffer;
    int cells;
};

#define FIELD_WIDTH 8
#define FIELD_HEIGHT 14

#define PUYO_START_X 3
#define PUYO_START_Y 1

#define PUYO_COLOR_MAX 4

enum {
    CELL_NONE,
    CELL_WALL,
    CELL_PUYO_0,
    CELL_PUYO_1,
    CELL_PUYO_2,
    CELL_PUYO_3,
    CELL_MAX
};

enum {
    PUYO_ANGLE_0,
    PUYO_ANGLE_90,
    PUYO_ANGLE_180,
    PUYO_ANGLE_270,
    PUYO_ANGLE_MAX
};
int puyoSubPotion[][2] = {
   {0,-1},//PUYO_ANGLE_0
   {-1,0},//PUYO_ANGLE_90
   {0,1},//PUYO_ANGLE_180
   {1,0}//PUYO_ANGLE_270
};

int checked[FIELD_HEIGHT][FIELD_WIDTH];

char cellNames[][2 + 1] = {
   "¤ý",//CELL_NONE
   "¡á",//CELL_WALL
   "£Ï",//CELL_PUYO_0
   "¡â",//CELL_PUYO_1
   "¡à",//CELL_PUYO_2
   "¡Ù",//CELL_PUYO_3
};

int puyoX = PUYO_START_X,
puyoY = PUYO_START_Y;
int puyoColor;
int puyoAngle;

int lock = 0;

void display(struct PuyoField *Field) {
    system("cls");
    memcpy(Field->displayBuffer, Field->cells, sizeof Field->cells);

    if (!lock) {
        int subX = puyoX + puyoSubPotion[puyoAngle][0];
        int subY = puyoY + puyoSubPotion[puyoAngle][1];
        Field->displayBuffer =
            Field->displayBuffer = CELL_PUYO_0 + puyoColor;
    }

    for (int y = 1; y < FIELD_HEIGHT; y++) {
        for (int x = 0; x < FIELD_WIDTH; x++)
            printf("%s", cellNames[Field->displayBuffer]);
        printf("\n");
    }
}

int intersectPuyoToField(int _puyoX, int _puyoY, int _puyoAngle, struct PuyoField *Field)
{
    if (Field->cells != CELL_NONE)
        return 1;

    int subX = _puyoX + puyoSubPotion[_puyoAngle][0];
    int subY = _puyoY + puyoSubPotion[_puyoAngle][1];

    if (Field->cells != CELL_NONE)
        return 1;

    return 0;
}

int getPuyoConnectedCount(int _x, int _y, int _cell, int _count) {
    if (
        checked[_y][_x]
        || (cells[_y][_x] != _cell)
        )
        return _count;

    _count++;
    checked[_y][_x] = 1;

    for (int i = 0; i < PUYO_ANGLE_MAX; i++) {
        int x = _x + puyoSubPotion[i][0];
        int y = _y + puyoSubPotion[i][1];
        _count = getPuyoConnectedCount(x, y, _cell, _count);
    }

    return _count;
}

extern void erasePuyo;

int main() {

    struct PuyoField Field[14][8];

    srand((unsigned int)time(NULL));

    for (int y = 0; y < FIELD_HEIGHT; y++)
        Field[y][0].cells =
        Field[y][FIELD_WIDTH - 1].cells = CELL_WALL;

    for (int x = 0; x < FIELD_WIDTH; x++)
        Field[FIELD_HEIGHT - 1][x].cells = CELL_WALL;

    puyoColor = rand() % PUYO_COLOR_MAX;

    time_t t = 0;
    while (1) {
        if (t < time(NULL)) {
            t = time(NULL);

            if (!lock) {
                if (!intersectPuyoToField(puyoX, puyoY + 1, puyoAngle, &Field)) {
                    puyoY++;
                }
                else {
                    int subX = puyoX + puyoSubPotion[puyoAngle][0];
                    int subY = puyoY + puyoSubPotion[puyoAngle][1];

                    Field[puyoY][puyoX].cells =
                        Field[subY][subX].cells = CELL_PUYO_0 + puyoColor;

                    puyoX = PUYO_START_X;
                    puyoY = PUYO_START_Y;
                    puyoAngle = PUYO_ANGLE_0;
                    puyoColor = rand() % PUYO_COLOR_MAX;

                    lock = 1;
                }
            }

            if (lock) {
                lock = 0;
                for (int y = FIELD_HEIGHT - 3; y >= 0; y--)
                    for (int x = 1; x < FIELD_WIDTH - 1; x++)
                        if (
                            (Field[y][x].cells != CELL_NONE)
                            && (Field[y + 1][x].cells == CELL_NONE)
                            ) {
                            Field[y + 1][x].cells = Field[y][x].cells;
                            Field[y][x].cells = CELL_NONE;
                            lock = 1;
                        }

                if (!lock) {
                    memset(checked, 0, sizeof checked);
                    for (int y = 0; y < FIELD_HEIGHT - 1; y++)
                        for (int x = 1; x < FIELD_WIDTH - 1; x++) {
                            if (Field[y][x].cells != CELL_NONE)
                                if (getPuyoConnectedCount(x, y, Field[y][x].cells, 0) >= 4) {
                                    erasePuyo(x, y, Field[y][x].cells);
                                }
                        }
                }
            }

            display(&Field);

        }

        if (_kbhit()) {
            if (lock)
                _getch();
            else {
                int x = puyoX;
                int y = puyoY;
                int angle = puyoAngle;
                switch (_getch()) {
                    //case 'w':y--;   break;
                case 's':y++;   break;
                case 'a':x--;   break;
                case 'd':x++;   break;
                case ' ':angle = (++angle) % PUYO_ANGLE_MAX;
                }
                if (!intersectPuyoToField(x, y, angle, &Field)) {
                    puyoX = x;
                    puyoY = y;
                    puyoAngle = angle;
                }

                display(&Field);
            }
        }
    }
}