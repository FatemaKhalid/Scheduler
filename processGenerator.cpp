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
void for_HPF(int);
void for_Round_r(int);

pid_t sch_id;
int main() {
    
   
     initQueue(true);
     queue<struct processData> process_queue;
     
    //TODO: 
    // 1-Ask the user about the chosen scheduling Algorithm and its parameters if exists.
    // 2-Initiate and create Scheduler and Clock processes.
    
     
     int choosen_sch;
     cout<<"Enter 1 to choose non-preemptive HPF,"<<endl;
     cout<<"Or 2 to choose Shortest Remaining time​Next,"<<endl;
     cout<<"Or 3 to choose Round​Robin :D"<<endl;
     cin>>choosen_sch;
      
     pid_t clkid = fork(); // create child process
    //cout<<clkid<<endl;
    

    if(clkid==-1)
    {
        perror("fork");
    }
    else if (clkid==0)
    {
       // cout<<"before exec clk "<< getppid() << " my id "<<getpid() <<endl;
        execl("/home/heba/Desktop/cmp/os/os_windows/assim_1/code/clock.out","/home/heba/Desktop/cmp/os/os_windows/assim_1/code/clock.out","-r", "-t", "-l", (char *) 0); // run the command
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
        //cout<<"before exec sch "<< getppid() << " my id "<<getpid()<<endl;
        execl("/home/heba/Desktop/cmp/os/os_windows/assim_1/code/sch.out","/home/heba/Desktop/cmp/os/os_windows/assim_1/code/sch.out","-r", "-t", "-l", (char *) 0); // run the command
        //perror("execl");
        
    }
    else
    {
        
       
       // sleep(1);
       // cout<<"bdayet parent"<<endl;
         // 3-use this function AFTER creating clock process to initialize clock, and initialize MsgQueue
         initClk();
         
         process_queue= readprocesses();

     
    //TODO:  Generation Main Loop
    //4-Creating a data structure for process  and  provide it with its parameters 
    //5-Send & Notify the information to  the scheduler at the appropriate time 
    //(only when a process arrives) so that it will be put it in its turn.
        
         struct processData pD;  //to send choosen scheduler
         pD.id=choosen_sch;
         kill(sch_id,SIGCONT);
         cout<< "on send"<<Sendmsg(pD)<<endl;
         
         if (choosen_sch==1)
             signal(SIGCHLD,for_HPF);
         else if(choosen_sch==3)
                  {
                 signal(SIGCHLD,for_Round_r);
                 cout<<"sethandler"<<endl;
               }
          
         
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
                 
                /* if(choosen_sch==3)
                {
                    for_Round_r();
                }
                else if(choosen_sch==1)
                {
                    for_HPF();
                }*/
                 
             }
           
         }
         
        
    //===================================
    //Preimplemented Functions examples

    /////Toget time use the following function
       
       cout<<"parent after initclk"<<endl;
       int x= getClk();
       printf("current time is %d\n",x);
     
    //////Tosend something to the scheduler, for example send id 2
  
   // Sendmsg(process_queue.front()); //returns -1 on failure;*/
    
     sleep(1);
     
    //no more processes, send end of transmission message
    
    
      //if(choosen_sch==2)
     kill(sch_id,SIGINT);
     
     lastSend();
               /* if(choosen_sch==3)
                {
                    for_Round_r();
                }
                else if(choosen_sch==1)
                {
                    for_HPF();
                }*/
     // kill(sch_id,SIGCONT);
     cout<<"on last send "<<endl;
      
     if(choosen_sch==2)
     {
         int f,stat_loc; //to wait scheduler terminate
      f = wait(&stat_loc);
         
  	  if(!(stat_loc & 0x00FF))
  	    printf("\nA child with pid %d terminated with exit code %d\n", f, stat_loc>>8);
            
     
  
    //////////To clear all resources
    ClearResources(0);
     }
      else 
    {
        while(1)
        {
            
        }
    }
   
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
        inFile.open("/home/heba/Desktop/cmp/os/os_windows/assim_1/code/processes.txt",ios::in);
         getline(inFile,s);
         int id,arrival,runtime,priority;
       
         queue<struct processData> process_queue;
         
	 while (getline(inFile,s)){
	
	    stringstream ss(s);
		

             ss>>id>>arrival>>runtime>>priority;
            
              processData pD;
              pD.id = id;
              pD.arrivaltime=arrival;
              pD.runningtime=runtime;
              pD.priority=priority;
              process_queue.push(pD);
           }
         
         return process_queue;

	
}
void for_HPF(int)
{
   // cout<<"hpf"<<endl;
    int f,stat_loc; //to wait scheduler terminate
      f = wait(&stat_loc);
         
  	  if(!(stat_loc & 0x00FF))
  	    printf("\nA child with pid %d terminated with exit code %d\n", f, stat_loc>>8);
            
     //////////To clear all resources
    ClearResources(0);
}

void for_Round_r(int)
{
    int* status;
    int w;
    
    cout<<"roubin "<<sch_id<<endl;
    
     w = waitpid(sch_id,status, WUNTRACED | WCONTINUED|WNOHANG);
     cout<<w<<" ";
            if (w == -1) { perror("waitpid"); exit(EXIT_FAILURE); }


            if (WIFEXITED(status)) {
                
                printf("exited, status=%d\n", WEXITSTATUS(status));
                ClearResources(0);
                
            } else if (WIFSIGNALED(status)) {
               
                printf("killed by signal %d\n", WTERMSIG(status));
                ClearResources(0);
                
                
            } else if (WIFSTOPPED(status)) {
                printf("stopped by signal %d\n", WSTOPSIG(status));
            } else if (WIFCONTINUED(status)) {
                printf("continued\n");
            }
     
}






