/*******************************************
* Program Name: Virtual Micro Mouse
* Created By: Timothy Mulvey
* Purpose: Simulate a MicroMouse maze and mouse to test algorithms
* Version: 0.2.1
* Date: 01/06/2017
*******************************************/

#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include <stack>
using namespace std;

/*******************************************
* Class Name: Edge
* Purpose: Represent a connection between Cells
*******************************************/
class Edge {
	int sourceX, sourceY;
	int destX, destY;
	int dist;
	bool wall, mazeEdge;
public:
	void randSetup(int x, int y, char dir) {
		sourceX = x; sourceY = y;
		wall = true;
		if ((x == 7 || x == 8) && (y == 7 || y == 8)) {
			if (y == 7 && dir == 's') wall = false;
			if (y == 8 && dir == 'n') wall = false;
			if (x == 7 && dir == 'e') wall = false;
			if (x == 8 && dir == 'w') wall = false;
		}
		if (dir == 'n' || dir == 's') {
			destX = x;
			if ((y == 0 && dir == 'n') || (y == 15 && dir == 's')) {
				destY = y;
				dist = 0;
				mazeEdge = true;
			}
			else {
				dist = (rand() % 15) + 1;
				mazeEdge = false;
				if (dir == 'n')
				destY = y - 1;
				else destY = y + 1;
			}
		}
		else if (dir == 'e' || dir == 'w') {
			destY = y;
			if ((x == 0 && dir == 'w') || (x = 15 && dir == 'e')) {
				destX = x;
				dist = 0;
				mazeEdge = true;
			}
			else {
				dist = (rand() % 15) + 1;
				mazeEdge = false;
				if (dir == 'w')
					destX = x - 1;
				else destX = x + 1;
			}
		}
		else {
			cout << "Error: Invalid direction in Edge.randSetup\n";
			system("PAUSE");
		}
	}
};

/*******************************************
* Class Name: Cell
* Purpose: Represent a cell in a maze.
*******************************************/
class Cell {
	int x, y;
	Edge north, east, south, west;
public:
	int dist;
	bool top, bottom;
	bool left, right;
	bool mouse;
	bool explored;
	//Constructor
	Cell() : top(false), left(false), right(false), bottom(false), mouse(false), explored(false) {};

	void randSetup(int xVal, int yVal) {
		explored = false;
		mouse = false;
		x = xVal; y = yVal;
		north.randSetup(x, y, 'n');
		east.randSetup(x, y, 'e');
		south.randSetup(x, y, 's');
		west.randSetup(x, y, 'w');
		top = true; bottom = true;
		left = true; right = true;
		if ((x == 7 || x == 8) && (y == 7 || y == 8)) {
			dist = 0;
			if (x == 7) right = false;
			if (x == 8) left = false;
			if (y == 7) bottom = false;
			if (y == 8) top = false;
		}
		else {
			dist = (rand() % 15) + 1;
		}
		
	}

	//Sets walls on cell
	void set(bool t, bool l, bool r, bool b) {
		top = t;
		left = l;
		right = r;
		bottom = b;
	}

	//Sets if the mouse is in the cell or not
	void mSet(bool m) {
		mouse = m;
	}

	//Sets cell values from another cell
	void set(Cell c) {
		top = c.top; left = c.left; right = c.right; bottom = c.bottom; mouse = c.mouse;
	}

	//sets coordinates of cell in a maze
	void setCord(int x1, int y1) {
		x = x1;
		y = y1;
		dist = 0;  //default to zero for maze drawing
	}

	//sets coordinates of cell and calculates distance to goal
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
* Class Name: Move
* Purpose: Represent a move in the fringe of the explored area.
*******************************************/
class Move {
	int toGoal;
public:
	char dir;
	int sourceX, sourceY;
	int destX, destY;
	int fromStart;

	void set(char d, int x1, int y1, int g, int s, int x2, int y2) {
		dir = d;
		sourceX = x1; sourceY = y1;
		toGoal = g;
		fromStart = s;
		destX = x2; destY = y2;
	}
	int cost() {
		return toGoal;// + fromStart;
	}

	void draw() {
		cout << dec << "|| Source: (" << sourceX << ", " << sourceY << ") Dir: " << dir
			<< " Dest: (" << destX << ", " << destY
			<< ") Cost: " <</* toGoal << " + " << fromStart << " = " <<*/ cost() << " ||\n";
	}
};

/*******************************************
* Class Name: Fringe
* Purpose: Represents the fringe of the explored area. (List of possible moves)
*******************************************/
class Fringe {
	deque<Move> moves;
	bool showOutput;
public:
	Fringe() : showOutput(true) {};
	Fringe(bool show) : showOutput(show) {};
	bool empty() {
		return moves.empty();
	}
	void clearAll() {
		while (!moves.empty()) {
			moves.pop_front();
		}
	}
	deque<Move> getAll() {
		return moves;
	}
	void add(deque<Move> que) {
		if (!que.empty() && showOutput) {
			cout << "Adding moves:\n";
		}
		while (!que.empty()) {
			moves.push_front(que.front());
			if (showOutput) {
				que.front().draw();
			}
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
		if (showOutput) {
			cout << "Move selected:\n";
			move.draw();
		}
		return move;
	}
	Move getBig() {
		if (moves.empty()) {
			cout << "Fringe.getBig(): ERROR: No moves available! Exiting\n";
			system("PAUSE");
			return Move();
		}
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
		if (showOutput) {
			cout << "Move selected:\n";
			move.draw();
		}
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
* Class Name: Maze
* Purpose: Represent a Micro Mouse maze.
*******************************************/
class Maze {
	Cell maze[16][16];
public:
	/*******************************************
	* Fuction Name: randomMaze()
	* Parameters: bool, turns output on or off
	* Return Value: None
	* Purpose: Generate a random maze. Version 1.0
	*******************************************/
	void randomMaze(bool b) {
		bool showOutput = b;
		//turn on all walls
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				maze[x][y].randSetup(x, y);
			}
		}
		
		//stops to show maze setup if output is on
		if (showOutput) {
			draw();
			system("PAUSE");
		}
		
		//create x and y to track location in maze
		int x = 0; int y = 0;
		//Create fringe and set if output is off or on
		Fringe fringe = showOutput;
		
		//Loop until center of the maze is reached
		while (!((x == 7 || x == 8) && (y == 7 || y == 8))) {
			//Mark maze cell as explored
			maze[x][y].explored = true;

			//Add any moves to adjacent cells, that have not been explored, to fringe 
			Move move;
			if (x > 0 && !maze[x - 1][y].explored) {
				move.set('w', x, y, maze[x - 1][y].dist, 0, x - 1, y);
				fringe.add(move);
			}
			if (x < 15 && !maze[x + 1][y].explored) {
				move.set('e', x, y, maze[x + 1][y].dist, 0, x + 1, y);
				fringe.add(move);
			}
			if (y > 0 && !maze[x][y - 1].explored) {
				move.set('n', x, y, maze[x][y - 1].dist, 0, x, y - 1);
				fringe.add(move);
			}
			if (y < 15 && !maze[x][y + 1].explored) {
				move.set('s', x, y, maze[x][y + 1].dist, 0, x, y + 1);
				fringe.add(move);
			}

			//Check if fringe is empty. Reduntant, this should never happen.
			if (fringe.empty()) {
				cout << "Maze.randomMaze(): ERROR: No moves available. Exiting!\n";
				system("PAUSE");
				return;
			}

			//Pick the next move
			move = fringe.getBig();
			while (maze[move.destX][move.destY].explored) {
				move = fringe.getBig();
			}

			//Take down the wall for the move
			if (move.dir == 'n') {
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

			//Update location
			x = move.destX; y = move.destY;

			//Show updated maze and pause if output is on
			if (showOutput) {
				draw();
				system("PAUSE");
				system("cls");
			}
		}
	}

	/*******************************************
	* Fuction Name: Maze()
	* Parameters: Bool
	* Return Value: None
	* Purpose: Generate an empty maze. Used for mouse memory.
	*******************************************/
	void blankMaze() {
		maze[0][0].mSet(true);
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				maze[i][j].setCord(i, j, true);
			}
		}
	}

	//draw Maze
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

	//return cell from position (x,y)
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
* Class Name: Mouse
* Purpose: Representa the mouse in the maze.
*******************************************/
class Mouse {
	Maze maze;			//mouse memory of maze
	Fringe fringe;		//possible moves
	int steps;			//counter for steps
	int x, y;			//coordinates of mouse
	char dir;			// Direction mouse is facing n, e, s, w
	Cell curCell;		//current location

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

	//Constructor
	Mouse() : x(0), y(0), dir('e'), steps(0) {
	}

	void go(Maze input) {
		maze.blankMaze();
		fringe.clearAll();
		char cont;
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
			//end program when center of the maze is reached
			if ((x == 7 || x == 8) && (y == 7 || y == 8)) {
				cout << "Goal reached!!\n";
				system("PAUSE");
				return;
			}
			system("PAUSE");
		}
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
			system("PAUSE");
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
	//Seed rand() for random numbers
	srand((unsigned)time(0));
	char cont;
	bool showOutput = false;
	do {
		Mouse mouse;
		maze.randomMaze(showOutput);
		mouse.go(maze);
		system("cls");
		cout << "Would you like to play again? (Y/N): ";
		cin >> cont;
		system("cls");
	} while (cont == 'Y' || cont == 'y');

	return 0;
}

/*******************************************
* Fuction Name: MoveSimple()
* Parameters: None
* Return Value: None
* Purpose: Used to move the mouse when there is only one path.
*******************************************/
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
