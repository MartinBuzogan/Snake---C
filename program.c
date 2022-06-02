#define _POSIX_C_SOURCE 200201L
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <time.h>

typedef struct {
	float starting;
	float main;
	float slowed;
}posible_time;
struct timespec ts = {
    .tv_sec = 0,   	                // nr of secs
    .tv_nsec = 0.125 * 1000000000L  // nr of nanosecs
};

void read_score(int bfile[]){
	FILE* fpr = fopen("score.txt", "r");
    if(fpr == NULL){
        fprintf(stderr, "Error opening file.\n");
    }else{
		fscanf(fpr,"%d %d %d", &bfile[2], &bfile[0], &bfile[1]);
		}
    fclose(fpr);
}

void print_in_midd(int x,int y,char *text){
	mvprintw(x,(y/2)-strlen(text)/2,"%s",text);
}
void erasing(int mxy[],int value){
	if(!(mxy[0] == mxy[2] && mxy[1] == mxy[3])){
		mxy[2]=mxy[0];
		mxy[3]=mxy[1];
		erase();
	}
	if (value == 1){
		erase();
	}
}
int welcmenu(int game_run,int mxy[]){ 
	char name[][255]={"                  _       ", 
					  "	                | |      ",
					  "	 ___ _ __   __ _| | _____", 
					  "	/ __| '_ \\ / _` | |/ / _ \\",
					  "	\\__ \\ | | | (_| |   <  __/",
					  "	|___/_| |_|\\__,_|_|\\_\\___|"};
	//char premenne
	char *underline = "_______________________________________";
	char *menu[] = {" -- Play -- ","-- Controls --"," -- Quit -- "};
	//základne premenné 
	int user_choice= 0;
	int user_choice_index = 0;
	int play = 0;
	int bfile[3];
	read_score(bfile);

	while(game_run){
		getmaxyx(stdscr,mxy[0],mxy[1]);  //ukladá rozmery okna do poľa mxy
		nodelay(stdscr,1);				//zabezpečí, aby sa znaky po stlačení a načítaní funkciou getch() nevypisovali na obrazovku
		int space = 0;					// premmenná zabezpečujuca vyzualnu medzeru medzi vykreslovaním moznosťi 

		box(stdscr,0,0);				//vykresľuje okraje okna
		// Printing Welcome
		print_in_midd(1,mxy[1],underline);
		for(int i = 0 ; i <6;i++ ){
			if (i==0){
				attron(COLOR_PAIR(2));
			}else
				attron(COLOR_PAIR(i+1));
			print_in_midd(2+i,mxy[1],name[i]);}
		attron(COLOR_PAIR(1));
		print_in_midd(10,mxy[1],"Created by Martin Buzogan");
		mvprintw(12,mxy[1]/2-12,"Last score : %d Time: %d:%d",bfile[2],bfile[0],bfile[1]);
		print_in_midd(13,mxy[1],underline);
			
		user_choice = getch();
		for (int i = 0;i< 3 ;i++){
			if(i == user_choice_index){
				wattron(stdscr,A_REVERSE);
			}
			if (i == 1 || i ==2 )
				space+=1;
			print_in_midd(i+15+space,mxy[1],menu[i]);
			wattroff(stdscr,A_REVERSE);
		}
		switch(user_choice){
			case KEY_DOWN:
				user_choice_index++;
				if (user_choice_index == 3)
					user_choice_index = 0;
				break;
			case KEY_UP:
				user_choice_index--;
				if (user_choice_index == -1)
					user_choice_index = 2;
				break;
			default:
				break;
		}
		if (user_choice == 10){
			if (user_choice_index == 2){
				play = 0;
				break;}
			if (user_choice_index == 0){
				play = 1;
				break;}
			if (user_choice_index == 1){
				print_in_midd(24,mxy[1],"Arrows - movement");
				print_in_midd(26,mxy[1],"Space - slow    M - remove border");
			}
		}
		erasing(mxy,0);
		
		refresh();
	}
	return play;
}

void gen_x_y(int xy[],int mxy[]){
	xy[0] = rand() % (mxy[0]-2) +1;
	while(xy[0]>=mxy[0]){
		xy[0]-=2;
	}
	xy[1] = rand() % (mxy[1]-2) +1;
	while(xy[1]>=mxy[1]){
		xy[1]-=2;
	}
}

void score_power_up(int lenght,int mxy[],int colected,int col_power,int power,int stime[]){
	if (mxy[1]>110){
		mvprintw(mxy[0],2,"Your lenght is : %d",lenght-3);
		(lenght >= 12 && power == 1)? mvprintw(mxy[0],40,"Border has been activated"):mvprintw(mxy[0],40,"Border has been deactivated");
		mvprintw(mxy[0],25,"Time %d:%d ",stime[2],stime[1]);
		attron(COLOR_PAIR(5));
		mvprintw(mxy[0],70,"Your slowdowns: %d",colected);
		attron(COLOR_PAIR(3));
		mvprintw(mxy[0],90,"Your power_ups: %d",col_power);
		attron(COLOR_PAIR(1));}
	else if (mxy[1]>70){
		mvprintw(mxy[0],2,"Lenght is : %d",lenght-3);
		mvprintw(mxy[0],20,"Time %d:%d ",stime[2],stime[1]);
		(lenght >= 12 && power == 1)? mvprintw(mxy[0],30,"Border activated"):mvprintw(mxy[0],30,"Border deactivated");
		attron(COLOR_PAIR(5));
		mvprintw(mxy[0],50,"Slow: %d",colected);
		attron(COLOR_PAIR(3));
		mvprintw(mxy[0],60,"power_ups: %d",col_power);
		attron(COLOR_PAIR(1));
	}else {
		mvprintw(mxy[0],2,"Lenght: %d",lenght-3);
		mvprintw(mxy[0],15,"Time %d:%d ",stime[2],stime[1]);
		(lenght >= 12 && power == 1)? mvprintw(mxy[0],30,"B. activated"):mvprintw(mxy[0],30,"B. deactivated");
		attron(COLOR_PAIR(5));
		mvprintw(mxy[0],50,"S.: %d",colected);
		attron(COLOR_PAIR(3));
		mvprintw(mxy[0],60,"Pow_ups: %d",col_power);
		attron(COLOR_PAIR(1));
	}
}
int checking(int head[],int pos_x_y[],int mxy[],char char_look,int color,float snaketime){
	if (head[0] == pos_x_y[0] && head[1] == pos_x_y[1]){
			ts.tv_nsec = snaketime * 1000000000L;
			mvprintw(pos_x_y[0],pos_x_y[1]," ");
			gen_x_y(pos_x_y,mxy);
			return 1;
		}else {
			attron(COLOR_PAIR(color));
			mvprintw(pos_x_y[0],pos_x_y[1],"%c",char_look);
			attron(COLOR_PAIR(1));
			return 0;
		}
}
void score_to_file(int lenght,int stime[]){
	FILE* fpr = fopen("score.txt", "w");
    char buffer[255];
    if(fpr == NULL){
        fprintf(stderr, "Error opening file.\n");
    }else{
		sprintf(buffer, "%d %d %d", lenght,stime[2],stime[1]);
		fputs(buffer,fpr);}
    fclose(fpr);
}

int game(int game_run,int mxy[]){
	nodelay(stdscr,1);
	for (int i = 0;i<mxy[1];i++)
		mvprintw(mxy[0]-3,i,"-");
	mxy[0]-=2;
	//struktura na menenie času 
	posible_time snaketime;
	snaketime.starting = 0.125;
	snaketime.main = snaketime.starting;
	snaketime.slowed = 0.225;
	// deklarovanie hada
	int head[2] = {mxy[0]/2,mxy[1]/2};                  
    int body[500][2] = {0};
    body[0][0] = mxy[0]/2;
    body[0][1] = mxy[1]/2;
    int tail[2] = {(mxy[0]/2) + 2,mxy[1]/2};
	int movement[] = {1,0};
	int lenght = 3;
	
	int user_choice;
	//snake look
	int snake_look[4] = {'<','>','U','^'};

	//food
	int food_x_y[2];
	char food_char = *"*";
	//slow
	int colected = 0;
	int slow_x_y[2];
	char slow_char = *"@";
	// power
	int power = 1;
	int power_x_y[2];
	int col_power = 0;
	char power_char = *"#";

	int dead_menu = 0;
	int erase_once = 1;
	//generovanie prvých powerupov
	gen_x_y(slow_x_y,mxy);
	gen_x_y(food_x_y,mxy);
	gen_x_y(power_x_y,mxy);
	// počitanie času 
	int stime[]={0,0,0};
	int speed_bool=0;

	while(game_run){
		srand(time(NULL));
		user_choice = getch();
		curs_set(0);
		
		// vypočet času
		if (stime[0] >= ((int)(1000/(snaketime.main*1000)))){
			stime[1]++;
			stime[0]=0;
			speed_bool=1;
		}
		if ((stime[1]==30 || stime[1]==60) && speed_bool){
			if (snaketime.main > 0.07){
				snaketime.main-=0.01;
				ts.tv_nsec = snaketime.main * 1000000000L;
				speed_bool = 0;
			}
		}
		if(stime[1]>=60){
			stime[1]=0;
			stime[2]++;
		}
		stime[0]++;
		
		// food
		if (checking(head,food_x_y,mxy,food_char,4,snaketime.main)){
			lenght+=2;
			power = 1;
		}
		//slow
		if (checking(head,slow_x_y,mxy,slow_char,5,snaketime.main)){
			colected++;
			power = 1;
		}
		//power
		if (checking(head,power_x_y,mxy,power_char,3,snaketime.main)){
			col_power++;
			power = 1;
		}
		//vypisovanie celejho hada
		attron(COLOR_PAIR(2));
		mvprintw(tail[0], tail[1], " ");
        mvprintw(body[2][0], body[2][1], "+");         
        mvprintw(body[1][0], body[1][1], "+");
		//vypisovanie hlavy hada
		attron(COLOR_PAIR(6));
		switch(movement[0]){
			case -1:
				mvprintw(head[0], head[1], "%c",snake_look[3],lenght);
				break;
			case 1:
				mvprintw(head[0], head[1], "%c",snake_look[2],lenght);
				break;
			default:
				break;
		}
		switch(movement[1]){
			case -1:
				mvprintw(head[0], head[1], "%c",snake_look[0],lenght);
				break;
			case 1:
				mvprintw(head[0], head[1], "%c",snake_look[1],lenght);
				break;
			default:
				break;
		}
		attron(COLOR_PAIR(1));
		//anti bug system na stred terminalu XD
		if(erase_once){
			if (tail[0]!= mxy[0] && tail[1]!= mxy[1]){
				mvprintw(mxy[0]/2,mxy[1]/2," ");
				erase_once = 0;
			}
		}
		// pohyb hada
		switch(user_choice){
			case KEY_UP:
				if(movement[0] != 1){
					movement[0]=-1;
					movement[1] = 0;}
				break;
			case KEY_DOWN:
				if (movement[0] != -1){
					movement[0] = 1;
					movement[1] = 0;
				}
				break;
			case KEY_RIGHT:
				if (movement[1] != -1){
					movement[1] = 1;
					movement[0] = 0;}
				break;
			case KEY_LEFT:
				if (movement[1] != 1){
					movement[1] = -1;
					movement[0] = 0;}
				break;
			case ' ':
				if (colected>0){
					if (!(ts.tv_nsec == snaketime.slowed * 1000000000L)){
						colected--;
						ts.tv_nsec = snaketime.slowed * 1000000000L; 
					}
				}
				break;
			case 'M':
			case 'm':
				if(lenght >= 12){	
					if (col_power>0){
							col_power--;
							power = 0;
						}
				}
				break;
			default:
				break;
		}

		// posuvanie hlavy hada
		head[1]+=movement[1];
		head[0]+=movement[0];
		// chvost prebera poslednu hodnotu casti prvku 
		tail[0] = body[lenght-1][0];
		tail[1] = body[lenght-1][1];
		// telo
		for (int i = lenght ;i > 0;i--){
			body[i][0] = body[i-1][0];
			body[i][1] = body[i-1][1];
			if( head[0] == body[i][0] && head[1] == body[i][1] ){
				dead_menu = 1;
			}
		}

		body[0][0] = head[0];
        body[0][1] = head[1];

		refresh();

		nanosleep(&ts, NULL);
		
		// colisions with border
		if (lenght >= 12 && power == 1){
			box(stdscr,0,0);
			if(head[1]==0){
				dead_menu = 1;
			}else if (head[1] == mxy[1]-1)
				dead_menu = 1;
			if(head[0]==0){
				dead_menu = 1;
			}else if (head[0] == mxy[0]-1)
				dead_menu = 1;
		}else{
			border(' ',' ',' ',' ',' ',' ',' ',' ');
			if(head[1]==0){
				head[1] = mxy[1]-1;
			}else if (head[1] == mxy[1]-1)
				head[1] = 0;
			if(head[0]==0){
				head[0] = mxy[0]-2;
			}else if (head[0] == mxy[0]-2)
				head[0] = 0;
		}
		for (int i = 1;i<mxy[1]-1;i++)
			mvprintw(mxy[0]-2,i," ");

		score_power_up(lenght,mxy,colected,col_power,power,stime);
		
		while(dead_menu){
			nodelay(stdscr,0);
			print_in_midd(mxy[0]/2,mxy[1],"------Game Over-----");
			print_in_midd(mxy[0]/2+2,mxy[1],"------Press R for restart-----");
			print_in_midd(mxy[0]/2+4,mxy[1],"------Presa Any key to go to menu-----");
			user_choice = getch();
			switch(user_choice){
				case 'R':
				case 'r':
					mxy[0]+=2;
					score_to_file(lenght,stime);
					return 1;
					break;
				default:
					score_to_file(lenght,stime);
					return 2;
					break;
			}
			refresh();
		}
	}
	return 1;
}

int main(int argc, char *argv[]){

   int mxy[]={0,0,0,0};   //ukladanie rozmerov okna
   int game_run = 0;	  //premenne pomocou ktorých sa určuje či sa má hra spustiť 
   int menu = 1;		 //premenne pomocou ktorých sa určuje či sa má menu spustiť
   
   initscr();
   curs_set(0);
   cbreak();							
   noecho();							
   keypad(stdscr ,1);
   getmaxyx(stdscr,mxy[0],mxy[1]);
   start_color();
   init_pair(1, COLOR_WHITE, COLOR_BLACK);
   init_pair(2, COLOR_GREEN, COLOR_BLACK);
   init_pair(3, COLOR_RED, COLOR_BLACK);
   init_pair(4, COLOR_YELLOW, COLOR_BLACK);
   init_pair(5, COLOR_CYAN, COLOR_BLACK);
   init_pair(6, COLOR_BLUE, COLOR_BLACK);
   
   attron(COLOR_PAIR(1));

   game_run = welcmenu(menu,mxy);
   while (game_run){
		erasing(mxy,1);
		if (game_run == 2){
			erase();
			game_run = welcmenu(menu,mxy);
		}else{
			game_run = game(game_run,mxy);}
	}	

   endwin();		      
	return EXIT_SUCCESS;
}