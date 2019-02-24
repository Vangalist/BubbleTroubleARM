#include "ball32.h"
#include "player.h"
#define ScreenWidhth	320
#define ScreenHeight	240
//set basic colors for testing
#define white	 0xE610
#define black	 0xD489
#define Alibb	 0xA118

volatile char* point1 = (char*)0xC8000000;
void loadTitleScreen();
void	 resetScreen();
void	 fillScreen();
void	drawFire(int X,int Y);
void drawPlayer(int playerPosition, int playerPositionY, int direction);
void drawBall(int x1, int y1, int Radi);
void VGA_text(int x, int y, char * text_ptr);
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color);
void VGA_pic(int x, int y, int width, int height, short* pic);
void deleteObj(int x, int y ,int width,int height);
void endGameScreen(int i);
void lvlPassedScreen();
void drawHealth(int health);

void drawBigBall(int Y);
void deleteBigBall(int Y);
void deleteBalls1(int ball1X, int ball1Y);
void drawBalls1(int ball1X, int ball1Y);
//void	 displayImage(int x, int y, int width, int height, int**image);
//char**	 decodeImage(char* colSpace,char** compressedIm);





