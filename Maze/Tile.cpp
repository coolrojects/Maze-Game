
#include "Tile.h"
#include <iostream>
#include <stack>
#include <random>
using namespace std;

//ToDO:
// 1. buildMaze function
// 
//variables
Tile::TileSize tileSize;
vector<Tile::Node> nodes;


//methods

/// <summary>
/// stores how many character width and height make each tile
/// </summary>
void Tile::setTileSize(short xWidth, short yHeight) {
	tileSize.width = xWidth;
	tileSize.height = yHeight;
}
/// <summary>
/// nothing
/// </summary>
/// <param name="tileWidth"></param>
/// <param name="tileHeight"></param>
Tile::Tile(short tileWidth, short tileHeight) {
	setTileSize(tileWidth, tileHeight);
}

/// <summary>
/// creates nodes list with the given size and each with respect to their tile number
/// </summary>
/// <param name="size"></param>
void Tile::createNodes(int size) {
	nodes.resize(size);
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].number = i;
	}
}

int Tile::randomNumber(int min, int max) {
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(min, max);
	return distr(eng);

}

int Tile::chooseNeighbour(int houseNumber, short maxMazeRows, short maxMazeCols) {
	vector<int> neighbours;
	int houseRow = (int)(houseNumber /maxMazeCols);
	int rowStartNum = houseRow * (maxMazeCols);
	int houseCol = houseNumber - rowStartNum;
	cout << "\ncol: " << houseCol << " rowStart" << rowStartNum;
	//left neighbour
	if (houseCol > 0) {
		neighbours.push_back(houseNumber - 1);
	}
	//right neighbour
	if (houseNumber < maxMazeCols - 1) {
		neighbours.push_back(houseNumber + 1);
	}
	//if node not in 0th row then find upper neighbour
	if (houseRow > 0) {
		int upperNeighbour = ((houseRow - 1) * maxMazeCols) + houseCol;
		cout << "\nupperNeighbour: " << upperNeighbour;
		neighbours.push_back(upperNeighbour);
	}
	//if node not in last row then find below neighbour
	if (houseRow < (maxMazeRows - 1)) {
		int belowNeighbour = ((houseRow + 1) * maxMazeCols) + houseCol;
		cout << "\nbelowNeighbour: " << belowNeighbour;
		neighbours.push_back(belowNeighbour);
	}
	int randomValue = randomNumber(0, neighbours.size() - 1);
	return neighbours[randomValue];

}
/// <summary>
/// builds maze with specified number of columns and rows
/// </summary>
/// <param name="numberOfColumns"></param>
void Tile::buildMaze(short numberOfColumns, short numberOfRows, HANDLE &handleOutput) {
	int visitedCount = 0;
	stack<Node> visitedNodeStack;
	createNodes(numberOfColumns * numberOfRows);

	//connect nodes and build walls around each node where nodes are not connected
	/*while (visitedCount < (numberOfColumns * numberOfRows)) {
	* 	int neighbour = chooseNeighbour(0, numberOfRows, numberOfColumns);
	if (!nodes[neighbour].visited) {
		nodes[neighbour].visited = true;
		visitedNodeStack.push(nodes[neighbour]);
		cout << "\nnode Nmber: " << visitedNodeStack.top().number;;
		visitedCount++;
	}
	}*/
	short wall = 1;
	short tile = 0;
	short wallPos = tileSize.width;
	int pos = 0;
	COORD mousePos{ pos, 0 };
	for (int j = 0; j < numberOfRows; j++) {
		mousePos.Y = j;
		tile = 0;
		wall = 0;
		for (int i = 0; wall < numberOfColumns; wall++, tile++) {
			pos = tile * tileSize.width + wall;
			mousePos.X = pos;
			SetConsoleCursorPosition(handleOutput, mousePos);
			cout << wall + 1 << '_';
			mousePos.X = pos + wallPos;
			SetConsoleCursorPosition(handleOutput, mousePos);
			cout << '|';
		}
	}
}
