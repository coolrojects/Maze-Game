// Maze.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include "Tile.h"
using namespace std;

void changeCursorPos(COORD& cursorPos, SHORT x, SHORT y){
    cursorPos.X = x;
    cursorPos.Y = y;
}
void move() {
    char direction;
    cin >> direction;
}
int main()
{
    CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    HANDLE hOutputBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOutputBuffer, &csbInfo);
    Tile tile{2, 1};
    int cols, rows;
    COORD pos{ 0, 0 };

    SMALL_RECT rect;
    rect.Top = 0;
    rect.Bottom = csbInfo.dwSize.Y / 2;
    rect.Left = 0;
    rect.Right = csbInfo.dwSize.X;
    SMALL_RECT rectclip{ rect };
    COORD temp{ -100, -100 };
    CHAR_INFO ch;
    ch.Attributes = 0x0000;
    ch.Char.AsciiChar = ' ';
    short count = 5;
    tile.wait(milliseconds(2000));
    while (cin && count <= 25) {
        rows = cols = count;
        cout << "rows: " << rows << "\n";
        cout << "Cols: " << cols;
        tile.wait((milliseconds)1000);
        ScrollConsoleScreenBuffer(hOutputBuffer, &rect, &rectclip, temp, &ch);
        tile.buildMaze(cols, rows, hOutputBuffer);
        tile.wait((milliseconds)2000);
        ScrollConsoleScreenBuffer(hOutputBuffer, &rect, &rectclip, temp, &ch);
        SetConsoleCursorPosition(hOutputBuffer, pos);
        count += 5;
    }
    
    
}
