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
int main()
{
    CONSOLE_SCREEN_BUFFER_INFO csbInfo;
    HANDLE hOutputBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hOutputBuffer, &csbInfo);
    Tile tile{4, 2};
    tile.buildMaze(9, 9, hOutputBuffer);
    
    
}
