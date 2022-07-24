
//Useful thing mvaddch(y,x, ' ');    //To errase previous dot (y,x) using "space"
#include <ncurses.h>


struct dots
{
 int y;
 int x;
 int sign; //charecter representing this dot. int type used because of 2-byted keys on a keyboard(arrows, Fn`s, ...)
};



int main(void)
{
 int i;                         //counter 

 initscr();
 curs_set(0);                   //argument 0 sets cursor in invisible mode, 1 - norm-visible and 2 - very-visible
 keypad(stdscr, TRUE);          //"To recognize special 2-byted symbols like F1, F2, KEY_DOWN, ..."
 refresh();
 
 int Y = LINES/2;               //set dot(y,x) as center of screen
 int X = COLS/2;
 struct dots Zmeyka[75]; 
 for(i=0;i<75;++i) { Zmeyka[i].y = Y; Zmeyka[i].x = X + i; Zmeyka[i].sign = '*'; }
 //Inicialise each point of "Zmeyka" according to default position( '********' in the center of screen)

 

struct dots move_dot( struct dots* p)       //Giving pointer ecxept value - cheeper 
{
// keypad(stdscr, TRUE);          //"To recognize special 2-byted symbols like F1, F2, KEY_DOWN, ..."
 int bf = getch();                //special simbols, like "arrows" requires 2bytes, so char type - isn`t enough               

 if (bf == KEY_DOWN) 
 {
  (*p).y+=1;                       //The uppest line has nuber 0, so each belower has greater value(cause line numb supposed noy to be  negative)
  mvaddch((*p).y, (*p).x, (*p).sign );
 }


 else if (bf == KEY_UP) 
 {
  (*p).y-=1;                       //Upper we rise - more we decriment line number
  mvaddch((*p).y, (*p).x, (*p).sign );
 }

 else if (bf == KEY_LEFT) 
  {
   (*p).x-=1;
   mvaddch((*p).y, (*p).x, (*p).sign );
  }

 else if (bf == KEY_RIGHT) 
  {
   (*p).x+=1;
   mvaddch((*p).y, (*p).x, (*p).sign );
  }
 else (*p).sign = '&';            //& - will mean that player typed wrong button 
    
 return *p;                       //*p is unnamed pointer to struct dots <=> struct dots
}

struct dots buff;

 for(;;)                       
 {
  
  Zmeyka[0].sign = ' ';          //Vanishing tail of snake
  mvaddch(Zmeyka[0].y, Zmeyka[0].x, Zmeyka[0].sign );
 
  for(i=0;i<74;++i)               //"moving"(changing for next) each block of snake 
  {                              //except last(7th if to count from 0) due it has been already mooved by move_dot() 
   Zmeyka[i] = Zmeyka[i+1];
   mvaddch(Zmeyka[i].y, Zmeyka[i].x, Zmeyka[i].sign );  
  }
  
  buff = move_dot(&Zmeyka[74]);   //Player moves head of snake 
      
  Zmeyka[74].y = buff.y;
  Zmeyka[74].x = buff.x;
  if (buff.sign == '&') break;  //Checking if player typed something ecxept arrows of moving
 }




 endwin();                      //closing window
 return 0;
}


