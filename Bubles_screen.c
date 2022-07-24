#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


int Buble[9] ={' ','-',' ',
               '(',' ',')',
               ' ','-',' ',};

void mv_Buble(int x, int y){
    for(int k = 0; k < 7; k++){
      erase();
      for(int i=0;i<9;++i)      
        mvaddch(x+i/3,y+i%3,Buble[i]); 
      
      x++;  y++;              // diagonal moving down
      refresh();
      usleep(50000);
    }
}


void* thread(int* arg)
{
  int i;
  int x, y;
  int colour = *arg; 
 
  while(1)
  {
    
//--------Community_section abowe----

//--------Monopoly of current thread section bellow----
    pthread_mutex_lock( &mutex ); //garanty that mutex is locked
//doing drawing stuff asshured by mutex in this Frame   

    x = random() % (20);
    y = random() % (20);

    attron(COLOR_PAIR(colour));
    mv_Buble(x, y);
    attroff(COLOR_PAIR(colour));
    
    pthread_mutex_unlock( &mutex );
//--------Comunity section bellow-------------------
    refresh();
    usleep(100000); 
  }
  return NULL;
}

int main(int argc, char *argv[])
{
  pthread_t pid[2];

  initscr();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  start_color();
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2,COLOR_YELLOW, COLOR_BLACK);
  
  int colours[2] = {1, 2};

  for ( int i = 0 ; i < 2; i++ ) 
    pthread_create(&pid[i],NULL, thread, &colours[i]);

  getch();            //this getch devides us from workflag = 0, until we push someth. E.g. cycle works(flag=1) until we push a key

  for ( int i = 0 ; i < 2; i++ ) pthread_join(pid[i],NULL);

  pthread_mutex_destroy(&mutex);
  endwin();
  return 0;
}












