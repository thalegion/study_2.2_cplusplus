#include "stdafx.h"
#include "level.h"

level::level(){
	this->columns = NULL;
	this->rows = NULL;
	this->points = NULL;
	this->map = NULL;
}
level::level(char **m)
{
	this->columns = NULL;
	this->rows = NULL;
	this->points = 0;
	this->map = new unsigned char*;
	char **current = m;
	int column = 0, row = 0;

	do{
		do{
			column++;
		} while (*(*(current + row) + column) != '\0');
		if (this->columns == NULL) this->columns = column+1;
		row++;
		column = 0;
	} while (*(current + row + 1) != '\0');
	this->rows = row+1;

	this->map = new unsigned char*[this->rows];
	for (row = 0; row <= this->rows; row++)
	{
		*(this->map + row) = new unsigned char[this->columns];
	}

	for (row = 0; row < this->rows; row++)
	{
		for (column = 0; column < this->columns; column++)
		{
			switch (*(*(current + row) + column))
			{
			case '#':
				*(*(this->map + row) + column) = wallSymb;
				break;
			case '*':
				*(*(this->map + row) + column) = pointSymb;
				points++;
				break;
			default:
				*(*(this->map + row) + column) = emptySymb;
				break;
			}
		}
	}
}

level::~level()
{
}

bool level::checkIsWall(int x, int y)
{
	if (*(*(this->map + y) + x) == wallSymb)
		return true;
	else 
		return false;
}

bool level::checkIsPoint(int x, int y)
{
	if (*(*(this->map + y) + x) == pointSymb)
		return true;
	else
		return false;
}

void level::clearPoint(int x, int y)
{
	*(*(this->map + y) + x) = emptySymb;
	points--;
}