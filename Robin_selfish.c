#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>           //for random

#define NUMBER_OF_PROCESSES 6 //number of consumers-proceses which`re represented by own progress-line on a screen
                              //proces - just a name, this prog works as single process

                              

float kvant = 0.0;            //time given each consumer to work(in real life it means proces`s time given monopoly to process) 
float worktime[NUMBER_OF_PROCESSES]={0.0}; //"oldness" term in self round robin(srr)

char sign[15]={'@', '#', '^', '%', '$',
               '+', '*', '(', '>', '<',
               '|', '/', '\\', '?','o'};

int occupated[NUMBER_OF_PROCESSES]={0};   //occupated blocks(run the program - and understand what it means)

typedef struct Process
{   
    int number;
    char sign;   

} Process;


float generate_kvant(void)
{
 float new_kvant=(float)(random()%32);  //MAX = 32, Min 0
 new_kvant/=10;                         //MAX_kv = 3.2, MIN_kv = 0;
 return new_kvant;
}


void consum (Process proc)   
{    
  kvant = generate_kvant();   //process takes kvant-time to work
  int k=occupated[proc.number];                       //k - need for simple refering to occupation

  
  while ( kvant>0.0 )                //works when kvant!=0.0
  {
    k = occupated[proc.number]+1;   //"occupate" next block in line    

    mvaddch (proc.number, k, proc.sign);  //occupate j-th element of string on a screen
   // usleep(300000);
    usleep(90000);
    kvant-=(float)0.1;               //it`s global var, so we don`t need to return it`s value
    worktime[proc.number]+=(float)0.1; 

    occupated[proc.number]++; //reflect occupation
    refresh();
  }
}


int youngest_proc(void)
{
 int i,j;
 float min=500.0; //Big number, which bigger than any worktime  
 
 for (i=0;i<NUMBER_OF_PROCESSES;i++)   
  if(worktime[i]<min)
   { min = worktime[i]; j=i; }

 return j;       //youngest proc - worked not more than any other proc 
}




int main ( int argc, char* argv[])
{
 int i,j;
 initscr();
 noecho();
 refresh();

 Process processes[NUMBER_OF_PROCESSES];
 
//default_initialization
 for (i=0;i<NUMBER_OF_PROCESSES;++i) 
 {
  processes[i].number=i;
  processes[i].sign=sign[i%15]; //we have defined only 15 charecters. So let`s use `em again if we have >15 "processes"
 }



int youngest;

while(1)
{
 move(10,10);
 printw("Owned blocks: 1:%d 2:%d 3:%d 4:%d 5:%d 6:%d", occupated[0], occupated[1], occupated[2], occupated[3], occupated[4], occupated[5] );
 move(12,10);
 printw("Worktime:     1:%f 2:%f 3:%f 4:%f 5:%f 6:%f", worktime[0], worktime[1], worktime[2], worktime[3], worktime[4], worktime[5] );
 refresh();
 move(0,0);
 youngest = youngest_proc();
 consum(processes[youngest]);
 refresh();
}

 


 endwin();
 return 0;
}
