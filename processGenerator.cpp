#include "clkUtilities.h"
#include "queueUtilities.h"
#include <iostream>
 #include <unistd.h>
#include<queue>
#include<fstream>
#include<string>
#include<sstream>
#include<signal.h>

using namespace std;


void ClearResources(int);
queue<struct processData> readprocesses();


pid_t sch_id;
int main() {
    
   
     initQueue(true);
     queue<struct processData> process_queue;
     
    //TODO: 
    // 1-Ask the user about the chosen scheduling Algorithm and its parameters if exists.
    // 2-Initiate and create Scheduler and Clock processes.
    
     
     int choosen_sch,quantum;
     cout<<"Enter 1 to choose non-preemptive HPF,"<<endl;
     cout<<"Or 2 to choose Shortest Remaining time​Next,"<<endl;
     cout<<"Or 3 to choose Round​Robin :D"<<endl;
     cin>>choosen_sch;
     my_algo=choosen_sch;
     if(choosen_sch==3)
     {
         cout<<"Enter the Quantum you want..."<<endl;
         cin>>quantum;
         
     }
     
         
     
     pid_t clkid = fork(); // create child process
    //cout<<clkid<<endl;
    

    if(clkid==-1)
    {
        perror("fork");
    }
    else if (clkid==0)
    {
        
        
         
        execl("./clock.out","clock.out","-r", "-t", "-l",(char *) 0); // run the command
        //perror("execl");
      
    }
    
    sch_id = fork(); // create child process
    cout<<sch_id<<endl;
    

    if(sch_id==-1)
    {
        
        perror("fork");
    }
    else if (sch_id==0)
    {
        sleep(1);
        
        stringstream ss,ss1;
        ss<<choosen_sch;
        
        
        
        if(choosen_sch==3)
        {
            ss1<<quantum;
            execl("./sch.out","sch.out","-r", "-t", "-l", ss.str().c_str(),ss1.str().c_str(), (char *) 0); // run the command
        }
   
        else
            execl("./sch.out","sch.out","-r", "-t", "-l", ss.str().c_str(), (char *) 0); // run the command
            
        //perror("execl");
        
    }
    else
    {
        
       
       
         // 3-use this function AFTER creating clock process to initialize clock, and initialize MsgQueue
         initClk();
         
         process_queue= readprocesses();

     
    //TODO:  Generation Main Loop
    //4-Creating a data structure for process  and  provide it with its parameters 
    //5-Send & Notify the information to  the scheduler at the appropriate time 
    //(only when a process arrives) so that it will be put it in its turn.
        
        
         
         
         while(!process_queue.empty())
         {
             struct processData pD1=process_queue.front();
             
             while(pD1.arrivaltime==(int)getClk())
             {
                 cout<<pD1.arrivaltime<<" "<<(int)getClk()<<endl;
                 
                if(choosen_sch==2)
                kill(sch_id,SIGINT);

                 
                cout<< "on send"<<Sendmsg(pD1)<<endl;
                
                 process_queue.pop();
                 //sleep(1);
                 if(!process_queue.empty())
                 pD1=process_queue.front();
                 else
                 {
                     
                     break;
                 }  
                 
             }
         }
         
        
    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
       
       cout<<"parent after initclk"<<endl;
       int x= getClk();
       printf("current time is %d\n",x);
     
    //////Tosend something to the scheduler, for example send id 2
 
     sleep(1);
     
    //no more processes, send end of transmission message
    
    
     kill(sch_id,SIGINT);
     lastSend();
     
    
     
         int f,stat_loc; //to wait scheduler terminate
      f = wait(&stat_loc);
         
  	  if(!(stat_loc & 0x00FF))
  	    printf("\nA child with pid %d terminated with exit code %d\n", f, stat_loc>>8);
            
     
  
      //To clear all resources
      ClearResources(0);
     
   //ClearResources(0);
    //======================================
    }

  
}

void ClearResources(int)
{
    msgctl(qid, IPC_RMID, (struct msqid_ds*)0);
    destroyClk(true); 
    exit(0);
}



queue<struct processData> readprocesses(){
	string s="";
        
        
        ifstream inFile;
        inFile.open("./processes.txt",ios::in);
         getline(inFile,s);
         //int id,arrival,runtime,priority;
       
         queue<struct processData> process_queue;
         
	 while (getline(inFile,s)){
	
	    stringstream ss(s);
		

             
             processData pD;
             ss>>pD.id >>pD.arrivaltime>>pD.runningtime>>pD.priority;
            
              pD.remainingtime=pD.runningtime;
              process_queue.push(pD);
           }
         
         return process_queue;

	
}



