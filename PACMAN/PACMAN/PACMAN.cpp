// PACMAN.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "Windows.h"
#include "conio.h"
#include <cstdlib>
#include <iostream>
#include "level.h"
#include "character.h"

using namespace std;

HANDLE consoleHandle = 0;

char scoresFileName[] = "scores.dat";
char saveFileName[] = "save.dat";
FILE *scoresFile;
FILE *saveFile;

int plusTime = 0;
int gameStatus = 1; //1 - active, 2 - level end, 3 - exit
int activeLevel = 1, levels = 10 , points = 0, gametime = 0, takedPoints = 0, lives = 3;

int ghostsData[][4] = { 
	{ 15, 7, 2, 1 }, 
	{ 15, 7, 2, 2 }, 
	{ 15, 7, 2, 3 },
	{ 15, 7, 2, 4 },
	{ 15, 7, 2, 5 },
	{ 15, 7, 2, 6 },
	{ 15, 7, 2, 9 },
	{ 15, 7, 2, 10 },
	{ 15, 7, 2, 11 },
	{ 15, 7, 2, 12 },
	{ 15, 7, 2, 13 } };

char* mapLevel01[] = {
	"##############################",
	"#****************************#",
	"#*###########*##*###########*#",
	"#*************##*************#",
	"####*##*##*########*##*##*####",
	"#****************************#",
	"#*###*###*###*##*#*####*####*#",
	"#*************#**#***********#",
	"#*###*###*###*##*#*########*##",
	"#****************************#",
	"####*##*##*########*##*##*####",
	"#*************##*************#",
	"#*###########*##*###########*#",
	"#****************************#",
	"##############################",
	'\0'};

int mainMenu();
void showScores();
int startGame();
void loadData();
void saveData();
void render(level*,character*,character**); //level,pacman,ghosts
bool update(level*,character*,character**); 
char*** getScores(int&);
bool checkIsInScores();
void addToScores();

int _tmain(int argc, _TCHAR* argv[])
{
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int action = 0;
	while (1)
	{
		action = mainMenu();

		switch (action)
		{
		case 1:
			points = 0;
			takedPoints = 0;
			gametime = 0;
			lives = 3;
			startGame();
			break;
		case 2:
			takedPoints = 0;
			loadData();
			startGame();
			break;
		case 3:
			showScores();
			break;
		case 4:
			return 0;
			break;
		}
	}
}

int mainMenu()
{
	system("cls");

	char action;
	//Start menu
	std::cout << "Press:\n N - New game\n L - Load game\n R - Records\n X - Exit";

	while (1)
	{
		action = _getch();
		switch (action)
		{
		case 'N':
		case 'n':
			return 1;
			break;
		case 'L':
		case 'l':
			return 2;
			break;
		case 'R':
		case 'r':
			return 3;
			break;
		case 'X':
		case 'x':
			system("cls");
			std::cout << "Now you will leave this game...";
			_getch();
			return 4;
			break;
		}
	}
}

void showScores(){
	system("cls");
	int i = 0;
	char ***numbers = getScores(i);

	std::cout << "Num." << "\t" << "Name" << "\t" << "Scores" << "\t" << "Time" << "\t" << endl;
	for (int j = 0; j < i; j++)
	{
		std::cout << j + 1 << "\t" << **(numbers + j) << "\t" << *(*(numbers + j) + 1) << "\t" << *(*(numbers + j) + 2) << "\t" << endl;
	}

	_getch();
}

char*** getScores(int &i){
	if ((fopen_s(&scoresFile, scoresFileName, "r")) != NULL)
	{
		puts("Open error");
		_getch();
	}

	char *active = new char[100],
		*element = new char[30];
	int n = 0;
	while (!feof(scoresFile))
	{
		fgets(active, 255, scoresFile);
		n++;
	}
	char ***numbers = new char**[n];
	fseek(scoresFile, 0, 0);
	int z = 0, v = 0, f = 0;
	i = 0;
	while (!feof(scoresFile))
	{
		fgets(active, 255, scoresFile);
		*(numbers + i) = new char*[3];
		do
		{
			if (*(active + z) == ';')
			{
				*(element + f) = '\0';
				numbers[i][v] = element;
				v++;
				element = new char[30];
				f = 0;
			}
			else  {
				*(element + f) = *(active + z);
				f++;
			}
			z++;
		} while ((*(active + z) != '\0') && (*(active + z) != '\n'));

		*(element + f) = '\0';
		numbers[i][v] = element;
		element = new char[30];
		f = 0;
		z = 0;

		v = 0;
		i++;
	}

	fclose(scoresFile);

	return numbers;
}

int startGame()
{
	while (activeLevel <= levels)
	{
		level* l1 = new level(mapLevel01);
		character* pacman = new character(1, 1, 1, 2, 14);
		character** ghosts = new character*[activeLevel + 1];
		points = points - takedPoints;
		takedPoints = 0;
		gameStatus = 1;
		plusTime = false;
		system("cls");

		for (int i = 0; i <= activeLevel; i++)
		{
			*(ghosts + i) = new character(2, ghostsData[i][0], ghostsData[i][1], ghostsData[i][2], ghostsData[i][3]);
		}

		render(l1, pacman, ghosts);

		while (gameStatus == 1)
		{
			Sleep(100);
			if (update(l1, pacman, ghosts))
				render(l1, pacman, ghosts);
		}

		system("cls");
		std::cout << "\n\n\t";

		switch (gameStatus)
		{
		case 2:
			if (activeLevel == levels)
			{
				std::cout << "Congratulations! You win the game!\n";
				if (checkIsInScores())
					addToScores();
				return 0;
			}
			else
			{
				std::cout << "Congratulations! Now you will go to the next level!\n\tYou can save game by pressing key S\n\tWARNING: It will overwrite your previous saved game.";
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); 
				char pressed = _getch();
				if (pressed == 's' || pressed == 'S')
					saveData();
			}
			takedPoints = 0;
			activeLevel++;
			break;
		case 3:
			if (lives > 1)
			{
				std::cout << "You died! Please press any key to restart level or X to exit.";
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				char pressed = _getch();
				if (pressed == 'x' || pressed == 'X')
					return 0;
				lives--;
			}
			else
			{
				if (checkIsInScores())
					addToScores();
				std::cout << "You lost! Please press any key to restart game or X to exit.";
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
				char pressed = _getch();
				if (pressed == 'x' || pressed == 'X')
					return 0;
				activeLevel = 1;
				takedPoints = 0;
				lives = 3;
				gametime = 0;
				points = 0;
			}
			break;
		}
	}

	return 0;
}

void render(level *l, character *p,character **g){
	COORD cursorCoords;
	cursorCoords.X = 0;
	cursorCoords.Y = 0;
	SetConsoleCursorPosition(consoleHandle, cursorCoords);

	bool rendered;

	std::cout << "Your points: " << points << "\t\t" << "Your game time: " << gametime << "\t\t" << "Your lives: " << lives << endl;
	std::cout << "\n\n\t";
	for (int row = 0; row < l->rows; row++)
	{
		for (int column = 0; column < l->columns; column++)
		{
			rendered = false;
			for (int i = 0; i <= activeLevel; i++)
			{
				if (g[i]->coordY == row && g[i]->coordX == column && !rendered)
				{
					SetConsoleTextAttribute(consoleHandle, g[i]->color);
					std::cout << g[i]->icon;
					rendered = true;
				}
			}

			if (p->coordY == row && p->coordX == column && !rendered)
			{

				SetConsoleTextAttribute(consoleHandle, p->color);
				std::cout << p->icon;
				rendered = true;
			}
			if (!rendered)
			{

				SetConsoleTextAttribute(consoleHandle, 7);
				std::cout << *(*(l->map + row) + column);
			}
		}
		std::cout << "\n\t";
	}
	std::cout << "\n\n Level: " << activeLevel;
};

bool update(level *l,character *p,character **g){

	for (int i = 0; i <= activeLevel; i++)
	{
		if (g[i]->coordX == p->coordX && g[i]->coordY == p->coordY)
		{
			gameStatus = 3;
			return false;
		}
	}

	if (plusTime == 4)
	{
		gametime++;
		plusTime = 0;
	}	else 
	plusTime++;

	if (GetAsyncKeyState(VK_UP))
		p->changeDirectionByKey(1);
	if (GetAsyncKeyState(VK_RIGHT))
		p->changeDirectionByKey(2);
	if (GetAsyncKeyState(VK_DOWN))
		p->changeDirectionByKey(3);
	if (GetAsyncKeyState(VK_LEFT))
		p->changeDirectionByKey(4);

	//----------------------------pacman moves--------------------------------
	int activeDirection = p->newDirection;
	bool checkAllDirections = true;
	do {
		int pNewX = p->coordX, pNewY = p->coordY;
		checkAllDirections = true;
		switch (activeDirection)
		{
		case 1:
			pNewY--;
			break;
		case 2:
			pNewX++;
			break;
		case 3:
			pNewY++;
			break;
		case 4:
			pNewX--;
			break;
		}


		if (!l->checkIsWall(pNewX, pNewY))
		{
			p->newCoords(pNewX, pNewY);
			p->changeDirection(activeDirection);
			if (l->checkIsPoint(pNewX, pNewY))
			{
				l->clearPoint(pNewX, pNewY);
				points++;
				takedPoints++;

				if (l->points == 0)
				{
					gameStatus = 2;
					return false;
				}
			}
		}
		else if (activeDirection != p->direction){
			activeDirection = p->direction;
			checkAllDirections = false;
		}
	} while (!checkAllDirections);
	//----------------------------end pacman moves--------------------------------


	for (int i = 0; i <= activeLevel; i++)
	{
		if (g[i]->coordX == p->coordX && g[i]->coordY == p->coordY)
		{
			gameStatus = 3;
			return false;
		}
	}

	//----------------------------ghosts moves------------------------------------
	activeDirection = 0;
	int gNewX = 0, gNewY = 0;
	bool moved = false;
	int followDir = 0, possibleDir = 0;
	for (int i = 0; i <= activeLevel; i++)
	{
		followDir = g[i]->checkIsInArea(l, p);

		if (followDir > 0)
		{
			g[i]->direction = followDir;
		} else {
			possibleDir = g[i]->checkForPossibleDir(l);
			if (possibleDir > 0)
				g[i]->direction = possibleDir;
		}
		do {
			moved = false;
			gNewX = g[i]->coordX; gNewY = g[i]->coordY;
			switch (g[i]->direction)
			{
			case 1:
				gNewY--;
				break;
			case 2:
				gNewX++;
				break;
			case 3:
				gNewY++;
				break;
			case 4:
				gNewX--;
				break;
			}

			if (!l->checkIsWall(gNewX, gNewY))
			{
				g[i]->newCoords(gNewX, gNewY);
				moved = true;
			}
			else
				g[i]->changeDirectionRandomly();
		} while (!moved);
	}
	//----------------------------end ghosts moves--------------------------------


	return true;
}

bool checkIsInScores()
{
	int i = 0;
	char ***scores = getScores(i);

	for (int j = 0; j < i; j++)
	{
		if ((atoi(*(*(scores + j) + 1)) < points) && (j < 10))
			return true;
	}

	return false;
}

void addToScores()
{
	char *yourname = new char[5];
	std::cout << "Please insert your name(it will be cut off till 5 symbols):" << endl;
	std::cin.getline(yourname,6);

	int i = 0;
	char ***scores = getScores(i);

	char ***newScores =  new char**[i + 1];
	for (int j = 0; j < i; j++)
	{
		*(newScores + j) = new char*[3];
		*(*(newScores + j)) = *(*(scores + j));
		*(*(newScores + j) + 1) = *(*(scores + j) + 1);
		*(*(newScores + j) + 2) = *(*(scores + j) + 2);
	}

	*(newScores + i) = new char*[3];
	*(*(newScores + i)) = yourname;
	char convertedPoints[30], convertedTime[30];
	_itoa_s(points, convertedPoints, 10);
	*(*(newScores + i) + 1) = convertedPoints;
	_itoa_s(gametime, convertedTime, 10);
	*(*(newScores + i) + 2) = convertedTime;

	bool sorted = true;
	char **temp = new char*[3];
	do {
		sorted = true;
		for (int j = 0; j < i; j++)
		{
			if (atoi(*(*(newScores + j) + 1)) < atoi(*(*(newScores + j + 1) + 1)))
			{
				temp = newScores[j];
				newScores[j] = newScores[j + 1];
				newScores[j + 1] = temp;
				sorted = false;
			}
		}
	} while (!sorted);

	if ((fopen_s(&scoresFile, scoresFileName, "w")) != NULL)
	{
		puts("Open error");
		_getch();
	}

	char *active = new char[100];
	int sPointer = 0, aPointer = 0;
	for (int j = 0; j < 10; j++)
	{
		sPointer = 0, aPointer = 0;
		while (*(*(*(newScores + j) + 0) + sPointer) != '\0')
		{
			*(active + aPointer) = *(*(*(newScores + j) + 0) + sPointer);
			aPointer++;
			sPointer++;
		}
		*(active + aPointer) = ';';
		aPointer++;
		sPointer = 0;
		while (*(*(*(newScores + j) + 1) + sPointer) != '\0')
		{
			*(active + aPointer) = *(*(*(newScores + j) + 1) + sPointer);
			aPointer++;
			sPointer++;
		}
		*(active + aPointer) = ';';
		aPointer++;
		sPointer = 0;
		while (*(*(*(newScores + j) + 2) + sPointer) != '\0')
		{
			*(active + aPointer) = *(*(*(newScores + j) + 2) + sPointer);
			aPointer++;
			sPointer++;
		}
		if (j != 9)
		{
			*(active + aPointer) = '\n';
			aPointer++;
		}
		*(active + aPointer) = '\0';

		fputs(active, scoresFile);
		active = new char[100];
	}

	fclose(scoresFile);

	system("cls");
	cout << "\n\n\t";
}

void loadData()
{
	if ((fopen_s(&saveFile, saveFileName, "r")) != NULL)
	{
		puts("Open error");
		_getch();
	}

	char *active = new char[100],
		*element = new char[30];
	int n = 0;
	char **numbers = new char*[4];
	int z = 0, v = 0, f = 0;
	while (!feof(saveFile))
	{
		fgets(active, 255, saveFile);
		do
		{
			if (*(active + z) == ';')
			{
				*(element + f) = '\0';
				numbers[v] = element;
				v++;
				element = new char[30];
				f = 0;
			}
			else  {
				*(element + f) = *(active + z);
				f++;
			}
			z++;
		} while ((*(active + z) != '\0') && (*(active + z) != '\n'));

		*(element + f) = '\0';
		numbers[v] = element;
		element = new char[30];
		f = 0;
		z = 0;

		v = 0;
	}

	fclose(saveFile);

	activeLevel = atoi(numbers[0]);
	points = atoi(numbers[1]);
	gametime = atoi(numbers[2]);
	lives = atoi(numbers[3]);
}

void saveData()
{
	if ((fopen_s(&saveFile, saveFileName, "w")) != NULL)
	{
		puts("Open error");
		_getch();
	}

	char *active = new char[100];
	char levelSave[2],
	pointSave[10],
	timeSave[10],
	livesSave[2];
	int sPointer = 0, aPointer = 0;

	_itoa_s(activeLevel + 1, levelSave, 10);
	_itoa_s(points, pointSave, 10);
	_itoa_s(gametime, timeSave, 10);
	_itoa_s(lives, livesSave, 10);

		while (*(levelSave + sPointer) != '\0')
		{
			*(active + aPointer) = *(levelSave + sPointer);
			aPointer++;
			sPointer++;
		}
		*(active + aPointer) = ';';
		aPointer++;
		sPointer = 0;
		while (*(pointSave + sPointer) != '\0')
		{
			*(active + aPointer) = *(pointSave + sPointer);
			aPointer++;
			sPointer++;
		}
		*(active + aPointer) = ';';
		aPointer++;
		sPointer = 0;
		while (*(timeSave + sPointer) != '\0')
		{
			*(active + aPointer) = *(timeSave + sPointer);
			aPointer++;
			sPointer++;
		}
		*(active + aPointer) = ';';
		aPointer++;
		sPointer = 0;
		while (*(livesSave + sPointer) != '\0')
		{
			*(active + aPointer) = *(livesSave + sPointer);
			aPointer++;
			sPointer++;
		}

		*(active + aPointer) = '\0';

		fputs(active, saveFile);

	fclose(saveFile);
}