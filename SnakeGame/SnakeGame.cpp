#include <iostream>
#include <conio.h>
#include <ctime>
#include <thread>
#include <chrono>
#include <string>
#include <cstdlib>
#include <vector>


using namespace std;

class Game {
public:
	bool isNotFailed=true;
	int rate=0;
};

class PlayGround
{
private:
	char grid[11][12] =
	{
		"[=========]",
		"[         ]",
		"[         ]",
		"[    *    ]",
		"[         ]",
		"[    ~    ]",
		"[         ]",
		"[         ]",
		"[         ]",
		"[         ]",
		"[=========]",
	};
public:
	void getGrid(int size)
	{
		for (int i = 0; i < size;i++)
		{
			cout << grid[i] << endl;
		}
	}
	void setSimbol(char charToSet, int cordX, int cordY)
	{
		grid[cordY][cordX] = charToSet;
	}
	void removeSimbol(int cordX, int cordY)
	{
		grid[cordY][cordX] = ' ';
	}
	char checkSymbol(int cordX, int cordY) {
		return grid[cordY][cordX];
	}
	void generateNewCherry()
	{
		int minX = 1, maxX = 9;
		int	minY = 1, maxY = 9;
		bool IsCorrectCords = false;
		while (IsCorrectCords == false)
		{
			int randX = rand() % (maxX - minX + 1) + minX;
			int randY = rand() % (maxY - minY + 1) + minY;

			if (grid[randY][randX] == ' ')
			{
				IsCorrectCords = true;
				grid[randY][randX] = '*';
			}
		}
	}
};

enum MoveSide
{
	Up,
	Down,
	Left,
	Right,
};
class SnakeBodyPart
{
public:
	int xCord;
	int yCord;
	SnakeBodyPart(int x, int y)
	{
		xCord = x;
		yCord = y;
	}
};
class Snake:public SnakeBodyPart
{
private:
	char snakeSign = '~';
	PlayGround &grid;
	Game &game;
	vector<SnakeBodyPart> Bodys;
	MoveSide sideToMove=Up;

public:
	Snake(int cordX, int cordY, PlayGround &grid, Game &game) : game(game),grid(grid),SnakeBodyPart(cordX, cordY)
	{
		this->grid = grid;
		this->game = game;
		Bodys.push_back(SnakeBodyPart(xCord,yCord));
	}

	void CreateSnakePart()
	{
		SnakeBodyPart tail = Bodys.back();
		Bodys.push_back(SnakeBodyPart(tail.xCord, tail.yCord));
	}
	void ControlSnake()
	{
		MoveSide prevSide = sideToMove;
		if(_kbhit())
		{
			char charToMove = _getch();
			switch (charToMove)
			{
				case 'w':
				{
					if (prevSide != Down)
					{
						sideToMove = Up;
					}
					break;
				}
				case 's':
				{
					if (prevSide != Up)
					{
						sideToMove = Down;
					}
					break;
				}
				case 'a':
				{
					if (prevSide != Right)
					{
						sideToMove = Left;
					}
					break;
				}
				case 'd':
				{
					if(prevSide!=Left)
					{
						sideToMove = Right;
					}
					break;
				}
			}
		}
	}

	void MoveSnake()
	{
		int prevX = Bodys[0].xCord;
		int prevY = Bodys[0].yCord;

		int newX = prevX;
		int newY = prevY;

		switch (sideToMove)
		{
			case Up:
			{
				newY--;
				break;
			}
			case Down:
			{
				newY++;
				break;
			}
			case Left:
				newX--;
				break;
			case Right:
			{
				newX++;
				break;
			}
		}

		char symbol = grid.checkSymbol(newX, newY);

		if (symbol == snakeSign)
		{
			game.isNotFailed = false;
			return;
		}

		bool grow = false;

		if (symbol == '*')
		{
			grow = true;
		}


		if (symbol == '=' || symbol == '[' || symbol == ']')
		{
			if (sideToMove == Up)
			{
				newY = 9;
			}
			if (sideToMove == Down)
			{
				newY = 1;
			}
			if (sideToMove == Left)
			{
				newX = 9;
			}
			if (sideToMove == Right)
			{
				newX = 1;
			}
		}


		Bodys[0].xCord = newX;
		Bodys[0].yCord = newY;

		for (int i = 1; i < Bodys.size(); i++)
		{
			int tx = Bodys[i].xCord;
			int ty = Bodys[i].yCord;

			Bodys[i].xCord = prevX;
			Bodys[i].yCord = prevY;

			prevX = tx;
			prevY = ty;
		}
		if (grow)
		{
			game.rate++;
			Bodys.push_back(SnakeBodyPart(prevX, prevY));
			grid.generateNewCherry();
		}
		for (auto& part : Bodys)
		{
			grid.removeSimbol(prevX, prevY);
		}

		for (auto& part : Bodys)
		{
			grid.setSimbol(snakeSign, part.xCord, part.yCord);
		}
	}
};


int main()
{
	srand(time(nullptr));
	setlocale(LC_ALL, "RU");

	Game game;
	PlayGround grid;
	Snake snake(5, 5, grid,game);
	
	auto lastMoveTime = chrono::steady_clock::now();

	while (game.isNotFailed==true)
	{
		snake.ControlSnake();

		auto currentTime = chrono::steady_clock::now();

		if (duration_cast<chrono::milliseconds>(currentTime - lastMoveTime).count() >= 300)
		{
			snake.MoveSnake();

			system("cls");
			grid.getGrid(11);

			lastMoveTime = currentTime;
		}

		this_thread::sleep_for(chrono::milliseconds(1));
	}
	cout << "[=========]\n[         ]\n[         ]\n[         ]\n[ game    ]\n[  over   ]\n[         ]\n[         ]\n[         ]\n[         ]\n[=========]\n" << "rate: " << game.rate << endl;
	string holder;
	cin >> holder;
	return 0;
}