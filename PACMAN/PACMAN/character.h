#pragma once
#include "level.h"
class character
{
public:
	unsigned char icon; //1 - pacman, 2 - ghost
	int coordX,coordY,lastCoordX,lastCoordY;
	int newDirection, direction; //1-up,2-right,3-down,4-left
	int color;


	const unsigned char pacmanSymb = 2;
	const unsigned char ghostSymb = 1;

	character();
	character(int, int, int, int,int); //type,coords,direction,color
	~character();

	void newCoords(int, int);
	void changeDirection(int);
	void changeDirectionByKey(int);
	void changeDirectionRandomly();
	int checkIsInArea(level*,character*);
	int checkForPossibleDir(level*);
};

