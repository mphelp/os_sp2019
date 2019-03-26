#pragma once

#include <string.h>

typedef struct Job {
	Job* next;
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

typedef struct JobQueue {
	Job* front;

	int addJob(Job* job){
		// empty
		if (front == NULL){
			front = job;
			return EXIT_SUCCESS;
		}
		// not empty
		for (Job* j = front; j->next != NULL; j = j->next){}
		j->next = job;
		
		return EXIT_SUCCESS;
	}
} JobQueue;

JobQueue* JobQueue_create(){
	JobQueue* jq = malloc(sizeof(JobQueue));
	jq->front = NULL;
	jq->back  = NULL;
	
	return jq;
}
