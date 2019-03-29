#pragma once

#include <inttypes.h>

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

int helpFunc(char* commandList, char* words[], int nwords, JobQueue *jobqueue){
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
int submitFunc(char* commandList, char* words[], int nwords, JobQueue* jobqueue){
	// Create and add job
	if (words[1] == NULL){
		errInput2("Submit requires <command> to be run");
		return EXIT_FAILURE;
	}
	Job* job = Job_create(commandList, nwords, words);
	if (addJob(jobqueue, job) < 0){
		debug("Failed to add job to queue");
		return EXIT_FAILURE;	
	}
	printf("Job %d submitted.\n", job->id);

	return EXIT_SUCCESS;
}
int statusFunc(char* commandList, char* words[], int nwords, JobQueue* jobqueue){
	// show jobs in queue
	if (showJobs(jobqueue) < 0){
		debug("Failed to show jobs in queue");
		return EXIT_FAILURE;
	}	
	return EXIT_SUCCESS;
}
int waitFunc(char* commandList, char* words[], int nwords, JobQueue* jobqueue){
	// wait for given job
	if (words[1] == NULL){
		errInput2("Wait requires <jobid> to specify job");
		return EXIT_FAILURE;
	}
	int id = strtoumax(words[1], NULL, 10);

	// spin until job is done
	Job* currJob = jobqueue->front;
	while (currJob != NULL && currJob->id != id){
		currJob = currJob->next;
	}
	if (currJob == NULL){
		return 1; // not found
	} else if (currJob->id == id){
		// spin!
		while (currJob->state != DONE){}

		// display contents
		printf("JOBID\tCOMMAND\n");
		printf("%d\t%s\n", currJob->id, currJob->commandList);
		// display its output file
		int c;
		FILE* file;
		char inputFileName[20];
		sprintf(inputFileName, "./outputs/output.%d", id);
		file = fopen(inputFileName, "r");
		if (file){
			while ((c = getc(file)) != EOF)
				putchar(c);
			fclose(file);
		}	
		return EXIT_SUCCESS;
	} else {
		return EXIT_FAILURE;
	}
}
int removeFunc(char* commandList, char* words[], int nwords, JobQueue* jobqueue){
	// remove non-running job from queue
	if (words[1] == NULL){
		errInput2("Remove requires <jobid> to specify job");
		return EXIT_FAILURE;
	}
	int id = strtoumax(words[1], NULL, 10);
	if (removeJob(jobqueue, id) < 0){
		errInput2("Cannot remove running job ...");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
int njobsFunc(char* commandList, char* words[], int nwords, JobQueue* jobqueue){
	// set njobs
	if (words[1] == NULL){
		errInput2("njobs requires <jobid> to specify job");
		return EXIT_FAILURE;
	}
	int njobs = strtoumax(words[1], NULL, 10);

	set_njobs(jobqueue, njobs);
	return EXIT_SUCCESS;
}
int drainFunc(char* commandList, char* words[], int nwords, JobQueue* jobqueue){

	return EXIT_SUCCESS;
}


