#pragma once
#include <Windows.h>
#include <vector>
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
		int connectingNodeNumber;
	};
	Tile(short tileWidth, short tileHeight);
	void setTileSize(short xWidth, short yHeight);
	/// <summary>
	/// builds maze
	/// <summary>
	void buildMaze(short numberOfColumns, short numberOfRows, HANDLE& handleOutput);
	void createNodes(int size);
	int chooseNeighbour(int houseNumber, short maxMazeRows, short maxMazeCols);
	int randomNumber(int min, int max);
};

