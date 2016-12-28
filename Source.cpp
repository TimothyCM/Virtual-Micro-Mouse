/*******************************************
* Program Name: Virtual Micro Mouse
* Created By: Timothy Mulvey
* Purpose: Simulate a Micro Mouse maze and mouse to test algorithms without hardware.
* Version: 0.1
* Date: 12/26/2017
*******************************************/

#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <stack>
using namespace std;

/*******************************************
* Class Name: Cell
* Purpose: Represent a cell in a maze.
*******************************************/
class Cell {
	int x;
	int y;
public:
	int dist;
	bool top;
	bool left;
	bool right;
	bool bottom;
	bool mouse;
	bool explored;
	Cell() {
		top = 0; left = 0; right = 0; bottom = 0; mouse = false; explored = false;
	}
	Cell(bool t, bool l, bool r, bool b) {
		top = t; left = l; right = r; bottom = b; mouse = false; explored = false;
	}
	void set(bool t, bool l, bool r, bool b) {
		top = t;
		left = l;
		right = r;
		bottom = b;
	}
	void mSet(bool m) {
		mouse = m;
	}
	void set(Cell c) {
		top = c.top; left = c.left; right = c.right; bottom = c.bottom; mouse = c.mouse;
	}
	void setCord(int x1, int y1) {
		x = x1;
		y = y1;
		dist = 0;
	}
	void setCord(int x1, int y1, bool b) {
		x = x1;
		y = y1;
		if (x < 8 && y < 8)
			dist = 14 - x - y;
		else if (x > 7 && y < 8)
			dist = 7 - y + x - 8;
		else if (x < 8)
			dist = 7 - x + y - 8;
		else
			dist = x - 8 + y - 8;
	}
};

/*******************************************
* Class Name: Maze
* Purpose: Represent a Micro Mouse maze.
*******************************************/
class Maze {
	Cell maze[16][16];
public:
	Maze();
	Maze(bool);
	void draw() {
		//Draw top edge
		cout << "+";
		for (int x = 0; x < 16; x++) {
			if (maze[x][0].top == true) cout << "---+"; else cout << "   +";
		}
		cout << "\n";

		for (int y = 0; y < 16; y++) {
			//draw left edge (center)
			if (maze[0][y].left == true) cout << "|"; else cout << " ";
			//loop to draw horizontal lines
			for (int x = 0; x < 16; x++) {
				//check for mouse and draw it
				if (maze[x][y].mouse == true) cout << " m ";
				//check for empty cell and draw it
				else if (maze[x][y].explored == true) cout << "   ";
				//show distance to goal
				else if (maze[x][y].dist > 0) cout << " " << hex << maze[x][y].dist << " ";
				//default to empty cell
				else cout << "   ";
				//draw right edge
				if (x < 15) {
					if (maze[x][y].right == true || maze[x + 1][y].left == true) cout << "|"; else cout << " ";
				}
				else {
					if (maze[x][y].right == true) cout << "|"; else cout << " ";
				}
			}
			cout << "\n";
			cout << "+";
			//draw bottom edge of row
			for (int x = 0; x < 16; x++) {
				if (y < 15) {
					if (maze[x][y].bottom == true || maze[x][y + 1].top == true) cout << "---+"; else cout << "   +";
				}
				else {
					if (maze[x][y].bottom == true) cout << "---+"; else cout << "   +";
				}
			}
			cout << "\n";
		}

	}
	Cell getCell(int x, int y) {
		return maze[x][y];
	}
	void setCell(int x, int y, Cell c) {
		maze[x][y].set(c);
	}
	void moveMouse(int x1, int y1, int x2, int y2) {
		maze[x1][y1].mSet(false);
		maze[x2][y2].mSet(true);
	}
	void explore(int x, int y) {
		maze[x][y].explored = true;
	}
	bool explored(int x, int y) {
		return maze[x][y].explored;
	}
	int getDist(int x, int y) {
		return maze[x][y].dist;
	}
};

/*******************************************
* Class Name: Move
* Purpose: Represent a move in the fringe of the explored area.
*******************************************/
class Move {
	int toGoal;
public:
	char dir;
	int sourceX;
	int sourceY;
	int destX;
	int destY;
	int fromStart;

	void set(char d, int x1, int y1, int g, int s, int x2, int y2) {
		dir = d;
		sourceX = x1;
		sourceY = y1;
		toGoal = g;
		fromStart = s;
		destX = x2;
		destY = y2;
	}
	int cost() {
		return toGoal + fromStart;
	}

	void draw() {
		cout << dec << "|| Source: (" << sourceX << ", " << sourceY << ") Dir: " << dir
			<< " Dest: (" << destX << ", " << destY
			<< ") Cost: " << toGoal << " + " << fromStart << " = " << cost() << " ||\n";
	}
};

/*******************************************
* Class Name: Move
* Purpose: Represents the fringe of the explored area. (List of possible moves)
*******************************************/
class Fringe {
	deque<Move> moves;
public:
	bool empty() {
		return moves.empty();
	}
	deque<Move> getAll() {
		return moves;
	}
	void add(deque<Move> que) {
		if (!que.empty()) {
			cout << "Adding moves:\n";
		}
		while (!que.empty()) {
			moves.push_front(que.front());
			que.front().draw();
			que.pop_front();
		}
	}
	void add(Move move) {
		moves.push_front(move);
	}
	Move get() {
		Move move = moves.front();
		moves.pop_front();
		if (!moves.empty()) {
			for (int i = 0; i < moves.size(); i++) {
				Move move2 = moves.front();
				moves.pop_front();
				if (move2.cost() < move.cost()) {
					moves.push_back(move);
					move = move2;
				}
				else {
					moves.push_back(move2);
				}
			}
		}
		cout << "Move selected:\n";
		move.draw();
		return move;
	}
	Move getBig() {
		Move move = moves.front();
		moves.pop_front();
		if (!moves.empty()) {
			for (int i = 0; i < moves.size(); i++) {
				Move move2 = moves.front();
				moves.pop_front();
				if (move2.cost() > move.cost()) {
					moves.push_back(move);
					move = move2;
				}
				else {
					moves.push_back(move2);
				}
			}
		}
		cout << "Move selected:\n";
		move.draw();
		return move;
	}
	void draw() {
		for (int i = 0; i < moves.size(); i++) {
			Move move = moves.front();
			moves.pop_front();
			move.draw();
			moves.push_back(move);
		}
	}
};

/*******************************************
* Class Name: Mouse
* Purpose: Representa the mouse in the maze.
*******************************************/
class Mouse {
	Maze maze;
	Fringe fringe;
	int steps;
	// (x,y) coordinates of mouse
	int x;
	int y;
	// Direction mouse is facing n, e, s, w
	char dir;
	//current location
	Cell curCell;

	//private functions
	void sense(Cell cell) {
		if (maze.explored(x, y) == false) {

			curCell.set(cell.top, cell.left, cell.right, cell.bottom);
			maze.setCell(x, y, cell);
			maze.explore(x, y);
			int moves = countMoves();
			if (moves > 1) {
				fringe.add(getMoves());
			}
		}
		else {
			curCell = maze.getCell(x, y);
		}
	}

	bool valid(int x, int y) {
		//Upper Left Section
		if (x < 8 && y < 8) {
			for (int i = y; i >= 0; i--) {
				for (int j = x; j < 8; j++) {
					if (maze.explored(j, i)) {
						break;
					}
					else if (j == 7) {
						return true;
					}
				}
			}
			for (int i = x; i >= 0; i--) {
				for (int j = y; j < 8; j++) {
					if (maze.explored(i, j)) {
						break;
					}
					else if (j == 7) {
						return true;
					}
				}
			}
		}
		//Upper Right Section
		else if (x > 7 && y < 8) {
			return true;
		}
		//Lower Left Section
		else if (x < 8 && y > 7) {
			//Look for path to the right
			for (int i = y; i < 8; i++) {
				for (int j = x; j < 8; j++) {
					if (maze.explored(j, i)) {
						break;
					}
					else if (j == 7) {
						return true;
					}
				}
			}
			//Look for a path up
			for (int i = x; i >= 0; i--) {
				for (int j = y; j >= 0; j--) {
					if (maze.explored(i, j)) {
						break;
					}
					else if (j == 8) {
						return true;
					}
				}
			}
		}
		//Lower Right Section
		else if (x > 7 && y > 7) {
			return true;
		}
		return false;
	}

public:

	Mouse() {
		maze = Maze(true);
		x = 0;
		y = 0;
		dir = 'e';
		steps = 0;
	}

	Mouse(Maze m) {
		maze = m;
		x = 0;
		y = 0;
		dir = 'e';
		steps = 0;
	}

	void go(Maze input) {
		char cont;
		do {
			input.draw();
			cout << "Start? (Y / N) : ";
			cin >> cont;
			while (cont == 'Y' || cont == 'y') {
				system("cls");
				int tempX = x; int tempY = y;
				sense(input.getCell(x, y));
				move();
				input.moveMouse(tempX, tempY, x, y);
				input.draw();
				sense(input.getCell(x, y));
				showMaze();
				cout << "Move again? (Y/N): ";
				cin >> cont;
			}
			system("cls");
			input.draw();
			cout << "Would you like to play again? (Y/N): ";
			cin >> cont;
			system("cls");
		} while (cont == 'Y' || cont == 'y');
	}

	void move() {
		int moves = countMoves();
		if (moves == 1) {
			moveSimple();
		}
		else {
			moveSmart();
		}
	}

	/*******************************************
	* Fuction Name: MoveSimple()
	* Parameters: None
	* Return Value: None
	* Purpose: Used to move the mouse when there is only one path.
	*******************************************/
	void moveSimple();

	void moveSmart() {
		Move move1 = fringe.get();
		while (!valid(move1.destX, move1.destY)) {
			cout << "Discarding move:\n";
			move1.draw();
			move1 = fringe.get();
		}
		if (move1.sourceX == x && move1.sourceY == y) {
			move(move1.dir);
		}
		else {
			move(move1.sourceX, move1.sourceY);
			steps = move1.fromStart;
			move(move1.dir);
		}
	}

	void showMaze() {
		maze.draw();
		cout << dec << "Current steps from start: " << steps << "\nMoves on fringe:\n";
		fringe.draw();
	}

	deque<Move> getMoves() {
		deque<Move> moves;

		if (curCell.top == false && !maze.explored(x, y - 1)) {
			Move move;
			move.set('n', x, y, maze.getDist(x, y - 1), steps, x, y - 1);
			moves.push_front(move);
		}
		if (curCell.left == false && !maze.explored(x - 1, y)) {
			Move move;
			move.set('w', x, y, maze.getDist(x - 1, y), steps, x - 1, y);
			moves.push_front(move);
		}
		if (curCell.right == false && !maze.explored(x + 1, y)) {
			Move move;
			move.set('e', x, y, maze.getDist(x + 1, y), steps, x + 1, y);
			moves.push_front(move);
		}
		if (curCell.bottom == false && !maze.explored(x, y + 1)) {
			Move move;
			move.set('s', x, y, maze.getDist(x, y + 1), steps, x, y + 1);
			moves.push_front(move);
		}

		return moves;
	}

	void move(int x1, int y1) {
		int tempX = x;
		int tempY = y;
		x = x1;
		y = y1;
		maze.moveMouse(tempX, tempY, x, y);
	}

	int countMoves() {
		int moves = 0;
		if (dir == 'n') {
			if (curCell.top == false)
				moves++;
			if (curCell.left == false)
				moves++;
			if (curCell.right == false)
				moves++;
		}
		if (dir == 'e') {
			if (curCell.top == false)
				moves++;
			if (curCell.bottom == false)
				moves++;
			if (curCell.right == false)
				moves++;
		}
		if (dir == 's') {
			if (curCell.bottom == false)
				moves++;
			if (curCell.left == false)
				moves++;
			if (curCell.right == false)
				moves++;
		}
		if (dir == 'w') {
			if (curCell.top == false)
				moves++;
			if (curCell.left == false)
				moves++;
			if (curCell.bottom == false)
				moves++;
		}
		return moves;
	}

	void move(char direction) {
		dir = direction;
		moveForward();
	}

	void turnLeft() {
		if (dir == 'n')
			dir = 'w';
		else if (dir == 'e')
			dir = 'n';
		else if (dir == 's')
			dir = 'e';
		else if (dir == 'w')
			dir = 's';
	}

	void turnRight() {
		if (dir == 'n')
			dir = 'e';
		else if (dir == 'e')
			dir = 's';
		else if (dir == 's')
			dir = 'w';
		else if (dir == 'w')
			dir = 'n';
	}

	void moveForward() {
		int tempX = x;
		int tempY = y;
		if (dir == 'n') {
			y--;
		}
		if (dir == 'e') {
			x++;
		}
		if (dir == 's') {
			y++;
		}
		if (dir == 'w') {
			x--;
		}
		maze.moveMouse(tempX, tempY, x, y);
		steps++;
	}
};

/*******************************************
* Fuction Name: main()
* Parameters: None
* Return Value: int
* Purpose: Create Maze and Mouse objects.  Start mouse AI.
*******************************************/
int main() {
	Maze maze;
	Mouse mouse;

	mouse.go(maze);

	return 0;
}

//See class for info.
void Mouse::moveSimple() {
	if (dir == 'n') {
		if (curCell.top == false)
			moveForward();
		else if (curCell.left == false) {
			turnLeft();
			moveForward();
		}
		else if (curCell.right == false) {
			turnRight();
			moveForward();
		}
	}
	else if (dir == 'e') {
		if (curCell.right == false)
			moveForward();
		else if (curCell.top == false) {
			turnLeft();
			moveForward();
		}
		else if (curCell.bottom == false) {
			turnRight();
			moveForward();
		}
	}
	else if (dir == 's') {
		if (curCell.bottom == false)
			moveForward();
		else if (curCell.right == false) {
			turnLeft();
			moveForward();
		}
		else if (curCell.left == false) {
			turnRight();
			moveForward();
		}
	}
	else if (dir == 'w') {
		if (curCell.left == false)
			moveForward();
		else if (curCell.bottom == false) {
			turnLeft();
			moveForward();
		}
		else if (curCell.top == false) {
			turnRight();
			moveForward();
		}
	}
}

Maze::Maze(bool empty) {
	maze[0][0].mSet(true);
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			maze[i][j].setCord(i, j, true);
		}
	}
}

Maze::Maze() {
	/*maze[0][0].set(true, true, false, false);	//set top left corner
	maze[15][0].set(true, false, true, false);  //set top right corner
	maze[0][15].set(false, true, false, true);	//set bottom left corner
	maze[15][15].set(false, false, true, true);	//set bottom right corner
	for (int i = 1; i < 15; i++) {
		//set wall along the top edge of the maze
		maze[i][0].set(true, false, false, false);
		//set wall along the bottom edge of the maze
		maze[i][15].set(false, false, false, true);
		//set wall along the left edge of the maze
		maze[0][i].set(false, true, false, false);
		//set wall along the right edge of the maze
		maze[15][i].set(false, false, true, false);
	}*/
	//Seed rand() for random numbers
	srand((unsigned)time(0));
	//turn on all walls
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			maze[x][y].set(true, true, true, true);
			maze[x][y].dist = (rand() % 15) + 1;
		}
	}
	//Take down center walls and set distance values to zero
	maze[7][7].dist = 0;
	maze[7][7].set(true, true, false, false);
	maze[8][7].dist = 0;
	maze[8][7].set(true, false, true, false);
	maze[7][8].dist = 0;
	maze[7][8].set(false, true, false, true);
	maze[8][8].dist = 0;
	maze[8][8].set(false, false, true, true);
	//create x and y to track location in maze
	int x = 0; int y = 0;
	//Create fringe
	Fringe fringe;
	//loop until center of the maze is reached
	while (!((x == 7 || x == 8) && (y == 7 || y == 8))) {
		cout << "Current locationg: x = " << dec << x << " y = " << y << "\n";
		maze[x][y].explored = true;
		Fringe tempFringe;
		if (x > 0 && !maze[x-1][y].explored) {
			Move move;
			move.set('w', x, y, maze[x-1][y].dist, 0, x - 1, y);
			tempFringe.add(move);
		}
		if (x < 15 && !maze[x + 1][y].explored) {
			Move move;
			move.set('e', x, y, maze[x+1][y].dist, 0, x + 1, y);
			tempFringe.add(move);
		}
		if (y > 0 && !maze[x][y-1].explored) {
			Move move;
			move.set('n', x, y, maze[x][y-1].dist, 0, x, y - 1);
			tempFringe.add(move);
		}
		if (y < 15 && !maze[x][y+1].explored) {
			Move move;
			move.set('s', x, y, maze[x][y+1].dist, 0, x, y + 1);
			tempFringe.add(move);
		}
		
		int rando = rand() % 10;
		if (rando == 9) {
			fringe.add(tempFringe.getAll());
		}
		else if (rando == 8) {
			for (int i = 0; i < 3; i++) {
				if (!tempFringe.empty()) {
					fringe.add(tempFringe.getBig());
				}
			}
		}
		else if (rando < 8 && rando > 5) {
			for (int i = 0; i < 2; i++) {
				if (!tempFringe.empty()) {
					fringe.add(tempFringe.getBig());
				}
			}
		}
		else if (rando > 0) {
			if (!tempFringe.empty()) {
				fringe.add(tempFringe.getBig());
			}
		}
		else {
			if (fringe.empty()) {
				if (!tempFringe.empty()) {
					fringe.add(tempFringe.getBig());
				}
			}
		}
		while (!tempFringe.empty()) {
			tempFringe.get();
		}
		Move move = fringe.getBig();
		if (move.dir == 'n') {
			cout << "moving north\n";
			maze[move.sourceX][move.sourceY].top = false;
			maze[move.destX][move.destY].bottom = false;
		}
		else if (move.dir == 's') {
			maze[move.sourceX][move.sourceY].bottom = false;
			maze[move.destX][move.destY].top = false;
		}
		else if (move.dir == 'e') {
			maze[move.sourceX][move.sourceY].right = false;
			maze[move.destX][move.destY].left = false;
		}
		else if (move.dir == 'w') {
			maze[move.sourceX][move.sourceY].left = false;
			maze[move.destX][move.destY].right = false;
		}
		
		x = move.destX; y = move.destY;
		//break;
		//char cont;
		//draw();
		//fringe.draw();
		//cout << "Cont? (Y / N) : ";
		//cin >> cont;
		//system("cls");
	}
	char cont;
	system("cls");
	cout << "Setup Done\n";
	draw();
	cout << "Cont? (Y / N) : ";
	cin >> cont;
	system("cls");
}

/*
Maze::Maze() {
	maze[0][0].mSet(true);
	maze[0][0].set(true, true, false, false);
	for (int x = 1; x < 15; x++)
		maze[x][0].set(true, false, false, true);
	maze[3][0].set(true, false, false, false);
	maze[15][0].set(true, false, true, false);

	maze[0][1].set(false, 1, false, false);
	maze[1][1].set(true, false, true, false);
	maze[2][1].set(true, true, true, false);
	maze[3][1].set(false, true, true, false);
	maze[4][1].set(true, true, false, false);
	maze[5][1].set(true, false, false, true);
	maze[6][1].set(true, false, false, true);
	maze[7][1].set(true, false, false, false);
	maze[8][1].set(true, false, false, false);
	maze[9][1].set(true, false, false, true);
	maze[10][1].set(true, false, false, false);
	maze[11][1].set(true, false, true, false);
	maze[12][1].set(true, true, true, false);
	maze[13][1].set(true, true, false, false);
	maze[14][1].set(true, false, true, true);
	maze[15][1].set(false, true, true, false);

	maze[0][2].set(false, true, true, false);
	maze[1][2].set(0, 1, 1, 1);
	maze[2][2].set(0, 1, 1, 0);
	maze[3][2].set(0, 1, 1, 0);
	maze[4][2].set(0, 1, 1, 0);
	maze[5][2].set(1, 1, 0, 0);
	maze[6][2].set(1, 0, 0, 1);
	maze[7][2].set(0, 0, 1, 1);
	maze[8][2].set(0, 1, 1, false);
	maze[9][2].set(true, 1, 1, false);
	maze[10][2].set(0, 1, 1, false);
	maze[11][2].set(0, 1, 1, false);
	maze[12][2].set(true, false, false, false);
	maze[13][2].set(0, false, 1, 1);
	maze[14][2].set(true, 1, 1, false);
	maze[15][2].set(0, 1, 1, false);

	maze[0][3].set(0, 1, 0, 1);
	maze[1][3].set(1, 0, 0, 0);
	maze[2][3].set(0, 0, 1, 0);
	maze[3][3].set(0, 1, 1, 0);
	maze[4][3].set(0, 1, 1, 0);
	maze[5][3].set(0, 1, 1, 1);
	maze[6][3].set(1, 1, 0, 1);
	maze[7][3].set(1, 0, 0, 0);
	maze[8][3].set(0, 0, 1, 1);
	maze[9][3].set(0, 1, 0, 1);
	maze[10][3].set(0, 0, 1, 1);
	maze[11][3].set(0, 1, 0, 1);
	maze[12][3].set(0, 0, 1, 0);
	maze[13][3].set(1, 1, 1, 0);
	maze[14][3].set(0, 1, 1, 0);
	maze[15][3].set(0, 1, 1, 0);

	maze[0][4].set(1, 1, 0, 0);
	maze[1][4].set(0, 0, 1, 1);
	maze[2][4].set(0, 1, 0, 0);
	maze[3][4].set(0, 0, 0, 1);
	maze[4][4].set(0, 0, 1, 0);
	maze[5][4].set(1, 1, 0, 0);
	maze[6][4].set(1, 0, 0, 1);
	maze[7][4].set(0, 0, 0, 1);
	maze[8][4].set(1, 0, 0, 1);
	maze[9][4].set(1, 0, 0, 0);
	maze[10][4].set(1, 0, 0, 1);
	maze[11][4].set(1, 0, 0, 1);
	maze[12][4].set(0, 0, 0, 1);
	maze[13][4].set(0, 0, 0, 1);
	maze[14][4].set(0, 0, 0, 1);
	maze[15][4].set(0, 0, 1, 1);

	maze[0][5].set(0, 1, 0, 1);
	maze[1][5].set(1, 0, 0, 0);
	maze[2][5].set(0, 0, 1, 1);
	maze[3][5].set(1, 1, 1, 0);
	maze[4][5].set(0, 1, 1, 0);
	maze[5][5].set(0, 1, 0, 0);
	maze[6][5].set(1, 0, 0, 1);
	maze[7][5].set(1, 0, 1, 0);
	maze[8][5].set(1, 1, 1, 0);
	maze[9][5].set(0, 1, 0, 0);
	maze[10][5].set(1, 0, 0, 1);
	maze[11][5].set(1, 0, 0, 0);
	maze[12][5].set(1, 0, 1, 1);
	maze[13][5].set(1, 1, 0, 0);
	maze[14][5].set(1, 0, 0, 1);
	maze[15][5].set(1, 0, 1, 0);

	maze[0][6].set(1, 1, 0, 0);
	maze[1][6].set(0, 0, 0, 1);
	maze[2][6].set(1, 0, 1, 0);
	maze[3][6].set(0, 1, 0, 1);
	maze[4][6].set(0, 0, 1, 1);
	maze[5][6].set(0, 1, 1, 0);
	maze[6][6].set(1, 1, 0, 0);
	maze[7][6].set(0, 0, 1, 0);
	maze[8][6].set(0, 1, 0, 1);
	maze[9][6].set(0, 0, 0, 0);
	maze[10][6].set(1, 0, 1, 0);
	maze[11][6].set(0, 1, 0, 1);
	maze[12][6].set(1, 0, 0, 1);
	maze[13][6].set(0, 0, 1, 1);
	maze[14][6].set(1, 1, 1, 0);
	maze[15][6].set(0, 1, 1, 1);

	maze[0][7].set(0, 1, 0, 0);
	maze[1][7].set(1, 0, 1, 0);
	maze[2][7].set(0, 1, 0, 0);
	maze[3][7].set(1, 0, 0, 1);
	maze[4][7].set(1, 0, 1, 0);
	maze[5][7].set(0, 1, 1, 1);
	maze[6][7].set(0, 1, 1, 0);
	maze[7][7].set(0, 1, 0, 0);
	maze[8][7].set(1, 0, 1, 0);
	maze[9][7].set(0, 1, 1, 0);
	maze[10][7].set(0, 1, 0, 1);
	maze[11][7].set(1, 0, 0, 1);
	maze[12][7].set(1, 0, 0, 1);
	maze[13][7].set(1, 0, 0, 0);
	maze[14][7].set(0, 0, 0, 1);
	maze[15][7].set(1, 0, 1, 0);

	maze[0][8].set(0, 1, 1, 0);
	maze[1][8].set(0, 1, 1, 0);
	maze[2][8].set(0, 1, 1, 0);
	maze[3][8].set(1, 1, 1, 0);
	maze[4][8].set(0, 1, 0, 0);
	maze[5][8].set(1, 0, 1, 0);
	maze[6][8].set(0, 1, 1, 0);
	maze[7][8].set(0, 1, 0, 1);
	maze[8][8].set(0, 0, 1, 1);
	maze[9][8].set(0, 1, 1, 0);
	maze[10][8].set(1, 1, 0, 0);
	maze[11][8].set(1, 0, 0, 0);
	maze[12][8].set(1, 0, 1, 0);
	maze[13][8].set(0, 1, 1, 1);
	maze[14][8].set(1, 1, 0, 0);
	maze[15][8].set(0, 0, 1, 0);

	maze[0][9].set(0, 1, 1, 0);
	maze[1][9].set(0, 1, 1, 0);
	maze[2][9].set(0, 1, 0, 0);
	maze[3][9].set(0, 0, 1, 0);
	maze[4][9].set(0, 1, 1, 1);
	maze[5][9].set(0, 1, 1, 1);
	maze[6][9].set(0, 1, 0, 0);
	maze[7][9].set(1, 0, 1, 1);
	maze[8][9].set(1, 1, 0, 0);
	maze[9][9].set(0, 0, 1, 0);
	maze[10][9].set(0, 1, 1, 0);
	maze[11][9].set(0, 1, 1, 1);
	maze[12][9].set(0, 1, 0, 1);
	maze[13][9].set(1, 0, 0, 1);
	maze[14][9].set(0, 0, 1, 0);
	maze[15][9].set(0, 1, 1, 1);

	maze[0][10].set(0, 1, 1, 0);
	maze[1][10].set(0, 1, 1, 0);
	maze[2][10].set(0, 1, 1, 0);
	maze[3][10].set(0, 1, 0, 0);
	maze[4][10].set(1, 0, 0, 1);
	maze[5][10].set(1, 0, 1, 0);
	maze[6][10].set(0, 1, 0, 1);
	maze[7][10].set(1, 0, 0, 0);
	maze[8][10].set(0, 0, 1, 0);
	maze[9][10].set(0, 1, 0, 0);
	maze[10][10].set(0, 0, 1, 1);
	maze[11][10].set(1, 1, 0, 0);
	maze[12][10].set(1, 0, 1, 1);
	maze[13][10].set(1, 1, 1, 0);
	maze[14][10].set(0, 1, 0, 0);
	maze[15][10].set(1, 0, 1, 0);

	maze[0][11].set(0, 1, 1, 0);
	maze[1][11].set(0, 1, 0, 0);
	maze[2][11].set(0, 0, 0, 1);
	maze[3][11].set(0, 0, 1, 0);
	maze[4][11].set(1, 1, 1, 0);
	maze[5][11].set(0, 1, 1, 1);
	maze[6][11].set(1, 1, 1, 0);
	maze[7][11].set(0, 1, 1, 1);
	maze[8][11].set(0, 1, 0, 1);
	maze[9][11].set(0, 0, 1, 1);
	maze[10][11].set(1, 1, 0, 0);
	maze[11][11].set(0, 0, 1, 1);
	maze[12][11].set(1, 1, 0, 0);
	maze[13][11].set(0, 0, 1, 0);
	maze[14][11].set(0, 1, 1, 0);
	maze[15][11].set(0, 1, 1, 1);

	maze[0][12].set(0, 1, 1, 0);
	maze[1][12].set(0, 1, 1, 1);
	maze[2][12].set(1, 1, 1, 0);
	maze[3][12].set(0, 1, 0, 1);
	maze[4][12].set(0, 0, 0, 1);
	maze[5][12].set(1, 0, 0, 0);
	maze[6][12].set(0, 0, 0, 1);
	maze[7][12].set(1, 0, 0, 0);
	maze[8][12].set(1, 0, 1, 1);
	maze[9][12].set(1, 1, 0, 1);
	maze[10][12].set(0, 0, 0, 1);
	maze[11][12].set(1, 0, 0, 0);
	maze[12][12].set(0, 0, 1, 1);
	maze[13][12].set(0, 1, 0, 0);
	maze[14][12].set(0, 0, 1, 0);
	maze[15][12].set(1, 1, 1, 0);

	maze[0][13].set(0, 1, 1, 0);
	maze[1][13].set(1, 1, 0, 0);
	maze[2][13].set(0, 0, 0, 1);
	maze[3][13].set(1, 0, 0, 1);
	maze[4][13].set(1, 0, 0, 0);
	maze[5][13].set(0, 0, 0, 1);
	maze[6][13].set(1, 0, 0, 0);
	maze[7][13].set(0, 0, 1, 0);
	maze[8][13].set(1, 1, 0, 0);
	maze[9][13].set(1, 0, 0, 1);
	maze[10][13].set(1, 0, 0, 0);
	maze[11][13].set(0, 0, 1, 1);
	maze[12][13].set(1, 1, 0, 0);
	maze[13][13].set(0, 0, 1, 1);
	maze[14][13].set(0, 1, 0, 1);
	maze[15][13].set(0, 0, 1, 1);

	maze[0][14].set(0, 1, 1, 0);
	maze[1][14].set(0, 1, 1, 0);
	maze[2][14].set(1, 1, 0, 0);
	maze[3][14].set(1, 0, 1, 0);
	maze[4][14].set(0, 1, 0, 1);
	maze[5][14].set(1, 0, 0, 0);
	maze[6][14].set(0, 0, 1, 0);
	maze[7][14].set(0, 1, 0, 0);
	maze[8][14].set(0, 0, 1, 0);
	maze[9][14].set(1, 1, 0, 0);
	maze[10][14].set(0, 0, 0, 1);
	maze[11][14].set(1, 0, 0, 0);
	maze[12][14].set(0, 0, 1, 1);
	maze[13][14].set(1, 1, 0, 0);
	maze[14][14].set(1, 0, 0, 0);
	maze[15][14].set(1, 0, 1, 0);

	maze[0][15].set(0, 1, 1, 1);
	maze[1][15].set(0, 1, 0, 1);
	maze[2][15].set(0, 0, 1, 1);
	maze[3][15].set(0, 1, 0, 1);
	maze[4][15].set(1, 0, 0, 1);
	maze[5][15].set(0, 0, 1, 1);
	maze[6][15].set(0, 1, 0, 1);
	maze[7][15].set(0, 0, 1, 1);
	maze[8][15].set(0, 1, 0, 1);
	maze[9][15].set(0, 0, 1, 1);
	maze[10][15].set(1, 1, 0, 1);
	maze[11][15].set(0, 0, 0, 1);
	maze[12][15].set(1, 0, 0, 1);
	maze[13][15].set(0, 0, 1, 1);
	maze[14][15].set(0, 1, 1, 1);
	maze[15][15].set(0, 1, 1, 1);

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			maze[i][j].setCord(i, j);
		}
	}
}
*/
