#pragma once
#include <Windows.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <iostream>
#include <stack>
#include <random>
using namespace std;
using namespace std::chrono;
class Tile
{
public:
	struct TileSize {
		short width;
		short height;
	};
	struct Node {
		bool visited;
		int number;
		vector<int> connectingNodeNumber;
		int houseCol, rowStartNum, houseRow;
		int neighbourLeft, neighbourRight, neighbourUp, neighbourBelow;
		COORD startPos;
		COORD wallRightPos, wallUpPos;
	};

	Tile(short tileWidth, short tileHeight);
	void setTileSize(short xWidth, short yHeight);
	/// <summary>
	/// builds maze
	/// <summary>
	void buildMaze(short numberOfColumns, short numberOfRows, HANDLE& handleOutput);
	void createNodes(int size, int numOfCols);
	int chooseNeighbour(Node& house, short maxMazeRows, short maxMazeCols);
	int randomNumber(int min, int max);
	void numberedMaze(int numOfRows, int numOfCols);
	void removeNumberFromNumberedMaze(int numOfRows, int numOfCols, Node& house, char ch);
	void wait(milliseconds milliSecondsToWait);
	void buildWalls(int numOfCols);
	void setHouseMetaInfo(int numOfCols, Node& house);
};

