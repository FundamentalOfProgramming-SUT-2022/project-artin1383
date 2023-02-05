#include <ncurses.h>			/* ncurses.h includes stdio.h */  
#include <string.h> 
#include <curses.h>
#include <string.h>
#include "phase1.c"
#include <stdlib.h>
int maxX, maxY, x, y , xcommand , commandindex = 0;
void cleanScreen()
{
    for (int i = 0; i < maxY - 2; i++)
    {
        move(i, 0);
        clrtoeol();
        move(y, x);
    }
}
int main()
{
 char* myinput  = malloc(200);
 for(int i=0;i<200; i++) myinput[i] = 0;
// scanf("%s" ,myinput);
//printf("1%s" , myinput); 
 //printf("2%s" , strtok(myinput , "artin "));
// FILE* file = fopen("artin.txt" , "a");
 char str = 'a';
 char mode = 'i';
 char command[100];
 char signsave = '+';
 for(int i=0;i<100;i++) command[i] = 0;
 int currentLine = 0 , shift = 0;
  getyx(stdscr, y, x);
 initscr();   
    raw();
 //chtype* a = malloc(200 * sizeof(chtype)); 
 chtype a;
 getmaxyx(stdscr, maxY, maxX);
 keypad(stdscr, TRUE);
 //getmaxyx(stdscr,row,col);		/* get the number of rows and columns */
//getstr(str);
 noecho();
 start_color();
char clipboard[1000];
for(int i=0;i<1000;i++) clipboard[i] = 0;
int indexclipboard = 0;
char filename[] = "artin.txt";
while(1){
 currentLine = 0;
  FILE* file = fopen(filename , "a+");
 if(mode == 'i'){
 clrtoeol();
 char c = getc(file);
 mvprintw(0,0 , "%d" , currentLine + 1);
 move(0 ,3);
 while(c!=EOF){
  addch(c);
   if (c == '\n'){
      mvprintw(currentLine + 1,0 , "%d" , currentLine + 2);
      move(++currentLine , 3);
   }
  c  = getc(file);
 }
 }
  getyx(stdscr , y,x);
  mvprintw(maxY-2 , 15 , "%s %c" , filename , signsave );
  move(y,x);
 if(mode == 'i'){
	 getyx(stdscr , y,x);
	 mvprintw(maxY-2 , 2 , "Insert");
	 move(y,x);
 }
 if(mode == 'v'){
	 getyx(stdscr , y,x);
	 mvprintw(maxY - 2 , 2 , "Visual");
	 move(y,x); 
 }
 if(mode =='c'){
	 if(str == ':') getyx(stdscr ,y,x);
         move(maxY-2,0);
	 if(str == ':')deleteln();
	 //getyx(stdscr , y , x);
	 if(str == ':') xcommand = 0;
	 if(str == '\n'){
		 xcommand = -1;
		 mode = 'i';
		 deleteln();
		 move(y , x);
		if(strstr(command , "undo") != NULL) {
		copycontent("undo.txt",filename);
                currentLine = 0;
                clrtoeol();
                FILE* file = fopen(filename , "a+");
                /*char c = getc(file);
                mvprintw(0,0 , "%d" , currentLine + 1);
                move(0 ,3);
                while(c!=EOF){
                addch(c);
                if (c == '\n'){
                mvprintw(currentLine + 1,0 , "%d" , currentLine + 2);
                move(++currentLine , 3);
                }
                c  = getc(file);
                }*/
		}
		if(strstr(command , "save") != NULL){
		  getyx(stdscr , y,x);
		  signsave = ' ';
                  mvprintw(maxY-2 , 15 , "%s %c" , filename , signsave );
                  move(y,x);
		}
		if(strstr(command , "saveas") != NULL){
		  strcpy(filename , strstr(command , " ") + 1);
                  //FILE* file2 = fopen(strstr(command , " ") + 1 , "w");
		  copycontent(filename , strstr(command , " ") + 1); 
		}
                str = 0;
              }
	 else {
           mvaddch(maxY-1 , xcommand , str);
	   if(str != ':') {
		   command[commandindex] = str;
		   commandindex++;
	   }
	 }
         xcommand++;
 }
 if(mode == 'n'){
         getyx(stdscr , y,x);
         mvprintw(maxY - 2 , 2 , "normal");
         move(y,x);
 }
 str = getch();
  if(str == 27) break;
 if(str != 5 && str != 3 && str !=2 && str!= 4&& str!=75 && mode =='i'&& str!=':' && str != 'n' && str !='v' ) {
	   copycontent(filename , "undo.txt");
	   fputc(str , file);
	   addch(str);
 }
if(str ==3){
     	//top
        getyx(stdscr , y,x);
        move(--y , x);
            }
 
     if(str == 5){ // right
        getyx(stdscr , y,x);
	if(x >= 1){
	move(y, x+1);	
	 if(mode == 'v'){
		 a = mvinch(y,x);
		 init_pair(1, COLOR_BLACK, COLOR_YELLOW);
		 mvaddch(y , x, a | COLOR_PAIR(1));
		 clipboard[indexclipboard] = a;
		 indexclipboard++;
	 }
	 }
     }
     if(str ==  4){ //left
	//a = inch();
	//if(a!='d')
	getyx(stdscr , y,x);
        if(x >= 1){
        move(y, x-1);
         if(mode == 'v'){
                 a = mvinch(y,x);
                 init_pair(1, COLOR_YELLOW, COLOR_BLACK);
                 mvaddch(y , x-1, a | COLOR_PAIR(1));
		 getyx(stdscr , y,x);        
		 move(y, x-1);
		 clipboard[indexclipboard] = a;
                 indexclipboard++;
            }
        }
     }
     if(str == 2){ //buttom
        getyx(stdscr , y,x);
        move(++y , x);
      }
     if(str == 75){
        mode = 'i';
     }
     if(str == 'n' && mode != 'c') mode = 'n';
     if(str == 'v' && mode != 'c') mode = 'v';
     if(mode == 'v' && str == 100){
       getyx(stdscr , y,x);
		 for(int i =0; i<strlen(clipboard);i++){
		 move(currentLine, x-1);
                 a = mvinch(y,x-1);
		 init_pair(2, COLOR_WHITE, COLOR_BLACK); //normal
                 mvaddch(y , x-1, a | COLOR_PAIR(2));
                 getyx(stdscr , y,x);
                 move(currentLine, x-1);
		 }
      getyx(stdscr , y,x);
         mvprintw(10 ,10, "%s" , clipboard);
         move(y,x);
	}
      if(str == 58){
        mode = 'c';
      }
      if(str == 'a' && mode =='n') {
	      indent(filename);
	      currentLine = 0;
  FILE* file = fopen(filename , "a+");
 clrtoeol();
 char c = getc(file);
 mvprintw(0,0 , "%d" , currentLine + 1);
 move(0 ,3);
 while(c!=EOF){
  addch(c);
   if (c == '\n'){
      mvprintw(currentLine + 1,0 , "%d" , currentLine + 2);
      move(++currentLine , 3);
   }
  c  = getc(file);
 }
      }
      if(str == 'p' && mode =='n'){
        for(int i = 0; i<strlen(clipboard);i++){
	  addch(clipboard[i]);
	}
      }
   if (str == '\n'){
      mvprintw(currentLine + 1,0 , "%d" , currentLine + 2);	
      move(++currentLine , 3);
   }
   fclose(file);
 }
 endwin();
}
