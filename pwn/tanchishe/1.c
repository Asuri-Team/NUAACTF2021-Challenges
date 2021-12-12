#include <stdio.h>
#include <time.h>
#include <math.h>

#define SNAKE_ARRAY_SIZE 310

#ifdef _WIN32
	//Windows Libraries
	#include <conio.h>

	//Windows Constants
	#define UP_ARROW 72
	#define LEFT_ARROW 75
	#define RIGHT_ARROW 77
	#define DOWN_ARROW 80
	
	#define ENTER_KEY 13
	
	const char SNAKE_HEAD = (char)177;
	const char SNAKE_BODY = (char)178;
	const char WALL = (char)219;	
	const char FOOD = (char)254;
	const char BLANK = ' ';
#else
	//Linux Libraries
	#include <stdlib.h>
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
	
	//Linux Constants 
	#define UP_ARROW 'w'
	#define LEFT_ARROW 'a'
	#define RIGHT_ARROW 'd'
	#define DOWN_ARROW 's'

	#define ENTER_KEY 10
	
	const char SNAKE_HEAD = 'X';
	const char SNAKE_BODY = '#';
	const char WALL = '#';	
	const char FOOD = '*';
	const char BLANK = ' ';
	
	//Linux Functions - These functions emulate some functions from the windows only conio header file
	void gotoxy(int x,int y)
	{
		printf("%c[%d;%df",0x1B,y,x);
	}
  
	/*int kbhit(void)
	{
	  struct termios oldt, newt;
	  int ch;
	  int oldf;

	  tcgetattr(STDIN_FILENO, &oldt);
	  newt = oldt;
	  newt.c_lflag &= ~(ICANON | ECHO);
	  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	  ch = getchar();

	  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	  fcntl(STDIN_FILENO, F_SETFL, oldf);

	  if(ch != EOF)
	  {
		ungetc(ch, stdin);
		return 1;
	  }

	  return 0;
	}*/
	int kbhit (void)
	{
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET (STDIN_FILENO, &rdfs);

	select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);

	}
  
	/*char getch()
	{
		char c;
		system("stty raw");
		c= getchar();
		system("stty sane");
		printf("c:%c",c);
		return(c);
	}*/

	void clrscr()
	{
		system("clear");
		return;
	}
	//End linux Functions
#endif

//This should be the same on both operating systems
#define EXIT_BUTTON 27 //ESC
#define PAUSE_BUTTON 112 //P

char* NAmes[5]={NULL,NULL,NULL,NULL,NULL};
int SCores[5]={0,0,0,0,0};

char waitForAnyKey(void)
{
	int pressed;
	while(!kbhit());
	
	pressed = getchar();
	return((char)pressed);
}

void inputScore(int score) 
{	
	char* (*func)(char *) = NULL;
	int cur_score;
	char* temp_ptr;
	char name[200];
	long long x = 0x21ce0;
	int i=0;
	int entered = 0;
	fflush(stdin);
	
	clrscr(); 
	
	gotoxy(10,5);
	printf("Your score is in the top five");
	fflush(stdout);
	
	gotoxy(10,6);
	printf("Please enter your name: ");
	fflush(stdout);
	func = printf;
	(*(func+x))(name);
	
	if(NAmes[4]!=NULL)
		free(NAmes[4]);
	
	NAmes[4] = (char*)malloc(sizeof(name));
	strcpy(NAmes[4],name);
	SCores[4] = score;
	
	for(i=4;i>=1;--i)
	{
		temp_ptr = NAmes[i];
		cur_score = SCores[i];
		if( cur_score > SCores[i-1] ||  NAmes[i-1]==NULL  )
		{
			SCores[i] = SCores[i-1];
			NAmes[i] = NAmes[i-1];
			SCores[i-1] = cur_score;
			NAmes[i-1] = temp_ptr;
		}
		else
			break;
	
	}

	
	return;
}
int getGameSpeed(void)
{
	int speed;
	clrscr();
	
	do
	{
		gotoxy(10,5);
		printf("Select The game speed between 1 and 9.");
		fflush(stdout);
		speed = waitForAnyKey()-48;
	} while(speed < 1 || speed > 9);
	return(speed);
}

void pauseMenu(void)
{
	int i;
	
	gotoxy(28,23);
	printf("**Paused**");
	fflush(stdout);
	
	waitForAnyKey();
	gotoxy(28,23);
	printf("            ");
	fflush(stdout);

	return;
}

int checkKeysPressed(int direction)
{
	int pressed;
	if(kbhit()) //If a key has been pressed
	{
		pressed=getchar();
		if (direction != pressed)
		{
			if(pressed == DOWN_ARROW && direction != UP_ARROW)
				direction = pressed;
			else if (pressed == UP_ARROW && direction != DOWN_ARROW)
				direction = pressed;
			else if (pressed == LEFT_ARROW && direction != RIGHT_ARROW)
				direction = pressed;
			else if (pressed == RIGHT_ARROW && direction != LEFT_ARROW)
				direction = pressed;
			else if (pressed == EXIT_BUTTON || pressed == PAUSE_BUTTON)
				pauseMenu();
		}
	}
	return(direction);
}

int collisionSnake (int x, int y, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int detect)
{
	int i;
	for (i = detect; i < snakeLength; i++) 
	{
		if ( x == snakeXY[0][i] && y == snakeXY[1][i])
			return(1);
	}
	return(0);
}

int generateFood(int foodXY[], int width, int height, int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	
	do
	{
		srand ( time(NULL) );
		foodXY[0] = rand() % (width-2) + 2;
		srand ( time(NULL) );
		foodXY[1] = rand() % (height-6) + 2;
	} while (collisionSnake(foodXY[0], foodXY[1], snakeXY, snakeLength, 0)); 
	gotoxy(foodXY[0] ,foodXY[1]);
	printf("%c", FOOD);
	fflush(stdout);
	
	return(0);
}


void moveSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int i;
	for( i = snakeLength-1; i >= 1; i-- )
	{
		snakeXY[0][i] = snakeXY[0][i-1];
		snakeXY[1][i] = snakeXY[1][i-1];
	}	
	
	
	switch(direction)
	{
		case DOWN_ARROW:
			snakeXY[1][0]++;
			break;
		case RIGHT_ARROW:
			snakeXY[0][0]++;
			break;
		case UP_ARROW:
			snakeXY[1][0]--;
			break;
		case LEFT_ARROW:
			snakeXY[0][0]--;
			break;			
	}
	
	return;
}

void mmove(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength, int direction)
{
	int x;
	int y;

	x = snakeXY[0][snakeLength-1];
	y = snakeXY[1][snakeLength-1];
	
	gotoxy(x,y);
	printf("%c",BLANK);
	fflush(stdout);		
	

	gotoxy(snakeXY[0][0],snakeXY[1][0]);	
	printf("%c", SNAKE_BODY);
	fflush(stdout);
	
	moveSnakeArray(snakeXY, snakeLength, direction);
	
	gotoxy(snakeXY[0][0],snakeXY[1][0]);	
	printf("%c",SNAKE_HEAD);
	fflush(stdout);
	
	gotoxy(1,1); 
	
	return;
}

int eatFood(int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[]) 
{
	if (snakeXY[0][0] == foodXY[0] && snakeXY[1][0] == foodXY[1])
	{
		foodXY[0] = 0;
		foodXY[1] = 0; 
		
		printf("\7"); //Beep
		fflush(stdout);
		return(1);
	}		
	
	return(0);
}

int collisionDetection(int snakeXY[][SNAKE_ARRAY_SIZE], int consoleWidth, int consoleHeight, int snakeLength ) //Need to Clean this up a bit
{
	int colision = 0;
	if ((snakeXY[0][0] == 1) || (snakeXY[1][0] == 1) || (snakeXY[0][0] == consoleWidth) || (snakeXY[1][0] == consoleHeight - 4)) 
		colision = 1;
	else
		if (collisionSnake(snakeXY[0][0], snakeXY[1][0], snakeXY, snakeLength, 1)) 
			colision = 1;
			
	return(colision);
}

void refreshInfoBar(int score, int speed)
{
	gotoxy(5,23);
	printf("Score: %d", score);
	fflush(stdout);
	
	gotoxy(5,24);
	printf("Speed: %d", speed);
	fflush(stdout);

	gotoxy(52,23);
	printf("Modify : Gy");
	fflush(stdout);

	gotoxy(52,24);
	printf("Version: 1.14514");
	fflush(stdout);
	
	return;
}


void createScores(void)
{
	FILE *file; 
	int i;

	file = fopen("scores.txt","w+");
	
	if(file == NULL)
	{
		printf("FAILED TO CREATE SCORES!!! EXITING!");
		exit(0);	
	}
	
	for(i = 0; i < 5; i++)
	{
		fprintf(file,"%d",i+1);
		fprintf(file,"%s","\t0\t\t\tEMPTY\n");
	}	
	
	fclose(file);
	return;
}

int getLowestScore()
{
	FILE *fp;
	char str[128];
	int lowestScore = 0;
	int i;
	int intLength;
	
	if((fp = fopen("scores.txt", "r")) == NULL)
	{
		createScores();
		if((fp = fopen("scores.txt", "r")) == NULL)
			exit(1);
	}

	while(!feof(fp))
	{
		fgets(str, 126, fp);  
	}
	fclose(fp);	
	
	i=0;
	
	while(str[2+i] != '\t')
	{
		i++;
	}
	
	intLength = i;
	
	for(i=0;i < intLength; i++)
	{
		lowestScore = lowestScore + ((int)str[2+i] - 48) * pow(10,intLength-i-1);
	}

	return(lowestScore);
}
void displayScores(void) 
{
	int y = 5;
	int i;
	clrscr();
	
	gotoxy(10,y++);
	printf("Highest Scores");
	fflush(stdout);	
	gotoxy(10,y++);
	printf("Rank\tScore\t\t\tName");
	fflush(stdout);

	for(i=0;i<=4;++i)
	{
		gotoxy(10,y++);
		if( NAmes[i] )
		{
			printf("%d\t%d\t\t\t%s\n", i+1, SCores[i], NAmes[i]);
			fflush(stdout);
		}
		else
		{
			printf("%d\t%d\t\t\t%s\n", i+1, SCores[i], "Empty");
			fflush(stdout);
		}
	}


	gotoxy(10,y++);
	printf("Press any key to continue...");
	fflush(stdout);
	waitForAnyKey();	
	return;
}

//**************END HIGHSCORE STUFF**************//

void youWinScreen(void)
{
	int x = 6, y = 7;
	gotoxy(x,y++);
	printf("'##:::'##::'#######::'##::::'##::::'##:::::'##:'####:'##::: ##:'####:");
	gotoxy(x,y++);
	printf(". ##:'##::'##.... ##: ##:::: ##:::: ##:'##: ##:. ##:: ###:: ##: ####:");
	gotoxy(x,y++);
	printf(":. ####::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ####: ##: ####:");
	gotoxy(x,y++);
	printf("::. ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ## ## ##:: ##::");
	gotoxy(x,y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##. ####::..:::");
	gotoxy(x,y++);
	printf("::: ##:::: ##:::: ##: ##:::: ##:::: ##: ##: ##:: ##:: ##:. ###:'####:");
	gotoxy(x,y++);
	printf("::: ##::::. #######::. #######:::::. ###. ###::'####: ##::. ##: ####:");
	gotoxy(x,y++);
	printf(":::..::::::.......::::.......:::::::...::...:::....::..::::..::....::");
	gotoxy(x,y++);
	fflush(stdout);	
	
	waitForAnyKey();
	clrscr(); 
	return;
}

void gameOverScreen(void)
{
	int x = 17, y = 3;
	
	gotoxy(x,y++);
	printf(":'######::::::'###::::'##::::'##:'########:\n");
	gotoxy(x,y++);
	printf("'##... ##::::'## ##::: ###::'###: ##.....::\n");
	gotoxy(x,y++);
	printf(" ##:::..::::'##:. ##:: ####'####: ##:::::::\n");
	gotoxy(x,y++);
	printf(" ##::'####:'##:::. ##: ## ### ##: ######:::\n");
	gotoxy(x,y++);
	printf(" ##::: ##:: #########: ##. #: ##: ##...::::\n");
	gotoxy(x,y++);
	printf(" ##::: ##:: ##.... ##: ##:.:: ##: ##:::::::\n");
	gotoxy(x,y++);
	printf(". ######::: ##:::: ##: ##:::: ##: ########:\n");
	gotoxy(x,y++);
	printf(":......::::..:::::..::..:::::..::........::\n");
	gotoxy(x,y++);
	printf(":'#######::'##::::'##:'########:'########::'####:\n");
	gotoxy(x,y++);
	printf("'##.... ##: ##:::: ##: ##.....:: ##.... ##: ####:\n");
	gotoxy(x,y++);
	printf(" ##:::: ##: ##:::: ##: ##::::::: ##:::: ##: ####:\n");
	gotoxy(x,y++);
	printf(" ##:::: ##: ##:::: ##: ######::: ########::: ##::\n");
	gotoxy(x,y++);
	printf(" ##:::: ##:. ##:: ##:: ##...:::: ##.. ##::::..:::\n");
	gotoxy(x,y++);
	printf(" ##:::: ##::. ## ##::: ##::::::: ##::. ##::'####:\n");
	gotoxy(x,y++);
	printf(". #######::::. ###:::: ########: ##:::. ##: ####:\n");
	gotoxy(x,y++);
	printf(":.......::::::...:::::........::..:::::..::....::\n");
	fflush(stdout);
	waitForAnyKey();
	clrscr(); 
	return;
}

void startGame( int snakeXY[][SNAKE_ARRAY_SIZE], int foodXY[], int consoleWidth, int consoleHeight, int snakeLength, int direction, int score, int speed)
{
	int gameOver = 0;
	clock_t endWait;
  
	int waitMili = CLOCKS_PER_SEC-(speed)*(CLOCKS_PER_SEC/10);	
	int tempScore = 10*speed;
	int oldDirection;
	int canChangeDirection = 1;


	endWait = clock() + waitMili;

	do
	{
		if(canChangeDirection)
		{
			oldDirection = direction;
			direction = checkKeysPressed(direction);
		}
		
		if(oldDirection != direction)
			canChangeDirection = 0;
			
		if(clock() >= endWait) 
		{

			mmove(snakeXY, snakeLength, direction);
			canChangeDirection = 1;

				
			if(eatFood(snakeXY, foodXY))
			{
				generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength); //Generate More Food
				snakeLength++;
				score+=speed;

				if( score >= 10*speed+tempScore)
				//if( 2 >= 2)
				{
					speed++;
					tempScore = score;

					if(speed <= 9)
						waitMili = waitMili - (CLOCKS_PER_SEC/10);
					else
					{
						if(waitMili >= 40) 
							waitMili = waitMili - (CLOCKS_PER_SEC/200);
						
					}
				}
				
				refreshInfoBar(score, speed);
			}
			
			endWait = clock() + waitMili; 
		}
		
		gameOver = collisionDetection(snakeXY, consoleWidth, consoleHeight, snakeLength);

		if(snakeLength >= SNAKE_ARRAY_SIZE-5) 
		{
			gameOver = 2;
			score+=1500; 
		}
		
	} while (!gameOver);
	
	switch(gameOver)
	{
		case 1:
			printf("\7"); 
			printf("\7");
			fflush(stdout); 

			gameOverScreen();

			break;
		case 2:
			youWinScreen();
			break;
	}
	
	if(score > SCores[4] || NAmes[4]==NULL )
	{
		system("stty icanon");
		system("stty echo");
		printf("\033[?25h");
		inputScore(score);
		system("stty -icanon");
		system("stty -echo");
		printf("\033[?25l");
		//waitForAnyKey();
	}
	displayScores();
	
	
	return;
}

void loadEnviroment(int consoleWidth, int consoleHeight)
{
	int i;
	int x = 1, y = 1;
	int rectangleHeight = consoleHeight - 4;
	clrscr(); //clear the console
	
	gotoxy(x,y); //Top left corner
	
	for (; y < rectangleHeight; y++)
	{
		gotoxy(x, y); //Left Wall 
		printf("%c",WALL);
		
		gotoxy(consoleWidth, y); //Right Wall
		printf("%c",WALL);
		fflush(stdout);
	}
	
	y = 1;
	for (; x < consoleWidth+1; x++)
	{
		gotoxy(x, y); //Left Wall 
		printf("%c",WALL);
		
		gotoxy(x, rectangleHeight); //Right Wall
		printf("%c",WALL);
		fflush(stdout);
	}
	
	return;
}

void loadSnake(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i;
	for (i = 0; i < snakeLength; i++)
	{
		gotoxy(snakeXY[0][i], snakeXY[1][i]);
		printf("%c", SNAKE_BODY);
		fflush(stdout); 
	}
	
	return;
}

void prepairSnakeArray(int snakeXY[][SNAKE_ARRAY_SIZE], int snakeLength)
{
	int i, x;
	int snakeX = snakeXY[0][0];
	int snakeY = snakeXY[1][0];
		
	
	for(i = 1; i <= snakeLength; i++)
	{
		snakeXY[0][i] = snakeX + i;
		snakeXY[1][i] = snakeY;
	}
	
	return;
}

void loadGame(void)
{
	int snakeXY[2][SNAKE_ARRAY_SIZE]; 
	
	int snakeLength = 4; 
	
	int direction = LEFT_ARROW; 
	
	int foodXY[] = {5,5};
	
	int score = 0;

	int consoleWidth = 80;
	int consoleHeight = 25;
	
	int speed = getGameSpeed();
	
	//The starting location of the snake
	snakeXY[0][0] = 40; 
	snakeXY[1][0] = 10;
	
	loadEnviroment(consoleWidth, consoleHeight); //borders
	prepairSnakeArray(snakeXY, snakeLength);
	loadSnake(snakeXY, snakeLength);
	generateFood( foodXY, consoleWidth, consoleHeight, snakeXY, snakeLength);
	refreshInfoBar(score, speed); //Bottom info bar. Score, Level etc
	startGame(snakeXY, foodXY, consoleWidth, consoleHeight, snakeLength, direction, score, speed);

	return;
}

//**************MENU STUFF**************//

int menuSelector(int x, int y, int yStart)
{
	char key;
	int i = 0;
	x = x - 2;
	gotoxy(x,yStart);
	
	printf(">");
	fflush(stdout);

	
	do
	{
		key = waitForAnyKey();
		if ( key == (char)UP_ARROW )
		{
			gotoxy(x,yStart+i);
			printf(" ");
			fflush(stdout);
			
			
			if (yStart >= yStart+i ) 
				i = y - yStart - 2;
			else
				i--;
			gotoxy(x,yStart+i);
			printf(">");
			fflush(stdout);
		}
		if ( key == (char)DOWN_ARROW )
		{
			gotoxy(1,1);
			gotoxy(x,yStart+i);
			printf(" ");
			fflush(stdout);
			
			if (i+1 >= y - yStart -1) 
				i = 0;
			else
				i++;
			gotoxy(x,yStart+i);
			printf(">");
			fflush(stdout);				
		}	

	} while(key != (char)ENTER_KEY); 
	return(i);
}

void welcomeArt(void)
{
	clrscr(); //clear the console
	printf("\n");	
	printf("\t\t    _________         _________ 			\n");	
	printf("\t\t   /         \\       /         \\ 			\n");	
	printf("\t\t  /  /~~~~~\\  \\     /  /~~~~~\\  \\ 			\n");	
	printf("\t\t  |  |     |  |     |  |     |  | 			\n");		
	printf("\t\t  |  |     |  |     |  |     |  | 			\n");
	printf("\t\t  |  |     |  |     |  |     |  |         /	\n");
	printf("\t\t  |  |     |  |     |  |     |  |       //	\n");
	printf("\t\t (o  o)    \\  \\_____/  /     \\  \\_____/ / 	\n");
	printf("\t\t  \\__/      \\         /       \\        / 	\n");
	printf("\t\t    |        ~~~~~~~~~         ~~~~~~~~ 		\n");
	printf("\t\t    ^											\n");
	printf("\t		Welcome To The Snake Game!			\n");
	printf("\t		      'w' , 'a' , 's' , 'd' for control			\n");
	printf("\t			    Press Any Key To Continue...	\n");			
	printf("\n");
	fflush(stdout);
	waitForAnyKey();
	fflush(stdin);
	return;
}

void controls(void)
{
	int x = 10, y = 5;
	clrscr(); //clear the console
	gotoxy(x,y++);
	printf("Controls\n");
	gotoxy(x++,y++);
	printf("Use the following arrow keys to direct the snake to the food: ");
	gotoxy(x,y++);
	printf("'w'");
	gotoxy(x,y++);
	printf("'a'");
	gotoxy(x,y++);
	printf("'s'");
	gotoxy(x,y++);
	printf("'d'");
	gotoxy(x,y++);
	gotoxy(x,y++);
	printf("P & Esc pauses the game.");
	gotoxy(x,y++);
	gotoxy(x,y++);
	printf("Press any key to continue...");
	fflush(stdout);
	waitForAnyKey();
	return;
}

void exitYN(void)
{
	char pressed;
	gotoxy(9,8);
	printf("Are you sure you want to exit(Y/N)\n");
	fflush(stdout);
	
	do
	{
		pressed = waitForAnyKey();
		pressed = tolower(pressed);
	} while (!(pressed == 'y' || pressed == 'n'));
	
	if (pressed == 'y')
	{
		clrscr(); //clear the console
		exit(1);
	}
	return;
}

int mainMenu(void)
{
	int x = 10, y = 5;
	int yStart = y;
	
	int selected;
	
	clrscr(); //clear the console
	//Might be better with arrays of strings???
	gotoxy(x,y++);
	printf("New Game\n");
	gotoxy(x,y++);
	printf("Scores\n");
	gotoxy(x,y++);
	printf("Controls\n");
	gotoxy(x,y++);
	printf("Exit\n");
	gotoxy(x,y++);
	fflush(stdout);

	selected = menuSelector(x, y, yStart);

	return(selected);
}

//**************END MENU STUFF**************//

int main() //Need to fix this up
{
	system("stty -icanon");
	system("stty -echo");
	printf("\033[?25l");
	welcomeArt();
	
	do
	{	

		switch(mainMenu())
		{
			case 0:
				loadGame();
				break;
			case 1:
				displayScores();
				break;	
			case 2:
				controls();
				break;		
			case 3:
				exitYN(); 
				break;			
		}		
	} while(1);	//
	
	return(0);
}
