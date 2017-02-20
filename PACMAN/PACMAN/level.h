#pragma once
class level
{
public:
	int columns,rows,points;
	unsigned char** map;

	const unsigned char emptySymb = 0;
	const unsigned char wallSymb = 219;
	const unsigned char pointSymb = 249;
	const unsigned char exitSymb = 176;

	level();
	level(char**);
	~level();

	bool checkIsWall(int, int);
	bool checkIsPoint(int, int);

	void clearPoint(int, int);
};


