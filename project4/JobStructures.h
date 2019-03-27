#pragma once

#include <string.h>

// JOB
typedef enum { RUN=0, WAIT, DONE } State;
char* StateStrs[] = {"RUN", "WAIT", "DONE"};
typedef struct Job {
	int id;
	struct Job* next;
	State state;
	int exit;
	char* commandList;
	char* words[];
} Job;

Job* Job_create(char* commandList, char* words[]){
	// id
	static int jobid = 1;
	Job* job = malloc(sizeof(Job));
	job->id = jobid++;
	
	// words i.e. commands (skips words[0])
	for (int i = 0; words[i+1] != NULL; i++){
		job->words[i] = malloc(sizeof(words[i+1]));
		strcpy(job->words[i], words[i+1]);	
	}
	// string of commands
	job->commandList = malloc(sizeof(commandList));
	strcpy(job->commandList, commandList);

	// next, state, exit
	job->next = NULL;
	job->state = RUN;
	job->exit = -1;

	return job;
}

// JOBQUEUE
typedef struct JobQueue {
	Job* front;
} JobQueue;

JobQueue* JobQueue_create(){
	JobQueue* jobqueue = malloc(sizeof(JobQueue));
	jobqueue->front = NULL;
	
	return jobqueue;
}

int addJob(JobQueue* jobqueue, Job* job){
	// empty
	if (jobqueue->front == NULL){
		jobqueue->front = job;
		return EXIT_SUCCESS;
	}
	// not empty
	Job* currJob = jobqueue->front;
	while(currJob->next != NULL){
		currJob = currJob->next;
	}
	currJob->next = job; // pushed to back
	
	return EXIT_SUCCESS;
}
int popJob(JobQueue* jobqueue, Job* poppedJob){
	// empty
	if (jobqueue->front == NULL){
		return 1;
	}
	// not empty
	poppedJob = jobqueue->front;
	jobqueue->front = jobqueue->front->next;
	return EXIT_SUCCESS;
} 
int showJobs(JobQueue* jobqueue){
	printf("JOBID\tSTATE\tEXIT\tCOMMAND\n");
	for (Job* currJob = jobqueue->front; currJob != NULL; currJob = currJob->next){
		if (currJob->exit == -1){
			printf("%d\t%s\t%s\t%s\n", currJob->id, StateStrs[currJob->state], "-", currJob->commandList);
		} else {
			printf("%d\t%s\t%d\t%s\n", currJob->id, StateStrs[currJob->state], currJob->exit, currJob->commandList);
		}
	}
	return EXIT_SUCCESS;
}


