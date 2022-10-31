
#include "Tile.h"

//ToDO:
// 1. buildMaze function
// 
//
//variables
Tile::TileSize tileSize;
vector<Tile::Node> nodes;
CONSOLE_SCREEN_BUFFER_INFO csbInfo;
HANDLE outputHandle;

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
	int neighbourNumber;
	//cout << "\ncol: " << houseCol << " rowStart" << rowStartNum;
	//left neighbour 
	if (houseCol > 0) {
		neighbourNumber = houseNumber - 1;
		if (!nodes[neighbourNumber].visited) {
			neighbours.push_back(neighbourNumber);
		}
	}
	//right neighbour
	if (houseCol < maxMazeCols - 1) {
		neighbourNumber = houseNumber + 1;
		if (!nodes[neighbourNumber].visited) {
			neighbours.push_back(houseNumber + 1);
		}
	}
	//if node not in 0th row then find upper neighbour
	if (houseRow > 0) {
		int upperNeighbour = ((houseRow - 1) * maxMazeCols) + houseCol;
		if (!nodes[upperNeighbour].visited) {
			neighbours.push_back(upperNeighbour); 
		}
	}
	//if node not in last row then find below neighbour
	if (houseRow < (maxMazeRows - 1)) {
		int belowNeighbour = ((houseRow + 1) * maxMazeCols) + houseCol;
		if (!nodes[belowNeighbour].visited) {
			neighbours.push_back(belowNeighbour);
		}
	}
	if (neighbours.size() > 0) {
		int randomValue = randomNumber(0, neighbours.size() - 1);
		//cout << "\nNodes size: " << neighbours.size();
		return neighbours[randomValue];
	}
	return 0;

}

void Tile::numberedMaze(int numOfRows, int numOfCols) {
	short wallX = 1, wallY = 0;
	short tile = 0;
	short wallPos = tileSize.width;
	int pos = 0, count = 0;
	COORD mousePos{ pos, 0 };
	for (int j = 0; j < numOfRows; j++, wallY++) {
		mousePos.Y = j * tileSize.height + wallY;
		tile = 0;
		wallX = 0;
		for (int i = 0; wallX < numOfCols; wallX++, tile++, count++) {
			pos = tile * tileSize.width + wallX;
			mousePos.X = pos;
			SetConsoleCursorPosition(outputHandle, mousePos);
			cout << count << '_';
			mousePos.X = pos + wallPos;
			SetConsoleCursorPosition(outputHandle, mousePos);
			cout << '|';
		}
	}
}

void Tile::removeNumberFromNumberedMaze(int numOfRows, int numOfCols, int numberToRemove, char ch) {
	int houseRow = (int)(numberToRemove / numOfCols);
	int rowStartNum = houseRow * (numOfCols);
	int houseCol = numberToRemove - rowStartNum;

	GetConsoleScreenBufferInfo(outputHandle, &csbInfo);
	COORD mouseErasePos{ 0, 0 }, mousePrevPos{0, 0};
	mousePrevPos = csbInfo.dwCursorPosition;

	mouseErasePos.X = houseCol * tileSize.width + houseCol;
	mouseErasePos.Y = houseRow * tileSize.height + houseRow;
	SetConsoleCursorPosition(outputHandle, mouseErasePos);
	for (int i = 0; i < tileSize.width; i++) {
		if (i == tileSize.width / 2) {
			cout << ch;
			continue;
		}
		cout << " ";
	}
	SetConsoleCursorPosition(outputHandle, mousePrevPos);
}

void Tile::wait(milliseconds milliSecondsToWait) {
	GetConsoleScreenBufferInfo(outputHandle, &csbInfo);
	COORD cursorPos{csbInfo.dwCursorPosition};

	milliseconds currentTime;
	chrono::milliseconds recordedTime = currentTime = duration_cast<milliseconds>(
		system_clock::now().time_since_epoch()
		);
	milliseconds timePassed = currentTime - recordedTime;
	//if time passed more than the specified time then return
	while (timePassed <= milliSecondsToWait) {
		 currentTime = duration_cast<milliseconds>(
			 system_clock::now().time_since_epoch()
			 );
		 timePassed = currentTime - recordedTime;
		 /*cout << timePassed;
		 SetConsoleCursorPosition(outputHandle, cursorPos);*/
	}
}
/// <summary>
/// builds maze with specified number of columns and rows
/// </summary>
/// <param name="numberOfColumns"></param>
void Tile::buildMaze(short numberOfColumns, short numberOfRows, HANDLE &handleOutput) {
	outputHandle = handleOutput;
	int totBlockTiles = numberOfColumns * numberOfRows;
	int visitedCount = 0;
	stack<Node> visitedNodeStack;
	createNodes(totBlockTiles);
	int house = 0;
	int neighbour = 1;

	numberedMaze(numberOfRows, numberOfColumns);
	cout << "\n";
	visitedNodeStack.push(nodes[house]);
	visitedNodeStack.top().visited = true;
	visitedCount++;
	removeNumberFromNumberedMaze(numberOfRows, numberOfColumns, house, '*');
	//connect nodes and build walls around each node where nodes are not connected
	while (visitedCount < (totBlockTiles)) {
		neighbour = chooseNeighbour(house, numberOfRows, numberOfColumns);
		if (neighbour == 0) {
			while (neighbour == 0 && visitedCount < totBlockTiles) {
				visitedNodeStack.pop();
				if (visitedNodeStack.empty()) {
					cout << "\nStack is empty. Nodes visited = " << visitedCount;
					break;
				}
				else {
					house = visitedNodeStack.top().number;
					neighbour = chooseNeighbour(house, numberOfRows, numberOfColumns);
				}
			}
		}
		if (visitedCount != totBlockTiles) {
			if (!nodes[neighbour].visited) {
				nodes[neighbour].visited = true;
				visitedNodeStack.push(nodes[neighbour]);
				visitedCount++;
				int direction = house - neighbour;
				char ch = '%';
				if (direction == -1) {
					ch = '>';
				}
				else if (direction == 1) {
					ch = '<';
				}
				else if (direction < -1) {
					ch = 'v';
				}
				else if (direction > 1) {
					ch = '^';
				}
				else {
					ch = '%';
				}

				house = neighbour;
				//cout << "\n" << visitedCount << ".node Nmber : " << visitedNodeStack.top().number;
				removeNumberFromNumberedMaze(numberOfRows, numberOfColumns, house, ch);
				wait((milliseconds)500);
			}
		}
	}

	//while (cin && neighbour != 0) {
	//	neighbour = chooseNeighbour(house, numberOfRows, numberOfColumns);
	//	if (neighbour > 0) {
	//		nodes[neighbour].visited = true;
	//		visitedNodeStack.push(nodes[neighbour]);
	//		cout << "node Number : " << visitedNodeStack.top().number;
	//	}
	//	//cin >> neighbour;
	//}
}
