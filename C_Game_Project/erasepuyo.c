#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<conio.h>


void erasePuyo(int _x, int _y, int _cell) {
    if (cells[_y][_x] != _cell)
        return;

    cells[_y][_x] = CELL_NONE;

    for (int i = 0; i < PUYO_ANGLE_MAX; i++) {
        int x = _x + puyoSubPotion[i][0];
        int y = _y + puyoSubPotion[i][1];
        erasePuyo(x, y, _cell);
    }
}