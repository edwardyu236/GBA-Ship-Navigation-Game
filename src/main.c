#include <stdlib.h>
#include <stdio.h>
#include "mylib.h"
#include "text.h"

// **Display**
#define REG_DISPCNT *(u16*) 0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10) // bit 10
// ==Display==

// **Colour**
#define RGB(r,g,b) ((b)<<10 | (g)<<5 | (r))
#define RED RGB(31, 0, 0)
#define BLUE RGB(0, 0, 31)
#define GREEN RGB(0, 31, 0)

#define CYAN RGB(0, 31, 31)
#define MAGENTA RGB(31, 0, 31)
#define YELLOW RGB(31, 31, 0)
#define BLACK RGB(0, 0, 0)
#define WHITE RGB(31, 31, 31)

#define NYELLOW RGB(25, 25, 0)
#define NEARBLUE RGB(0, 0, 27)
// ==Colour
void drawString(int row, int col,  char* str, u16 color);

// **Game Items**
typedef struct
{
	int row;
	int col;
} PLAYERSHIP;

typedef struct
{
	int row;
	int col;
	int drow;
	int dcol;
} OBSTACLE;

typedef struct
{
	int row;
	int col;
} SHIELD;

#define NUMOBSTACLES 5
// ==Game Items==

// **State**
enum {SPLASH, GAME, WIN, LOSE};
void splash();
int game();
void win();
void lose();
// ==State==

char scoreBuffer[8];

int main(void)
{
	int state = SPLASH;
	while(1)
	{
		switch(state)
		{
			case SPLASH:
				splash();
				state = GAME;
				break;
			case GAME:
				state = game();
				break;
			case LOSE:
				lose();
				state = SPLASH;
				break;
			case WIN:
				win();
				state = SPLASH;
				break;
			default:
				break;
		}
	}
}

int game(void)
{
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	drawRect(0, 0, 240, 160, BLACK);


	OBSTACLE obstacles[NUMOBSTACLES];
	OBSTACLE oldObstacles[NUMOBSTACLES];
	OBSTACLE *cur;
	int d[] = {-3, -2, -1, 1, 2, 3};
	int numd = sizeof(d) / sizeof(d[0]);

	for (int i = 0; i < NUMOBSTACLES; i++)
	{
		obstacles[i].row = rand()%50 + 12;
		obstacles[i].col = rand()%50 + 12;
		obstacles[i].drow = d[rand()%numd];
		obstacles[i].dcol = d[rand()%numd];
		oldObstacles[i] = obstacles[i];
	}


	drawHollowRect(0, 0, 190, 160, WHITE);
	drawRect(2, 2, 186, 156, BLUE);

	PLAYERSHIP ship = {136, 160};
	PLAYERSHIP oldship = ship;

	SHIELD shield;
	shield.row = rand()%150 + 3;
	shield.col = rand()%180 + 3;
	
	int score = 0;

	drawString(0, 190, "Score:", WHITE);
	drawChar(8, 190, '0', WHITE);
	drawShip(ship.row, ship.col, YELLOW, NEARBLUE, NYELLOW, BLACK);
	for (int i = 0; i < NUMOBSTACLES; i++)
	{
		drawObstacle(obstacles[i].row, obstacles[i].col, RED, BLACK);
		oldObstacles[i] = obstacles[i];
	}
	drawShield(shield.row, shield.col, WHITE, GREEN);

	int protection = 0;

	drawChar(76, 117, '3', WHITE);
	for (int i = 0; i < 500; i++)
	{
		waitForVblank();
	}
	
	drawRect(76, 117, 6, 8, BLUE);
	drawChar(76, 117, '2', WHITE);
	for (int i = 0; i < 500; i++)
	{
		waitForVblank();
	}
	drawRect(76, 117, 6, 8, BLUE);
	drawChar(76, 117, '1', WHITE);
	for (int i = 0; i < 500; i++)
	{
		waitForVblank();
	}
	drawRect(76, 117, 6, 8, BLUE);
	while(1)
	{
		if(KEY_DOWN_NOW(BUTTON_UP))
		{
			ship.row--;
			if (ship.row < 2)
			{
				ship.row = 2;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_DOWN))
		{
			ship.row++;
			if (ship.row > 136)
			{
				ship.row = 136;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT))
		{
			ship.col--;
			if (ship.col < 2)
			{
				ship.col = 2;
			}
		}
		if(KEY_DOWN_NOW(BUTTON_RIGHT))
		{
			ship.col++;
			if (ship.col > 183)
			{
				ship.col = 183;
			}
		}
		for (int i = 0; i < NUMOBSTACLES; i++)
		{
			cur = obstacles + i;
			cur->row = cur->row + cur->drow;
			cur->col = cur->col + cur->dcol;
			if(cur->row < 2)
			{
				cur->row=2;
				cur->drow=-cur->drow;
			}
			if(cur->row > 153)
			{
				cur->row = 153;
				cur->drow=-cur->drow;
			}

			if(cur->col < 2)
			{
				cur->col = 2;
				cur->dcol = -cur->dcol;
			}
			if(cur->col > 183)
			{
				cur->col = 183;
				cur->dcol =-cur->dcol;
			}
			if (collision(cur->col, cur->row, ship.col, ship.row))
			{
				if (protection)
				{
					protection--;
				}
				else
				{
					if (score < 1000)
					{
						return LOSE;
					}
					else
					{
						return WIN;
					}
					
				}				
			}

		}
		if (protection <= 5 && collision(shield.col, shield.row, ship.col, ship.row))
		{
			protection = 10;
			drawRect(shield.row, shield.col, 5, 5, BLUE);

			shield.row = rand()%150 + 3;
			shield.col = rand()%180 + 3;
		}
		score++;
		if (score > 9999999)
		{
			return WIN;
		}
		sprintf(scoreBuffer, "%i", score);
		waitForVblank();
		drawRect(8, 190, 42, 8, BLACK);
		drawString(8, 190, scoreBuffer, WHITE);

		drawRect(oldship.row, oldship.col, 5, 22, BLUE);
		oldship = ship;
		

		if (protection > 5)
		{
			drawShip(ship.row, ship.col, YELLOW, GREEN, NYELLOW, BLACK);
		}
		else
		{
			drawShield(shield.row, shield.col, WHITE, GREEN);
			drawShip(ship.row, ship.col, YELLOW, NEARBLUE, NYELLOW, BLACK);

		}


		for (int i = 0; i < NUMOBSTACLES; i++)
		{
			drawRect(oldObstacles[i].row, oldObstacles[i].col, 5, 5, BLUE);
		}
		for (int i = 0; i < NUMOBSTACLES; i++)
		{
			drawObstacle(obstacles[i].row, obstacles[i].col, RED, BLACK);
			oldObstacles[i] = obstacles[i];
		}

	}

}

void splash()
{
	u16* ptr = videoBuffer;
	REG_DISPCNT = MODE3 | BG2_ENABLE;
	for (int i = 0; i < 38400; i++)
	{
		*ptr++ = BLUE;
	}

	drawString(20, 60, "SHIP NAVIGATION GAME", WHITE);
	drawString(34, 15, "Colliding an obstacle ends the game.", WHITE);
	drawString(48, 20, "Achieve a score over 1000 to win!", WHITE);
	drawString(132, 60, "Press Start (Enter)", WHITE);
	drawShip(69, 40, YELLOW, NEARBLUE, NYELLOW, BLACK);
	drawString(95, 20, "Your Ship", WHITE);
	drawObstacle(77, 110, RED, BLACK);
	drawObstacle(77, 125, RED, BLACK);
	drawString(95, 95, "Obstacles", WHITE);
	drawShield(77, 195, WHITE, GREEN);
	drawString(90, 180, "Shield", WHITE);
	drawString(100, 177, "Powerup", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

void lose()
{
	u16* ptr = videoBuffer;
	for (int i = 0; i < 38400; i++)
	{
		*ptr++ = RED;
	}

	drawString(20, 85, "YOU LOSE! :(", WHITE);
	drawString(70, 98, "Score:", WHITE);
	drawString(80, 104, scoreBuffer, WHITE);

	drawString(132, 45, "Press Start to Play Again", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}

void win()
{
	u16* ptr = videoBuffer;
	for (int i = 0; i < 38400; i++)
	{
		*ptr++ = GREEN;
	}

	drawString(20, 85, "YOU WIN! :)", WHITE);
	drawString(70, 98, "Score:", WHITE);
	drawString(80, 104, scoreBuffer, WHITE);

	drawString(132, 45, "Press Start to Play Again", WHITE);

	while(!KEY_DOWN_NOW(BUTTON_START));
	while(KEY_DOWN_NOW(BUTTON_START));
}