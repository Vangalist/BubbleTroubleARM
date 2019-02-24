//GameModes

#include <stdio.h>
#include <inttypes.h>

#define StartScreen 0
#define OnePlayer	1
#define GameOn		2
#define LoseScreen	3

#define lvl1		5
#define lvl2		6
#define lvl3		7
#define lvl4		8
#define WinScreen	9
#define lvlPassed	10

#define ballX		0
#define ballY		1
#define ballRadious 2
#define ballUD	    3
#define ballRL		4

#define sw ((volatile unsigned int *) SW_BASE)

#define maxBallSize 16
double jumpCoef=1;
int Score = 0;
int tempGamemode = 0;

int screen_x=319; int screen_y=239;

int gameMode;//set initial game mode
int playerDir=0;
int health = 0;

// fire sets
int isFireOn  = 0;
int fireX = 0;
int fireY = 0;
int directionOst=0;

void activateVoice();
//player cordinates
int playerPosition = 200;
int playerPositionY = 173;
//set ball Settings
int ballSetings[32][5];

void	 startScreen();
void	 onePlayer();
void	 gameOn();
void	 loseScreen();
void	 winScreen();
void     setupGame();
void	 letTheGameBegin();
//Lvl settings
void	 setupBallsLvl1();
void 	 setupBallsLvl2();
void 	 setupBallsLvl3();
void 	 setupBallsLvl4();
void	 goLvl1();
void	 goLvl2();
void	 goLvl3();
void	 goLvl4();
void	 goAnyLvl();
void	 levelPassed();

void	 updateBalls();
void	 updatePlayer();
void	 updateFire();
void	 checkAction();


int isWinGame();

void deleteFire();
void deleteBalls();
void deletePlayer();

void	drawBalls(fireX,fireY);

void	ballHit(int index);

void set_A9_IRQ_stack(void);
void config_GIC(void);
void enable_A9_interrupts(void);
void config_PS2();
void config_MPcore_private_timer();