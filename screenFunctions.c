#include "screenFunctions.h"
#include "address_map_arm.h"
#include <inttypes.h>
/*
void	 resetScreen() {
	VGA_box(0, 0, 319, 239, 0x38EF);
}*/
void	 resetScreen() {
	VGA_pic(0, 0, 320, 240, level1);
}
void loadTitleScreen() {
	VGA_pic(0, 0, 320, 240, titlescreen);
}


void drawHealth(int health) {
	int x = 230;
	int y = 210;
	int i = 0;
	short color;
	int pixel_ptr, row, col;
	for (i = 0; i < health; i++) {
		x += 20;
		for (row = y; row < y+12; row++) {
			for (col = x; col < x+16; ++col)
			{
				color = heartPic[(row - y) * 23 + col - x];
				if (color != 1341) {
					pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
					*(short *)pixel_ptr = color;		// compute halfword address, set pixel
				}
			}

		}
	}
}
void drawFire(int x1,int y1) {
	short color;
	int pixel_ptr, row, col;
	int x2 = x1 + 2;
	int y2 = 200;
	x1 -= 2;
	color = 0xF810;
	for (row = y1; row < y2; row++) {
		for (col = x1; col < x2; ++col)
		{
				color = arrow[(row - y1) * 4 + col - x1];
				if(color!=1341){
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = color;		// compute halfword address, set pixel
		}
		}
	}
}

void fillScreen() {

}
/*
void drawPlayer(int playerPosition,int playerPositionY,int direction) {
	VGA_box(playerPosition - 10, playerPositionY - 10, playerPosition + 10, playerPositionY + 30, 0xFC00);//orange
}*/
void drawPlayer(int x1, int y1, int direction) {

	short color;
	int pixel_ptr, row, col;
	int x2 = x1 + 13;
	int y2 = y1 + 27;

	x1 -= 10;
	y1 -= 10;
	if(direction==0){
	for (row = y1; row < y2; row++) {
		for (col = x1; col < x2; ++col)
		{
			color = player[(row - y1) * 23 + col - x1];
			if (color != 1341) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = color;		// compute halfword address, set pixel
			}
		}
	}
	}else 	if(direction==1){
		for (row = y1; row < y2; row++) {
		for (col = x1; col < x2; ++col)
		{
			color = playerRight[(row - y1) * 23 + col - x1];
			if (color != 1341) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = color;		// compute halfword address, set pixel
			}
		}
	}
	}else 	if(direction==-1){
		for (row = y1; row < y2; row++) {
		for (col = x1; col < x2; ++col)
		{
			color = playerLeft[(row - y1) * 23 + col - x1];
			if (color != 1341) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = color;		// compute halfword address, set pixel
			}
		}
	}
	}
}


void lvlPassedScreen(){
	short color;
	int pixel_ptr, row, col;
	
		for (row = 0; row < 240; row++) {
		for (col = 0; col < 320; ++col)
		{
			color = nextLvl[(row ) * 320 + col ];
			if (color != 1341) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = color;		// compute halfword address, set pixel
			}
		}
	}
}

void drawBall(int x1, int y1,int Radi)
{
	int pixel_color = 0xF800;
	int pixel_ptr, row, col;
	int x2 = x1 + Radi;
	int y2 = y1 + Radi;
	x1 -= Radi;
	y1 -= Radi;
	int Ycenter = y1 + Radi;
	int Xcenter = x1 + Radi;
	Radi = Radi*Radi;
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)
		{
			if((row-Ycenter)*(row -Ycenter)+ (col - Xcenter)*(col - Xcenter)<Radi){
			pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
			*(short *)pixel_ptr = pixel_color;		// compute halfword address, set pixel
			}
		}
}
/*
void drawBall(int x1, int y1, int Radi)
{
	short color;
	int pixel_ptr, row, col;
	int x2 = x1 + Radi;
	int y2 = y1 + Radi;
	x1 -= Radi;
	y1 -= Radi;


	for (row = y1; row < y2; row++) {
		for (col = x1; col < x2; ++col)
		{
			color = ball32[(row - y1)*32 + (col - x1)];
			if (color != 1341) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = color;		// compute halfword address, set pixel
			}
		}
	}
}
*/

void endGameScreen(int i){

	short color;
	short color2;
	int pixel_ptr, row, col;
	
	if(i==0){
	color=0x181F;
	}else if(i==1){
	color=0xF800;
	}
	else if(i==2){
	color=0x07E0;
	}
	else if(i==3){
	color=0xFFe0;
	}
	
	for (row = 0; row < 240; row++) {
		for (col = 0; col < 320; ++col)
		{
			color2 = endGameScreen1[(row ) * 320 + col ];
			if (color2 != 1341) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = color;		// compute halfword address, set pixel
			}
		}
	}

}
void VGA_box(int x1, int y1, int x2, int y2, short pixel_color)
{
	int pixel_ptr, row, col;

	/* assume that the box coordinates are valid */
	for (row = y1; row <= y2; row++)
		for (col = x1; col <= x2; ++col)
		{
			pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
			*(short *)pixel_ptr = pixel_color;		// compute halfword address, set pixel
		}
}
void VGA_text(int x, int y, char * text_ptr)
{
	int offset;
	volatile char * character_buffer = (char *)FPGA_CHAR_BASE;	// VGA character buffer

																/* assume that the text string fits on one line */
	offset = (y << 7) + x;
	while (*(text_ptr))
	{
		*(character_buffer + offset) = *(text_ptr);	// write to the character buffer
		++text_ptr;
		++offset;
	}
}
void VGA_pic(int x, int y, int width, int height, short* pic) {
	int pixel_ptr, row, col;
	int x2 = x + width;
	int y2 = y + height;
	/* assume that the box coordinates are valid */
	for (row = y; row < y2; row++)
		for (col = x; col < x2; ++col)
		{
			pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
			*(short *)pixel_ptr = pic[(row-y)*width+(col-x)] ;		// compute halfword address, set pixel
		}
}
void deleteObj(int x, int y ,int width,int height){
	int pixel_ptr, row, col;
	int x2 = x + width;
	int y2 = y + height;
	
		for (row = y; row < y2; row++)
		for (col = x; col < x2; ++col)
		{
	
			pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
			*(short *)pixel_ptr = level1[(row)*320+(col)] ;		// compute halfword address, set pixel
		}

}


void drawBigBall(int y1) {

	int x1 = 160 - 47;
	int x2 = 160 + 47;

	int y2 = y1+47;
	y1 -= 47;

	short color;
	int pixel_ptr, row, col;

		for (row = y1; row < y2; row++) {
			for (col = x1; col < x2; ++col)
			{
				if (row > 0) {
					color = youwinBallPic[(row - y1) * 94 + col - x1];
					if (color != 1341) {
						pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
						*(short *)pixel_ptr = color;		// compute halfword address, set pixel
					}
				}
			}
	}

}
void deleteBigBall(int y1) {

	int pixel_ptr, row, col;
	int x1 = 160 - 47;
	int x2 = 160 + 47;

	int y2 = y1 + 47;
	y1 -= 47;

	for (row = y1; row < y2; row++)
		for (col = x1; col < x2; ++col)
		{
			if (row > 0) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = level1[(row) * 320 + (col)];		// compute halfword address, set pixel
			}
		}

}
void deleteBalls1(int ball1X, int ball1Y) {

	int x1 = 160 - ball1X - 40;
	int x2 = 160 - ball1X + 40;

	int y2 = ball1Y + 40;
	int y1 = ball1Y - 40;

	short color;
	int pixel_ptr, row, col;

	for (row = y1; row < y2; row++)
		for (col = x1; col < x2; ++col)
		{
			if (row > 0) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = level1[(row) * 320 + (col)];		// compute halfword address, set pixel
			}
		}

	x1 = 160 + ball1X - 40;
	x2 = 160 + ball1X + 40;

	for (row = y1; row < y2; row++)
		for (col = x1; col < x2; ++col)
		{
			if (row > 0) {
				pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
				*(short *)pixel_ptr = level1[(row) * 320 + (col)];		// compute halfword address, set pixel
			}
		}
}
void drawBalls1(int ball1X, int ball1Y) {

	int x1 = 160 - ball1X-40;
	int x2 = 160 - ball1X+40;

	int y2 = ball1Y + 40;
	int y1 = ball1Y - 40;

	short color;
	int pixel_ptr, row, col;

	for (row = y1; row < y2; row++) {
		for (col = x1; col < x2; ++col)
		{
			if (row > 0) {
				color = youBallPic[(row - y1) * 80 + col - x1];
				if (color != 1341) {
					pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
					*(short *)pixel_ptr = 0xFCA0;		// compute halfword address, set pixel
				}
			}
		}
	}

	x1 = 160 + ball1X - 40;
	x2 = 160 + ball1X + 40;

	for (row = y1; row < y2; row++) {
		for (col = x1; col < x2; ++col)
		{
			if (row > 0) {
				color = winBallPic[(row - y1) * 80 + col - x1];
				if (color != 1341) {
					pixel_ptr = FPGA_ONCHIP_BASE + (row << 10) + (col << 1);
					*(short *)pixel_ptr = 0xFCA0;		// compute halfword address, set pixel
				}
			}
		}
	}


}