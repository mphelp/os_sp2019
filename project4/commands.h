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
int submitFunc(char* words[]){
	Job* job1 = Job_create(words);
	Job* job2 = Job_create(words);

	printf("job1 id: %d\n", job1->id);
	printf("job1 word0: %s\n", job1->words[0]);
	printf("job1 word1: %s\n", job1->words[1]);
	printf("job2 id: %d\n", job2->id);
	printf("job2 word0: %s\n", job2->words[0]);
	printf("job2 word1: %s\n", job2->words[1]);

	return EXIT_SUCCESS;
}
int statusFunc(char* words[]){

	return EXIT_SUCCESS;
}
int waitFunc(char* words[]){

	return EXIT_SUCCESS;
}
int removeFunc(char* words[]){

	return EXIT_SUCCESS;
}
int njobsFunc(char* words[]){

	return EXIT_SUCCESS;
}
int drainFunc(char* words[]){

	return EXIT_SUCCESS;
}


