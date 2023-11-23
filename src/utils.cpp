#include "utils.h"
#include <conio.h>
#include <ctime>
#include <windows.h>
#include <iostream>

void SetCursor(const int x, const int y)
{
    COORD position;
    // position.X = x * 2; // 这里乘2是因为一个字符占两个位置（美观）
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}

void SetColor(const int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
