#pragma once
#define MAXNJOBS 20

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
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	Job* front;
	int njobs;
} JobQueue;

JobQueue* JobQueue_create(){
	JobQueue* jobqueue = malloc(sizeof(JobQueue));
	jobqueue->front = NULL;
	jobqueue->njobs = 1;
	pthread_mutex_init(&jobqueue->mutex, NULL);
	pthread_cond_init(&jobqueue->cond, NULL);

	return jobqueue;
}
void set_njobs(JobQueue* jobqueue, int njobs){
	pthread_mutex_lock(&jobqueue->mutex);
	if (njobs > 0 && njobs < MAXNJOBS){
		jobqueue->njobs = njobs;	
	}
	pthread_mutex_unlock(&jobqueue->mutex);
}
int addJob(JobQueue* jobqueue, Job* job){
	pthread_mutex_lock(&jobqueue->mutex);
	// empty
	if (jobqueue->front == NULL){
		jobqueue->front = job;
		pthread_mutex_unlock(&jobqueue->mutex);
		return EXIT_SUCCESS;
	}
	// not empty
	Job* newJob = jobqueue->front;
	while(newJob->next != NULL){
		newJob = newJob->next;
	}
	newJob->next = job; // pushed to back
	
	pthread_mutex_unlock(&jobqueue->mutex);
	return EXIT_SUCCESS;
}

// Selecting job and running it
int runJob(Job* awaitingJob){
	awaitingJob->state = RUN;

	pid_t rc = fork();
	if (rc < 0){
		debug("Fork failed");
		return EXIT_FAILURE;
	}	else if (rc == 0){

		// Make directory of output files if needed
		struct stat st = {0};
		if (stat("./outputs", &st) == -1){
			mkdir("./outputs", 0777);
		}
		// Open output file
		char outputFileName[20];
		sprintf(outputFileName, "./outputs/output.%d", awaitingJob->id);
		int outputFD = open(outputFileName, O_RDWR | O_CREAT, S_IRWXU | S_IRWXG);
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
	pthread_mutex_lock(&jobqueue->mutex);
	Job* currJob = jobqueue->front;
	while (currJob != NULL){
		// found correct job
		if (currJob->pid == pid){
			currJob->state = DONE;
			currJob->exit = exitStatus;
			pthread_mutex_unlock(&jobqueue->mutex);
			return EXIT_SUCCESS;
		}
		currJob = currJob->next;
	}
	pthread_mutex_unlock(&jobqueue->mutex);
	return 2; // reached end
}
int numJobsRunning(JobQueue* jobqueue){
	pthread_mutex_lock(&jobqueue->mutex);
	int nrunning = 0;
	Job* job = jobqueue->front;
	while (job != NULL){
		job = job->next;
		nrunning++;
	}
	pthread_mutex_unlock(&jobqueue->mutex);
	return nrunning;
}
int selectJobToRun(JobQueue* jobqueue){
	pthread_mutex_lock(&jobqueue->mutex);
	// GOAL: select job to complete, thus "popped" from queue
	if (jobqueue->front == NULL){
		pthread_mutex_unlock(&jobqueue->mutex);
		return 1; // empty
	}
	// not empty
	Job* awaitingJob = jobqueue->front;
	while (awaitingJob->state != WAIT){
		awaitingJob = awaitingJob->next;
		if (awaitingJob == NULL){
			pthread_mutex_unlock(&jobqueue->mutex);
			return 1; // no job able to run, reached end
		}
	}
	// send awaitingJob to other thread execution : state becomes RUN
	pthread_mutex_unlock(&jobqueue->mutex);
	return runJob(awaitingJob);
}
int removeJob(JobQueue* jobqueue, int id){
	pthread_mutex_lock(&jobqueue->mutex);
	// GOAL: remove job from queue, cannot be in RUN state
	if (jobqueue->front == NULL){
		pthread_mutex_unlock(&jobqueue->mutex);
		return 1; // empty
	}
	// not empty, attempt to remove
	Job* awaitingJob = jobqueue->front->next;
	Job* prevJob     = jobqueue->front;
	while (awaitingJob != NULL && awaitingJob->id != id){
		awaitingJob = awaitingJob->next;
		prevJob     = prevJob->next;
	}
	if (awaitingJob == NULL){
		return 1; // reached end, job does not exist
		pthread_mutex_unlock(&jobqueue->mutex);
	} else if (awaitingJob->state == RUN){
		return -1; // job is being run currently
		pthread_mutex_unlock(&jobqueue->mutex);
	} else {
		// actually remove from queue and delete any output file
		prevJob->next = awaitingJob->next;	
		pthread_mutex_unlock(&jobqueue->mutex);
		return EXIT_SUCCESS;
	}
} 

// Displaying jobs for "status" command
int showJobs(JobQueue* jobqueue){
	pthread_mutex_lock(&jobqueue->mutex);
	printf("JOBID\tSTATE\tEXIT\tCOMMAND\n");
	for (Job* currJob = jobqueue->front; currJob != NULL; currJob = currJob->next){
		if (currJob->state == WAIT || currJob->state == RUN){
			printf("%d\t%s\t%s\t%s\n", currJob->id, StateStrs[currJob->state], "-", currJob->commandList);
		} else {
			printf("%d\t%s\t%d\t%s\n", currJob->id, StateStrs[currJob->state], currJob->exit, currJob->commandList);
		}
	}
	pthread_mutex_unlock(&jobqueue->mutex);
	return EXIT_SUCCESS;
}

