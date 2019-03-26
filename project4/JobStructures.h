#pragma once

#include <string.h>

// JOB
typedef struct Job {
	struct Job* next;
	int id;
	char* words[];
} Job;

Job* Job_create(char* words[]){
	// id
	static int jobid = 1;
	Job* job = malloc(sizeof(Job));
	job->id = jobid++;
	
	// words i.e. commands (skips words[0])
	for (int i = 0; words[i+1] != NULL; i++){
		job->words[i] = malloc(sizeof(words[i+1]));
		strcpy(job->words[i], words[i+1]);	
	}
	// next
	job->next = NULL;

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


