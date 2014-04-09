typedef unsigned short u16;

// **Display**
#define OFFSET(r,c, numcols) ((r)*(numcols) + (c))
extern u16* videoBuffer;
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006
// ==Display==

// **Buttons**
#define BUTTON_A	(1<<0)
#define BUTTON_B	(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT	(1<<5)
#define BUTTON_UP	(1<<6)
#define BUTTON_DOWN	(1<<7)
#define BUTTON_R	(1<<8)
#define BUTTON_L	(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130
// ==Buttons==

// **Prototypes**
void setPixel(int r, int c, u16 colour);
void drawRect(int r, int c, int width, int height, u16 colour);
void drawHollowRect(int r, int c, int width, int height, u16 colour);


void drawShip(int r, int c, u16 colour, u16 colourBG, u16 colourA, u16 colourC);
void drawShip0(int r, int c, u16 colour, u16 colourBG);
void drawShip1(int r, int c, u16 colour, u16 colourBG);
void drawShip2(int r, int c, u16 colour);

void drawObstacle(int r, int c, u16 colourBG, u16 colourX);
void drawShield(int r, int c, u16 colourBG, u16 colourS);

void waitForVblank();

int collision(int boxCol, int boxRow, int shipCol, int shipRow);
// ==Prototypes==