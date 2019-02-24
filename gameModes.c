#include "gameModes.h"
#include "address_map_arm.h"

	
extern volatile char byte1, byte2, byte3;			/* modified by PS/2 interrupt service routine */
extern volatile int  buffer_index;	// used for audio
extern volatile int timeout;							// used to synchronize with the timer

void     setupGame() {
	//set interrupts
	byte1 = 0; byte2 = 0; byte3 = 0; 			// used to hold PS/2 data
	//record = 0; play = 0; buffer_index = 0;	// used for audio record/playback
	timeout = 0;										// synchronize with the timer

	set_A9_IRQ_stack();					// initialize the stack pointer for IRQ mode
	config_GIC();							// configure the general interrupt controller
	config_MPcore_private_timer();	// configure ARM A9 private timer
	config_PS2();							// configure PS/2 port to generate interrupts
	enable_A9_interrupts();			// enable interrupts
	gameMode = StartScreen;
}
void	 letTheGameBegin() {
	// let the game begin
	while (1) {
	
		while(timeout){
		if(gameMode==lvl1){
			goLvl1();
		} else if(gameMode==StartScreen){
			startScreen();
		}else if (gameMode==LoseScreen){
			loseScreen();
		}else if (gameMode==WinScreen){
			winScreen();
		}else if(gameMode==lvl2){
			goLvl2();
		}else if(gameMode==lvl3){
			goLvl3();
		}else if(gameMode==lvl4){
			goLvl4();
		}else if(gameMode==lvlPassed){
			levelPassed(); 
		}
		
		/*
		switch (gameMode)
		{
		case lvl1:
			goLvl1();
		case StartScreen:
			startScreen();
		case OnePlayer:
			onePlayer();
		case GameOn:
			gameOn();
		case LoseScreen:
			loseScreen();
		case WinScreen:
			winScreen();

		default:
			break;
		}*/
	}
}
}
void startScreen() {
	health = 3;
	loadTitleScreen();
	while (1) {
		while (timeout) {
		if (byte3 ==90){
			gameMode = lvl1;
			byte3=0;
			timeout=0;
			return;
		}
		else if(byte3==43){
			gameMode = winScreen;
			byte3=0;
			timeout=0;
			return;
		}
		else if(byte3==30){
			gameMode = lvl2;
			byte3=0;
			timeout=0;
			return;
		}
		else if(byte3==38){
			gameMode = lvl3;
			byte3=0;
			timeout=0;
			return;
		}
		else if(byte3==37){
			gameMode = lvl4;
			byte3=0;
			timeout=0;
			return;
		}
		}
	}
}

void goLvl1() {
	resetScreen();
	setupBallsLvl1();
	goAnyLvl();

	return;
}

void goLvl2() {
	resetScreen();
	setupBallsLvl2();
	goAnyLvl();
	return;
}
void goLvl3() {
	resetScreen();
	setupBallsLvl3();
	goAnyLvl();
	return;
}
void goLvl4() {
	resetScreen();
	setupBallsLvl4();
	goAnyLvl();
	return;
}

void goAnyLvl() {

	drawHealth(health);
	byte3=0;
	isFireOn=0;
	//*unsigned int *sw = *SW_BASE;
	jumpCoef=1;//-(*sw&0x3AA)/1024;
	while (1) {
		while (timeout) {
			deleteBalls();
			deletePlayer();
			updateBalls();//done
			updatePlayer();//done
			checkAction();

			//resetScreen();
			drawBalls();
			if (isFireOn) {
				drawFire(fireX, fireY);
			}
			drawPlayer(playerPosition, playerPositionY, playerDir);
			updateFire();
			if (checkEndGame()) {
				if (health == 0) {
					gameMode = LoseScreen;
					timeout = 0;
					return;
				}
				else {
					health--;
					timeout = 0;
					while (1) {
						while (timeout) {
							if (byte3 == 90) {
								timeout = 0;
								byte3 = 0;
								return;
							}
						}
					}
				}
			}

			if (isWinGame()) {

				tempGamemode = gameMode+1;
				gameMode = lvlPassed;
				timeout = 0;
				return;

			}
			timeout = 0;
		}
	}
}


void levelPassed(){
	lvlPassedScreen();
		while (1) {
		while(timeout){
		if (byte3 == 90) {
			gameMode = tempGamemode;
			byte3 = 0;
			timeout=0;
			return;
		}
		timeout=0;
		}
	}
	
}


void	 onePlayer() {

}

void	 gameOn() {
	gameMode = LoseScreen;
	return 0;
}
void	 loseScreen() {
int i=0;
	while (1) {
		while(timeout){

		if(i%4==0){
		endGameScreen((i/4)%4);
		}
		if (byte3 == 66) {
			gameMode = StartScreen;
			byte3 = 0;
			return;
		}
		i++;
		timeout=0;
		}
	}

}
void	 winScreen() {

	//move the guy in to the middle
	int centerX = 160;
	int guyIntheMiddle = 1;
	while (guyIntheMiddle) {
		while (timeout) {
			deletePlayer();
			if (playerPosition < 160) {
				playerPosition+=2;
				playerDir = 1;
			}
			else {
				playerPosition -= 2;
				playerDir = -1;
			}
			if (playerPosition > 158 && playerPosition < 162) {
				playerDir = 0;
				guyIntheMiddle = 0;
			}
			drawPlayer(playerPosition,playerPositionY,playerDir);
			timeout = 0;
		}
	}
	// ball  düþür ve adam ok atsýn
	int isMeet=1;
	int bigBallY=-40;
	int bigBallSpeed = 0;
	isFireOn = 1;
	fireX = playerPosition;
	fireY = 200;
	while (isMeet) {
		while (timeout) {
			deleteBigBall(bigBallY);
			fireY -= 3;
			bigBallSpeed += 20;
			bigBallY += bigBallSpeed / 100;
			drawBigBall(bigBallY);
			drawFire(fireX,fireY);
			drawPlayer(playerPosition, playerPositionY, playerDir);
			if (fireY < bigBallY+45) {
				deleteBigBall(bigBallY);
				deleteFire(fireX, fireY);
				isMeet = 0;
				timeout = 0;
			}
			timeout = 0;
		}
	}
	drawPlayer(playerPosition,playerPositionY,0);
	//top ikiye ayrýlýp seksin
	int ball1X=0;
	int ball1Y=bigBallY;//ortaya olan uzaklýk
	
	int ball1Speed = 0;

	int isTouchGround = 1;
	while (isTouchGround) {
		while (timeout) {
			deleteBalls1(ball1X,ball1Y);
			ball1Speed += 20;
			ball1Y += ball1Speed / 100;
			ball1X+=2;
			drawBalls1(ball1X, ball1Y);
			if (ball1Y+38 > 200) {
				isTouchGround = 0;
			}
			timeout = 0;
		}
	}
	ball1Speed = -1 * ball1Speed;
	//yere deðince dikey zýplasýn
	while (1) {
		while (timeout) {
			deleteBalls1(ball1X,ball1Y);
			ball1Speed += 20;
			ball1Y += ball1Speed / 100;
			drawBalls1(ball1X, ball1Y);
			
			if (ball1Y+38 > 200) {
				ball1Speed = -1 * ball1Speed;
			}
			
			if (byte3 == 41) {
				gameMode = StartScreen;
				timeout = 0;
				byte3 = 0;
				return;
			}
			timeout = 0;
		}
	}
	
	//gameMode = StartScreen;
	
	return ;
}

void setupBallsLvl1(){
	memset(ballSetings, 0, sizeof(ballSetings));
	
	ballSetings[0][ballX] = 100;
	ballSetings[0][ballY] = 20;
	ballSetings[0][ballRadious] = maxBallSize;
	ballSetings[0][ballRL] = -1;
	ballSetings[0][ballUD] = 0;

	ballSetings[1][ballX] = 220;
	ballSetings[1][ballY] = 20;
	ballSetings[1][ballRadious] = maxBallSize;
	ballSetings[1][ballRL] = 1;
	ballSetings[1][ballUD] = 0;
	/*
	ballSetings[2][ballX] = 220;
	ballSetings[2][ballY] = 150;
	ballSetings[2][ballRadious] = maxBallSize;
	ballSetings[2][ballRL] = -1;
	ballSetings[2][ballUD] = 0;*/

	playerPosition=150;
}
void setupBallsLvl2(){
	memset(ballSetings, 0, sizeof(ballSetings));
	
	ballSetings[0][ballX] = 100;
	ballSetings[0][ballY] = 20;
	ballSetings[0][ballRadious] = maxBallSize;
	ballSetings[0][ballRL] = 1;
	ballSetings[0][ballUD] = 0;
	/*
	ballSetings[1][ballX] = 220;
	ballSetings[1][ballY] = 20;
	ballSetings[1][ballRadious] = maxBallSize;
	ballSetings[1][ballRL] = -1;
	ballSetings[1][ballUD] = 0;
	
	ballSetings[2][ballX] = 160;
	ballSetings[2][ballY] = 20;
	ballSetings[2][ballRadious] = maxBallSize;
	ballSetings[2][ballRL] = -1;
	ballSetings[2][ballUD] = 0;
	*/
	playerPosition=150;
}
void setupBallsLvl3(){
	memset(ballSetings, 0, sizeof(ballSetings));
	
	ballSetings[0][ballX] = 50;
	ballSetings[0][ballY] = 70;
	ballSetings[0][ballRadious] = maxBallSize/2;
	ballSetings[0][ballRL] = 1;
	ballSetings[0][ballUD] = 0;

	ballSetings[1][ballX] = 80;
	ballSetings[1][ballY] = 70;
	ballSetings[1][ballRadious] = maxBallSize/2;
	ballSetings[1][ballRL] = 1;
	ballSetings[1][ballUD] = 0;
	
	ballSetings[2][ballX] = 110;
	ballSetings[2][ballY] = 70;
	ballSetings[2][ballRadious] = maxBallSize/2;
	ballSetings[2][ballRL] = 1;
	ballSetings[2][ballUD] = 0;
	
	ballSetings[3][ballX] = 140;
	ballSetings[3][ballY] = 70;
	ballSetings[3][ballRadious] = maxBallSize/2;
	ballSetings[3][ballRL] = 1;
	ballSetings[3][ballUD] = 0;
	
	ballSetings[4][ballX] = 170;
	ballSetings[4][ballY] = 70;
	ballSetings[4][ballRadious] = maxBallSize/2;
	ballSetings[4][ballRL] = 1;
	ballSetings[4][ballUD] = 0;
	
	ballSetings[5][ballX] = 200;
	ballSetings[5][ballY] = 70;
	ballSetings[5][ballRadious] = maxBallSize/2;
	ballSetings[5][ballRL] = 1;
	ballSetings[5][ballUD] = 0;

	
	playerPosition=270;
}
void setupBallsLvl4(){
	memset(ballSetings, 0, sizeof(ballSetings));
	
	ballSetings[0][ballX] = 80;
	ballSetings[0][ballY] = 20;
	ballSetings[0][ballRadious] = maxBallSize;
	ballSetings[0][ballRL] = -1;
	ballSetings[0][ballUD] = 0;

	ballSetings[1][ballX] = 120;
	ballSetings[1][ballY] = 70;
	ballSetings[1][ballRadious] = maxBallSize/2;
	ballSetings[1][ballRL] = -1;
	ballSetings[1][ballUD] = 0;
	
	ballSetings[2][ballX] = 200;
	ballSetings[2][ballY] = 70;
	ballSetings[2][ballRadious] = maxBallSize/2;
	ballSetings[2][ballRL] = 1;
	ballSetings[2][ballUD] = 0;
	
	ballSetings[3][ballX] = 240;
	ballSetings[3][ballY] = 20;
	ballSetings[3][ballRadious] = maxBallSize;
	ballSetings[3][ballRL] = 1;
	ballSetings[3][ballUD] = 0;
	
	playerPosition=160;
}



int isWinGame(){

	int i = 0;
	for (i = 0; i < 32; i++) {
		if (ballSetings[i][ballX] > 0) {
		return 0;
		}
	}
	return 1;
}

void deleteBalls(){

	int i = 0;
	for (i = 0; i < 32; i++) {
		if (ballSetings[i][ballX] > 0) {
			deleteObj(ballSetings[i][ballX]- ballSetings[i][ballRadious], ballSetings[i][ballY]- ballSetings[i][ballRadious],2* ballSetings[i][ballRadious],2* ballSetings[i][ballRadious]);
		}
	}

}
void deletePlayer(){
	deleteObj(playerPosition-10,playerPositionY-10,23,37);
}
void deleteFire(){
	deleteObj(fireX-5,fireY,10,200-fireY);
}

void drawBalls() {
	int i = 0;
	for (i = 0; i < 32; i++) {
		if (ballSetings[i][ballX] > 0) {
			drawBall(ballSetings[i][ballX], ballSetings[i][ballY], ballSetings[i][ballRadious]);
		}
	}
}

void updateBalls() {
	int i=0;
	for (i = 0; i < 32; i++) {
		if (ballSetings[i][ballX] > 0) {

			ballSetings[i][ballUD] += 35;
			ballSetings[i][ballX] += ballSetings[i][ballRL];
			ballSetings[i][ballY] += ballSetings[i][ballUD]/100;


			if(ballSetings[i][ballX]-ballSetings[i][ballRadious]<11 && ballSetings[i][ballRL]<0) {
				ballSetings[i][ballX] = 10+ ballSetings[i][ballRadious];
				ballSetings[i][ballRL] = -1*ballSetings[i][ballRL];
			}
			if (ballSetings[i][ballX]+ballSetings[i][ballRadious] > 309 && ballSetings[i][ballRL]>0) {
				ballSetings[i][ballX] = 309- ballSetings[i][ballRadious];
				ballSetings[i][ballRL] = -1* ballSetings[i][ballRL];
			}

			if (ballSetings[i][ballY]+ ballSetings[i][ballRadious] > 200 && ballSetings[i][ballUD]>0) {
				ballSetings[i][ballY] = 200- ballSetings[i][ballRadious];
				ballSetings[i][ballUD] = -1 * ballSetings[i][ballUD]*jumpCoef;
			}
			}
	}
}

void updatePlayer() {
	//switch case yazýlacak
	if(directionOst>0){
	directionOst--;}
	if(directionOst==0){
		playerDir=0;
	}

	if(byte3==116 || byte3==35){
		
		if(playerPosition<310){
			playerPosition=playerPosition+6;
			playerDir=1;
			directionOst=6;
		}

		byte3 = 0;
	}else if(byte3==107){
		if(playerPosition>10){
		playerPosition = playerPosition - 6;
			playerDir=-1;
			directionOst=6;
		}
		byte3 = 0;
	}
}

void updateFire() {
	
	if (isFireOn) {
		fireY-=5;
		if (fireY < 10) {
			isFireOn = 0;
			deleteFire();
		}
	}
	else if (byte3 == 41) {
		isFireOn = 1;
		fireX = playerPosition;
		fireY = 160;
		byte3 = 0;
	}
	
}
void checkAction() {
	if (isFireOn) {
		int ballDist = 0;
		int i = 0;
		for (i = 0; i < 32; i++) {
			if (ballSetings[i][ballX] > 0) {
				ballDist = ballSetings[i][ballX] - fireX;
				if (ballDist < 0)
					ballDist = -1*ballDist;
				if (ballDist< (ballSetings[i][ballRadious] + 2)) {
					if (ballSetings[i][ballY] + ballSetings[i][ballRadious] > fireY) {
						isFireOn = 0;
						ballHit(i);
						activateVoice();
						return;
					}
				}
			}
		}
	}
}
void activateVoice(){
		volatile int * audio_ptr= (int*) AUDIO_BASE;
	// reset counter to start playback
		buffer_index = 0;
		// clear audio-out FIFO
		*(audio_ptr) = 0x8;
		// turn off clear, and enable audio-out interrupts
		*(audio_ptr) = 0x2;
}

void ballHit(int index) {
	//first shirnk the hit ball
	if (ballSetings[index][ballRadious] == 4) {
		ballSetings[index][ballRadious]=0;
		ballSetings[index][ballX]=0;
		ballSetings[index][ballY]=0;
		ballSetings[index][ballRL]=0;
		ballSetings[index][ballUD]=0;
	}
	else {
		ballSetings[index][ballRadious] = ballSetings[index][ballRadious]/2;
		ballSetings[index][ballUD] = -1*ballSetings[index][ballUD];
		// find a place for new ball
		int i = -1;
		int done = 1;
		while (done &&i<31)
		{
			i++;
			if (ballSetings[i][ballX]<1) {
				done = 0;
			}
		}
		ballSetings[i][ballRadious] = ballSetings[index][ballRadious];
		ballSetings[i][ballX] = ballSetings[index][ballX];
		ballSetings[i][ballY] = ballSetings[index][ballY];
		ballSetings[i][ballRL] = -1* ballSetings[index][ballRL];
		ballSetings[i][ballUD] = ballSetings[index][ballUD];
	}
	deleteFire();

}



int checkEndGame() {
	int i = 0;
	int xDistance = 0;
	int yDistance = 0;
	for (i = 0; i < 32; i++) {
		if (ballSetings[i][ballX] > 0) {
			xDistance = ballSetings[i][ballX] - playerPosition;
			yDistance = ballSetings[i][ballY]- playerPositionY-10;

			if (xDistance*xDistance+yDistance*yDistance < (ballSetings[i][ballRadious]+13) * (ballSetings[i][ballRadious]+13))
				return 1;
		}
	}
	return 0;
}
void config_PS2()
{
	volatile int * PS2_ptr = (int *)PS2_BASE;	// PS/2 port address

	*(PS2_ptr) = 0xFF;				/* reset */
	*(PS2_ptr + 1) = 0x1; 			/* write to the PS/2 Control register to enable interrupts */
}

/* setup private timer in the ARM A9 */
void config_MPcore_private_timer()
{
	volatile int * MPcore_private_timer_ptr = (int *)MPCORE_PRIV_TIMER;	// timer base address

																		/* set the timer period */
	int counter = 8000000;							// period = 1/(200 MHz) x 40x10^6 = 0.2 sec
	*(MPcore_private_timer_ptr) = counter;		// write to timer load register

												/* write to control register to start timer, with interrupts */
	*(MPcore_private_timer_ptr + 2) = 0x7;		// int mask = 1, mode = 1, enable = 1
}