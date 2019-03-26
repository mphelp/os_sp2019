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

int helpFunc(char* words[]){
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
int submitFunc(char* words[], JobQueue* jq){
	Job* job = Job_create(words);
	
	return EXIT_SUCCESS;
}
int statusFunc(char* words[], JobQueue* jq){

	return EXIT_SUCCESS;
}
int waitFunc(char* words[], JobQueue* jq){

	return EXIT_SUCCESS;
}
int removeFunc(char* words[], JobQueue* jq){

	return EXIT_SUCCESS;
}
int njobsFunc(char* words[], JobQueue* jq){

	return EXIT_SUCCESS;
}
int drainFunc(char* words[], JobQueue* jq){

	return EXIT_SUCCESS;
}


