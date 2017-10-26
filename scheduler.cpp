//#include "clkUtilities.h"
//#include "queueUtilities.h"

//#include "non_prem_hpf.h"
//#include "strn.h"
#include "round_r.h"

//#include<iostream>
//#include<signal.h>
//using namespace std;

int recv_ret=-1,to_be_end=0;
void handle_rec(int)
{
   
}
void to_receive()
{
     do{
    struct processData pD;
    recv_ret=Recmsg(pD);
    if(recv_ret!=-1)
    {
    cout<< "on receive "<<recv_ret<<" time is "<<getClk()<<endl;//returns -1 on failure; 1 on the end of processes, 0 no processes available yet
    printf("current received data %d\n",pD.id);
    }
   
    if(recv_ret==1)
        to_be_end=1;
   
    }while(recv_ret>1);
    
}
int main(int argc, char* argv[]) {
    initQueue(false);
    initClk();
    int choosen_sch=0,quantum=-1;
    
    //TODO: implement the scheduler :)


    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
     
   /* struct processData pD1;  //to receive choosen scheduler
    cout<< "on receive "<<Recmsg(pD1)<<" time is "<<getClk()<<endl;//returns -1 on failure; 1 on the end of processes, 0 no processes available yet
    printf("current received data %d\n",pD1.id);
    choosen_sch=pD1.id;*/
    //raise(SIGTSTP);
    
    choosen_sch=atoi(argv[4]);
    
    if(choosen_sch==3)
    {
        quantum=atoi(argv[5]);
       
    }
   
  
    signal(SIGINT,handle_rec);
        while(1)
    {
    
        
        to_receive();
    if(to_be_end==1)   // && queue_of_processs.empty() will be added
    {
        printf("end of processes...exiting sch\n");
        break;
    }
       
   
    pause();
    //raise(SIGTSTP);
    //kill(getpid(),SIGTSTP);
    }
   
    
    exit(0);
    //kill(getppid(),SIGCHLD);
    //return 0;
    
}
