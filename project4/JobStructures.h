#pragma once

#include <string.h>

typedef struct Job {
	int id;
	char* words[];
} Job;

Job* Job_create(char* words[]){
	// id
	static int jobid = 1;
	Job* job = malloc(sizeof(Job));
	job->id = jobid++;
	
	// words i.e. commands
	for (int i = 0; words[i] != NULL; i++){
		job->words[i] = malloc(sizeof(words[i]));
		strcpy(job->words[i], words[i]);	
	}
	return job;
}

typedef struct JobQueue {
	Job* front;
	Job* back;

	/* int addJob(Job j){ */
	/* 	if (front ) */
	/* } */

} JobQueue;

JobQueue* JobQueue_create(){
	JobQueue* jq = malloc(sizeof(JobQueue));
	jq->front = NULL;
	jq->back  = NULL;
	
	return jq;
}
