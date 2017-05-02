/**************************************
Names: Amir Kessler, Zach, Colton Wood
Class: CSCE 4600
Assignment: Project 2
Due Date: 5/4/2017
Purpose: The goal of the project is to
simulate 3 different job scheduling
alogirthms using an input file and output
the results of the algorithms
**************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//Struct to hold the info for each process
struct list{
	char processID[20];
	int arrival;
	int runTime;
	int priority;
	int startTime;
	int endTime;
	int turn;
	int wait;
	int response;
};

//Function Declarations
void FIFO(struct list process[50], int num);
void SJF(struct list process[50], int num);
void other(struct list process[50], int num);
void output(struct list process[50], int fin);
void printer(struct list process);
void averager(int turner, int waiter, int responder, int contexter, int tot);

int main(void)
{
	//Create the struct and file pointer
	struct list process[50];
	FILE *fp;
	char filename[100];

	//Prompts the user to enter a file name
	printf("Please enter a file name: ");
	// scanf("%s", filename);

	//Opens the file, exits program if the file is not found/doesn't exist
	// fp = fopen(filename, "r");
	fp = fopen("input.txt", "r");
	if(fp == NULL){
		printf("Error: File not found");
		return 0;
	}

	//Reads the input file, storing the process info into an array and counting the numbrt of processes
	int num = 0;
	while(!feof(fp)){
		fscanf(fp, "%s %d %d %d", process[num].processID, &process[num].arrival, &process[num].runTime, &process[num].priority);
		process[num].endTime = 0;
		process[num].turn = 0;
		process[num].wait = 0;
		process[num].response = 0;
		process[num].startTime = 0;

		// testing
		// printf("%s %d %d %d\n", process[num].processID, process[num].arrival, process[num].runTime, process[num].priority);

		num++;
	}

	fclose(fp);

	printf("\njob      ariv      runt      prio      strt      endt      turn      wait      resp\n");
	printf("-----------------------------------------------------------------------------------\n");

	FIFO(process, num);

	return 0;
}

void FIFO(struct list process[50], int num)
{
	// totals
	int starter = 0;
	int ender = 0;
	int turner = 0;
	int waiter = 0;
	int responder = 0;
	int contexter = 0;

	// counter
	int i = 0;
	
	// loop through all processes
	for (i=0; i<num; i++) {

		// start times
		if (i!=0) process[i].startTime = process[i-1].endTime;
		starter += process[i].startTime;

		// end times
		process[i].endTime = process[i].startTime + process[i].runTime; 
		ender += process[i].endTime;

		// turnaround times
		process[i].turn = process[i].endTime - process[i].arrival;
		turner += process[i].turn;

		// wait times
		process[i].wait = process[i].startTime - process[i].arrival;
		waiter += process[i].wait;

		// response times
		if (i!=0) process[i].response = process[i-1].endTime;
		responder += process[i].response;

		// print values in table
		printer(process[i]);
	}	

	// average values
	averager(turner, waiter, responder, contexter, num);

}
//Schedule: Shortest Job First
//Schedules the next process
//based on which process has
//the shortest job to complete
void SJF(struct list process[50], int num)
{
	//bool to check when all processes run
	bool complete = false;
	int time = 0;
	int pro;
	int fin;

	//while loop to loop through all process
	while(complete == false){
		int job = 9999999;
		//set job time to a very high number, then check for min job time
		for(pro = 0;pro<num;pro++){
			if(process[pro].runTime<job && process[pro].endTime == 0){
				job = process[pro].runTime;
				fin = pro;
			}
		}

		//increment time by job time, then set that to the process' end and turnaround time
		//output the completed job
		time += process[fin].runTime;
		process[fin].endTime = time;
		process[fin].turn = time;
		output(process, fin);

		//increment the start, wait, and response times for waiting process
		//also checks if any processes are still waiting
		bool com = false;
		for(pro = 0;pro<num;pro++){
			if(process[pro].endTime == 0){
				com = true;
				process[pro].startTime += time;
				process[pro].wait += time;
				process[pro].response += time;
			}
		}

		//if no more processes are waiting, then the schedule is complete
		if(com == false)
			complete = true;
	}
}

void other(struct list process[50], int num)
{

}

//Output function
//Called upon completion of a process,
//outputs the info of that process to the screen
void output(struct list process[50], int fin)
{
	sprintf("%s %12d %13d %13d %15d %20d %14d %18d\n", process[fin].processID, &process[fin].arrival, 
	&process[fin].runTime, &process[fin].priority, &process[fin].startTime, 
	&process[fin].endTime, &process[fin].turn, &process[fin].wait, &process[fin].response);
}

// print result 
void printer(struct list process) {
	printf("%-5s %7d %9d %9d %9d %9d %9d %9d %9d\n", 
		process.processID, 
		process.arrival, 
		process.runTime, 
		process.priority, 
		process.startTime, 
		process.endTime, 
		process.turn, 
		process.wait, 
		process.response);
}

// calculate/print averages
void averager(int turner, int waiter, int responder, int contexter, int tot) {
	printf("Average Turnaround Time = %10.2f\n", (float)(turner / tot));
	printf("Average Wait Time       = %10.2f\n", (float)(waiter / tot));
	printf("Average Response Time   = %10.2f\n", (float)(responder / tot));
	printf("Number Context Switches = %10.2f\n", (float)contexter);
}

