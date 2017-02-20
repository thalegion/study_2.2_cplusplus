#include "stdafx.h"
#include "character.h"
#include "level.h"
#include "time.h"
#include "cstdlib"


character::character()
{
	this->coordX = 0;
	this->coordY = 0;
	this->lastCoordX = 0;
	this->lastCoordY = 0;
	this->direction = 0;
	this->newDirection = 0;
	this->icon = 0;
	this->color = 15;
	srand(time(0));
}

character::character(int t, int x, int y, int d,int c)
{
	this->icon = (t==1?pacmanSymb:ghostSymb);
	this->color = c;
	this->coordX = x;
	this->coordY = y;
	this->lastCoordX = x;
	this->lastCoordY = y;
	this->direction = d;
	this->newDirection = d;
	srand(time(0));
}

character::~character()
{
}

void character::newCoords(int x, int y){
	this->lastCoordX = this->coordX;
	this->lastCoordY = this->lastCoordY;
	this->coordX = x;
	this->coordY = y;
}

void character::changeDirection(int d)
{
	this->direction = d;
}
void character::changeDirectionByKey(int d)
{
	this->newDirection = d;
}
void character::changeDirectionRandomly()
{
	int dir = 1 + rand() % 4;
	this->direction = dir;
}

int character::checkIsInArea(level *l,character *p)
{
	if ((p->coordX == this->coordX) || (p->coordY == this->coordY))
	{
		int dir = 0, from = 0, to = 0;
		bool go = true;

		if (p->coordX == this->coordX)
		{
			if (p->coordY > this->coordY)
			{
				from = this->coordY;
				to = p->coordY;
				dir = 3;
			} else {
				from = p->coordY;
				to = this->coordY;
				dir = 1;
			}

				for (int i = from; i < to; i++)
				{
					if (*(*(l->map + i) + this->coordX) == l->wallSymb)
					{
						go = false;
					}
				}
		} else {
			if (p->coordX > this->coordX)
			{
				from = this->coordX;
				to = p->coordX;
				dir = 2;
			}
			else {
				from = p->coordX;
				to = this->coordX;
				dir = 4;
			}

			for (int i = from; i < to; i++)
			{
				if (*(*(l->map + this->coordY) + i) == l->wallSymb)
				{
					go = false;
				}
			}
		}

		if (go)
		{
			return dir;
		}
	}

	return 0;
}

int character::checkForPossibleDir(level* l)
{
	int possibleDirCount = 0;
	int *possibleDirs = new int[4];
	if ((*(*(l->map + this->coordY - 1) + this->coordX) != l->wallSymb) && ((this->coordY - 1 != this->lastCoordY) && ((this->coordX != this->lastCoordX))))
	{
		possibleDirs[possibleDirCount] = 1;
		possibleDirCount++;
	}
	if ((*(*(l->map + this->coordY + 1) + this->coordX) != l->wallSymb) && ((this->coordY + 1 != this->lastCoordY) && ((this->coordX != this->lastCoordX))))
	{
		possibleDirs[possibleDirCount] = 3;
		possibleDirCount++;
	}
	if ((*(*(l->map + this->coordY) + this->coordX - 1) != l->wallSymb) && ((this->coordY != this->lastCoordY) && ((this->coordX - 1 != this->lastCoordX))))
	{
		possibleDirs[possibleDirCount] = 4;
		possibleDirCount++;
	}
	if ((*(*(l->map + this->coordY) + this->coordX + 1) != l->wallSymb) && ((this->coordY != this->lastCoordY) && ((this->coordX + 1 != this->lastCoordX))))
	{
		possibleDirs[possibleDirCount] = 2;
		possibleDirCount++;
	}

	if (possibleDirCount > 1)
	{
		return possibleDirs[rand() % possibleDirCount];
	}

	return 0;
}