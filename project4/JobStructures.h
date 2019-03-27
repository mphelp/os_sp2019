#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// JOB ===============================================
typedef enum { WAIT=0, RUN, DONE } State;
char* StateStrs[] = {"WAIT", "RUN", "DONE"};
typedef struct Job {
	int id;
	pid_t pid;
	struct Job* next;
	State state;
	int exit;
	char* commandList;
	char** words;
} Job;

Job* Job_create(char* commandList, int nwords, char* words[]){
	// id
	static int jobid = 1;
	Job* job = malloc(sizeof(Job));
	job->id = jobid++;
	
	// words i.e. commands (skips words[0])
	job->words = malloc(nwords*sizeof(char*));
	int i;
	for (i = 0; words[i+1] != NULL; i++){
		/* job->words[i] = malloc(sizeof(words[i+1])); */
		/* strcpy(job->words[i], words[i+1]); */	
		job->words[i] = strdup(words[i+1]);
		/* printf("jw %d: %s, w %d: %s\n", i, job->words[i], i, words[i]); */
	}
	job->words[i] = '\0'; // null terminated

	// string of commands
	job->commandList = malloc(sizeof(commandList));
	strcpy(job->commandList, commandList);

	// next, state, exit, pid
	job->next = NULL;
	job->state = WAIT;
	job->exit = -1;
	job->pid = -1;

	return job;
}

// JOBQUEUE ===========================================
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
	Job* newJob = jobqueue->front;
	while(newJob->next != NULL){
		newJob = newJob->next;
	}
	newJob->next = job; // pushed to back
	
	return EXIT_SUCCESS;
}

// Selecting job and running it
int runJob(Job* awaitingJob){
	awaitingJob->state = RUN;
	// print out words of job:
	/* for (int i = 0; awaitingJob->words[i] != NULL; i++){ */
	/* 	printf("word %d: %s,  ", i, awaitingJob->words[i]); */
	/* } */
	/* printf("\n"); */

	pid_t rc = fork();
	if (rc < 0){
		debug("Fork failed");
		return EXIT_FAILURE;
	}	else if (rc == 0){
		/* printf("--> command %s ... process %d started\n", 
				awaitingJob->words[0], (int)getpid());*/

		// Make directory of output files if needed
		struct stat st = {0};
		if (stat("./outputs", &st) == -1){
			mkdir("./outputs", 0775);
		}
		// Open output file
		char outputFileName[10];
		sprintf(outputFileName, "./outputs/output.%d", awaitingJob->id);
		int outputFD = open(outputFileName, 
				O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		dup2(outputFD, 1);
		dup2(outputFD, 2);
		close(outputFD);

		// Execute job
		execvp(awaitingJob->words[0], awaitingJob->words);
		debug(": Failed... %s", strerror(errno));
		exit(1);
	} else {
		awaitingJob->pid = rc;
	}
	return EXIT_SUCCESS;
}
int indicateCompleteJob(JobQueue* jobqueue, pid_t pid, int exitStatus){
	Job* currJob = jobqueue->front;
	while (currJob != NULL){
		// found correct job
		if (currJob->pid == pid){
			currJob->state = DONE;
			currJob->exit = exitStatus;
			return EXIT_SUCCESS;
		}
		currJob = currJob->next;
	}
	return 2; // reached end
}
int selectJobToRun(JobQueue* jobqueue){
	// GOAL: select job to complete, thus "popped" from queue
	if (jobqueue->front == NULL){
		return 1; // empty
	}
	// not empty
	Job* awaitingJob = jobqueue->front;
	while (awaitingJob->state != WAIT){
		awaitingJob = awaitingJob->next;
		if (awaitingJob == NULL){
			return 1; // no job able to run, reached end
		}
	}
	// send awaitingJob to other thread execution : state becomes RUN
	return runJob(awaitingJob);
}
int removeJob(JobQueue* jobqueue, int id){
	// GOAL: remove job from queue, cannot be in RUN state
	if (jobqueue->front == NULL){
		return 1; // empty
	}
	// not empty
	Job* awaitingJob = jobqueue->front;
	// ADD ANOTHER JOB PREV TO REMOVE AWAITING
	while (awaitingJob != NULL && awaitingJob->id != id){}
	if (awaitingJob == NULL){
		return 1; // reached end, job does not exist
	} else if (awaitingJob->state == RUN){
		return -1; // job is being run currently
	} else {
		// actually remove from queue and delete any output file
		
	}
} 

// Displaying jobs for "status" command
int showJobs(JobQueue* jobqueue){
	printf("JOBID\tSTATE\tEXIT\tCOMMAND\n");
	for (Job* currJob = jobqueue->front; currJob != NULL; currJob = currJob->next){
		if (currJob->state == WAIT || currJob->state == RUN){
			printf("%d\t%s\t%s\t%s\n", currJob->id, StateStrs[currJob->state], "-", currJob->commandList);
		} else {
			printf("%d\t%s\t%d\t%s\n", currJob->id, StateStrs[currJob->state], currJob->exit, currJob->commandList);
		}
	}
	return EXIT_SUCCESS;
}

