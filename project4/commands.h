#pragma once

/* Commands */
/*
	 submit <command>
	 status
	 wait <jobid>
	 remove <jobid>
	 njobs <n>
	 drain
	 quit
	 help
*/

#include "JobStructures.h"
#include "macros.h"

int helpFunc(char* commandList, char* words[], JobQueue *jobqueue){
	printf("Usage:\n");
	printf("\tsubmit <command> - submit new job to queue\n");
	printf("\tstatus           - get status of all jobs\n");	
	printf("\twait <jobid>     - pause other jobs until chosen job finishes\n");
	printf("\tremove <jobid>   - remove job from job queue if done or waited for\n");
	printf("\tnjobs <n>        - set number of jobs the scheduler can run at once\n");
	printf("\tdrain            - wait until all jobs are in done state\n");
	printf("\tquit             - exit the program, regardless of jobs in queue\n");
	printf("\thelp             - display usage\n");
	return EXIT_SUCCESS;
}
int submitFunc(char* commandList, char* words[], JobQueue* jobqueue){
	// Create and add job
	Job* job = Job_create(commandList, words);
	if (addJob(jobqueue, job) < 0){
		debug("Failed to add job to queue");
		return EXIT_FAILURE;	
	}
	printf("Job %d submitted.\n", job->id);

	return EXIT_SUCCESS;
}
int statusFunc(char* commandList, char* words[], JobQueue* jobqueue){
	// show job in queue
	if (showJobs(jobqueue) < 0){
		debug("Failed to show jobs in queue");
		return EXIT_FAILURE;
	}	
	return EXIT_SUCCESS;
}
int waitFunc(char* commandList, char* words[], JobQueue* jobqueue){

	return EXIT_SUCCESS;
}
int removeFunc(char* commandList, char* words[], JobQueue* jobqueue){
	// temporarily only removes front
	Job* poppedJob = Job_create(commandList, words);
	if (popJob(jobqueue, poppedJob) < 0){
		debug("Failed to pop job from queue");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
int njobsFunc(char* commandList, char* words[], JobQueue* jobqueue){

	return EXIT_SUCCESS;
}
int drainFunc(char* commandList, char* words[], JobQueue* jobqueue){

	return EXIT_SUCCESS;
}


