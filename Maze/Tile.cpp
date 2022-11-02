
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
int lastRandomNumber = 0, sameRandomCount = 0;

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
void Tile::createNodes(int size, int numOfCols) {
	nodes.resize(size);
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i].number = i;
		setHouseMetaInfo(numOfCols, nodes[i]);
	}
}

int Tile::randomNumber(int min, int max) {
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(min, max);
	return distr(eng);

}

void Tile::setHouseMetaInfo(int numOfCols, Node& house) {
	house.houseRow = house.number / numOfCols;
	house.rowStartNum = house.houseRow * (numOfCols);
	house.houseCol = house.number - house.rowStartNum;
	house.startPos.X = house.houseCol * tileSize.width + house.houseCol;
	house.startPos.Y = house.houseRow * tileSize.height + house.houseRow;
	house.wallRightPos.X = house.startPos.X + tileSize.width;
	house.wallRightPos.Y = house.startPos.Y;
	house.wallUpPos.X = house.startPos.X;
	house.wallUpPos.Y = house.startPos.Y - 1;

	//left neighbour 
	if (house.houseCol > 0) {
		house.neighbourLeft = house.number - 1;
	}
	else {
		house.neighbourLeft = NULL;
	}

	//right neighbour
	if (house.houseCol < numOfCols - 1) {
		house.neighbourRight = house.number + 1;
	}
	else {
		house.neighbourRight = NULL;
	}

	//if node not in 0th row then find upper neighbour
	if (house.houseRow > 0) {
		house.neighbourUp = ((house.houseRow - 1) * numOfCols) + house.houseCol;
	}
	else {
		house.neighbourUp = NULL;
	}

	//if node not in last row then find below neighbour
	if (house.houseRow < (numOfCols - 1)) {
		house.neighbourBelow = ((house.houseRow + 1) * numOfCols) + house.houseCol;
	}
	else {
		house.neighbourBelow = NULL;
	}
}

int Tile::chooseNeighbour(Node& house, short maxMazeRows, short maxMazeCols) {
	vector<int> neighbours;
	int neighbourNumber;
	//cout << "\ncol: " << houseCol << " rowStart" << rowStartNum;
	//left neighbour 
	if (house.houseCol > 0) {
		neighbourNumber = house.neighbourLeft;
		if (!nodes[neighbourNumber].visited) {
			neighbours.push_back(neighbourNumber);
		}
	}
	//right neighbour
	if (house.houseCol < maxMazeCols - 1) {
		neighbourNumber = house.neighbourRight;
		if (!nodes[neighbourNumber].visited) {
			neighbours.push_back(house.neighbourRight);
		}
	}
	//if node not in 0th row then find upper neighbour
	if (house.houseRow > 0) {
		int upperNeighbour = house.neighbourUp;
		if (!nodes[upperNeighbour].visited) {
			neighbours.push_back(upperNeighbour); 
		}
	}
	//if node not in last row then find below neighbour
	if (house.houseRow < (maxMazeRows - 1)) {
		int belowNeighbour = house.neighbourBelow;
		if (!nodes[belowNeighbour].visited) {
			neighbours.push_back(belowNeighbour);
		}
	}
	if (neighbours.size() > 0) {
		int randomValue = randomNumber(0, neighbours.size() - 1);
		while (sameRandomCount > 6) {
			randomValue = randomNumber(0, neighbours.size() - 1);
			if (randomValue == lastRandomNumber) {
				sameRandomCount++;
			}
			else {
				sameRandomCount = 0;
				lastRandomNumber = randomValue;
			}
		}
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
			cout << count;
			mousePos.X = pos + wallPos;
			SetConsoleCursorPosition(outputHandle, mousePos);
			cout << '|';
		}
	}
}
void Tile::removeNumberFromNumberedMaze(int numOfRows, int numOfCols, Node& house, char ch) {

	GetConsoleScreenBufferInfo(outputHandle, &csbInfo);
	COORD mouseErasePos{ 0, 0 }, mousePrevPos{0, 0};
	mousePrevPos = csbInfo.dwCursorPosition;

	mouseErasePos.X = house.startPos.X;
	mouseErasePos.Y = house.startPos.Y;
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

void Tile::buildWalls(int numOfCols) {
	int waitTime = 15;
	COORD wallPosRight, wallPosUp;
	for (int i = 0; i < nodes.size(); i++) {
		bool hasConnectionRight = false, hasConnectionUp = false;
		//find if right or upper neighbour is connected
		for (int j = 0; j < nodes[i].connectingNodeNumber.size(); j++) {
			if (nodes[i].connectingNodeNumber[j] == nodes[i].neighbourRight) {
				hasConnectionRight = true;
			}
			if (nodes[i].houseRow != 0) {
				if (nodes[i].connectingNodeNumber[j] == nodes[i].neighbourUp) {
					hasConnectionUp = true;
				}
			}
		}
		//build right wall
		if (!hasConnectionRight || nodes[i].houseCol == (numOfCols - 1)) {
			wallPosRight = nodes[i].wallRightPos;
			if (nodes[i].houseRow != 0) {
				wallPosRight.Y--;
			}
			int b = 0;
			if (nodes[i].neighbourBelow != NULL) {
				b = -1;
			}
			else {
				b = 0;
			}
			for (int k = b; k <= tileSize.height; k++) {
				SetConsoleCursorPosition(outputHandle, wallPosRight);
				cout << '|';
				wait((milliseconds)waitTime);
				wallPosRight.Y++;
			}
		}
		//build upper wall
		if (!hasConnectionUp && nodes[i].houseRow != 0) {
			wallPosUp = nodes[i].wallUpPos;
			int b;
			//if it doesn't have connection right then we must have already built a wall so
			//don't build wall on that area again
			if (!hasConnectionRight) {
				b = 1;
			}
			else {
				b = 0;
			}
			for (int k = b; k <= tileSize.width; k++) {
				SetConsoleCursorPosition(outputHandle, wallPosUp);
				cout << '-';
				wait((milliseconds)waitTime);
				wallPosUp.X++;
			}
		}
		if (nodes[nodes.size() - 1].houseRow == nodes[i].houseRow) {
			wallPosUp = nodes[i].startPos;
			wallPosUp.Y += tileSize.height;
			for (int k = 0; k <= tileSize.width; k++) {
				SetConsoleCursorPosition(outputHandle, wallPosUp);
				cout << '-';
				wait((milliseconds)waitTime);
				wallPosUp.X++;
			}
		}

	}

	COORD endCursorPos = nodes[nodes.size() - 1].startPos;
	endCursorPos.Y += tileSize.height;
	SetConsoleCursorPosition(outputHandle, endCursorPos);
	cout << "\n";
}
/// <summary>
/// builds maze with specified number of columns and rows
/// </summary>
/// <param name="numberOfColumns"></param>
void Tile::buildMaze(short numberOfColumns, short numberOfRows, HANDLE &handleOutput) {
	nodes.clear();
	nodes.resize(numberOfColumns * numberOfRows);
	outputHandle = handleOutput;
	int totBlockTiles = numberOfColumns * numberOfRows;
	int visitedCount = 0;
	stack<Node> visitedNodeStack;
	createNodes(totBlockTiles, numberOfColumns);
	int house = 0;
	int neighbour = 1;

	//numberedMaze(numberOfRows, numberOfColumns);
	cout << "\n";
	visitedNodeStack.push(nodes[house]);
	visitedNodeStack.top().visited = true;
	visitedCount++;
	removeNumberFromNumberedMaze(numberOfRows, numberOfColumns, nodes[house], '*');
	//connect nodes and build walls around each node where nodes are not connected
	while (visitedCount < (totBlockTiles)) {
		neighbour = chooseNeighbour(nodes[house], numberOfRows, numberOfColumns);
		if (neighbour == 0) {
			while (neighbour == 0 && visitedCount < totBlockTiles) {
				visitedNodeStack.pop();
				if (visitedNodeStack.empty()) {
					cout << "\nStack is empty. Nodes visited = " << visitedCount;
					break;
				}
				else {
					house = visitedNodeStack.top().number;
					neighbour = chooseNeighbour(nodes[house], numberOfRows, numberOfColumns);
				}
			}
		}
		if (visitedCount != totBlockTiles) {
			if (!nodes[neighbour].visited) {
				nodes[neighbour].visited = true;
				nodes[neighbour].connectingNodeNumber.push_back(house);
				nodes[house].connectingNodeNumber.push_back(neighbour);
				visitedNodeStack.push(nodes[neighbour]);
				visitedCount++;
				int direction = house - neighbour;
				char ch = ' ';
				////right
				//if (direction == -1) {
				//	ch = 'L';
				//}
				////Left
				//else if (direction == 1) {
				//	ch = 'R';
				//}
				////down
				//else if (direction < -1) {
				//	ch = 'U';
				//}
				////up
				//else if (direction > 1) {
				//	ch = 'D';
				//}
				//else {
				//	ch = '%';
				//}
				house = neighbour;
				//cout << "\n" << visitedCount << ".node Nmber : " << visitedNodeStack.top().number;
				removeNumberFromNumberedMaze(numberOfRows, numberOfColumns, nodes[house], ch);
				wait((milliseconds)15);
			}
		}
	}
	buildWalls(numberOfColumns);
}
