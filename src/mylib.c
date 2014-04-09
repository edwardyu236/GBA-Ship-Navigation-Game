#include "mylib.h"
u16* videoBuffer = (u16*) 0x6000000;

// A function to set pixel (r, c) to the colour passed in.
void setPixel(int r, int c, u16 colour)
{
	videoBuffer[OFFSET(r, c, 240)] = colour;
}

// A function to draw a FILLED rectangle starting at (r, c).
void drawRect(int r, int c, int width, int height, u16 colour)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            setPixel(r+row, c+col, colour);
        }
    }
}

// A function to draw a HOLLOW rectangle starting at (r, c).
void drawHollowRect(int r, int c, int width, int height, u16 colour)
{
	int row, col;
	row = 0;
	for (col = 0; col < width; col++) {
		setPixel(r+row, c+col, colour);
	}

	col = 0;
	for (row = 1; row < height - 1; row++) {
		setPixel(r+row, c+col, colour);
	}

	col = width - 1;
	for (row = 1; row < height - 1; row++) {
		setPixel(r+row, c+col, colour);
	}


    row = height - 1;
	for (col = 0; col < width; col++) {
		setPixel(r+row, c+col, colour);
	}
}

void drawShip(int r, int c, u16 colour, u16 colourBG, u16 colourA, u16 colourC)
{
	for (int i = 0; i < 3; i++)
	{
		drawShip0(r+i, c, colour, colourBG);
	}
	for (int i = 3; i < 7; i++)
	{
		drawShip1(r+i, c, colour, colourBG);
	}
	for (int i = 7; i < 15; i++)
	{
		drawShip2(r+i, c, colour);
	}
	for (int i = 15; i < 19; i++)
	{
		drawShip1(r+i, c, colour, colourBG);
	}
	for (int i = 19; i < 22; i++)
	{
		drawShip0(r+i, c, colour, colourBG);
	}
	drawRect(r+6, c+2, 1, 10, colourA);
	
	setPixel(r+8, c+2, colourC);
	setPixel(r+9, c+2, colourC);
	setPixel(r+12, c+2, colourC);
	setPixel(r+13, c+2, colourC);
	
}

void drawShip0(int r, int c, u16 colour, u16 colourBG)
{
	for (int i = 0; i < 5; i++)
	{
		setPixel(r, c+i, colourBG);
	}
	setPixel(r, c+2, colour);
}

void drawShip1(int r, int c, u16 colour, u16 colourBG)
{
	for (int i = 0; i < 5; i++)
	{
		setPixel(r, c+i, colourBG);
	}
	for (int i = 1; i < 4; i++)
	{
		setPixel(r, c+i, colour);
	}
}

void drawShip2(int r, int c, u16 colourBG)
{
	for (int i = 0; i < 5; i++)
	{
		setPixel(r, c+i, colourBG);
	}
}

void drawObstacle(int r, int c, u16 colourBG, u16 colourX)
{
	for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            setPixel(r+row, c+col, colourBG);
        }
    }

    setPixel(r+1, c+1, colourX);
    setPixel(r+1, c+3, colourX);
    setPixel(r+2, c+2, colourX);
    setPixel(r+3, c+1, colourX);
    setPixel(r+3, c+3, colourX);
}

void drawShield(int r, int c, u16 colourBG, u16 colourS)
{
	for (int row = 0; row < 5; row++)
    {
        for (int col = 0; col < 5; col++)
        {
            setPixel(r+row, c+col, colourBG);
        }
    }

    drawRect(r+1, c+1, 3, 2, colourS);
    setPixel(r+3, c+2, colourS);
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

int collision(int boxCol, int boxRow, int shipCol, int shipRow)
{
	if (shipRow < (boxRow + 5) && (shipRow + 22) > boxRow)
	{
		if (shipCol < (boxCol + 5) && (shipCol + 5) > boxCol)
		{
			return 1;
		}
	}

	return 0;
}