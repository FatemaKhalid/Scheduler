#pragma once

#include "for_includes.h"

void NONPRE_HPF(priority_queue<processData> &ready_queue){			
        // receiving priority queue
	while(!ready_queue.empty()){

		processData processRunning;
		processRunning.process_id=fork();
		processRunning=ready_queue.top();
		ready_queue.pop();
		if(processRunning.process_id==-1)
		{

			perror("fork");
		}
		else if (processRunning.process_id==0)
		{
			processRunning.waitingTime=processRunning.arrivaltime-getClk();
			stringstream ss;
        	ss<<processRunning.remainingtime;
             execl("./process.out","sch.out","-r", "-t", "-l",ss.str().c_str(), (char *) 0);
            printf("process Running waiting Time %d \n", processRunning.waitingtime);
		}


	}
}