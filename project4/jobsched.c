/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#include "macros.h"
#include "commands.h"
#include "JobStructures.h"

/* Command Functions */
typedef int (*commandFunc)(char*, char**, int, JobQueue*);

/* Command Line Parsing */
int parseWordsFromLine(char* words[], char line[]){
	words[0] = strtok(line," \t\n");
	int nwords = 1;
	while ((words[nwords] = strtok(0, " \t\n")) != NULL){
		if (nwords == MAX_DISTINCT_WORDS)
			break;
		nwords++;
	}
	words[nwords] = NULL;
	return nwords;
}

// Globals
char PROMPT[] = "> ";
JobQueue* jobqueue; 

// Threads
void handler(int sig){
	int status;
	pid_t pid;
	pid = wait(&status);
	if (WIFEXITED(status)){
		// optional return status analysis: zero on success, 2 on all jobs completed
		indicateCompleteJob(jobqueue, pid, WEXITSTATUS(status));
	} else {
		printf("Job with pid %d did not exit\n", pid);
	}
}
void* schedThreadFunc(void* arg){
	signal(SIGCHLD, handler);
	while(1){
		// optional return status analysis: -1 on failure (ex: fork or exec failure)
		selectJobToRun(jobqueue);
	}
	return NULL;
}

int main(int argc, char* argv[]){
	// Initialize
	jobqueue = JobQueue_create();
	pthread_t schedThread;
	pthread_create(&schedThread, NULL, schedThreadFunc, NULL);

	while (true){
		// Display prompt
		if (fprintf(stdout, "%s", PROMPT) < 0){
			debug("failed to print prompt %s", strerror(errno));
			return EXIT_FAILURE;
		}
		if (fflush(stdout) == EOF){
			debug("failed to flush stdout %s:", strerror(errno));
			return EXIT_FAILURE;
		}	
		char line[MAX_CHARACTER_INPUT];
		char* words[MAX_DISTINCT_WORDS];

		// Parse from prompt
		if (fgets(line, MAX_CHARACTER_INPUT, stdin) != NULL){
			// Get commands (or extra args) in line for status
			char* commandList = malloc(sizeof(line));
			strcpy(commandList, line);
			commandList[strlen(commandList)-1] = 0;
			int nwords = parseWordsFromLine(words, line);
			if (words[0] != NULL){
				commandList = commandList + strlen(words[0]) + 1;
			}

			// Retrieve program
			commandFunc command;
			if (words[0] == NULL){ // spaces or enter key
				continue;
			} else if (streq(words[0], "help")){
				command = &helpFunc;
			} else if (streq(words[0], "submit")){
				command = &submitFunc;
			} else if (streq(words[0], "status")){
				command = &statusFunc;
			} else if (streq(words[0], "wait")){
				command = &waitFunc;
			} else if (streq(words[0], "remove")){
				command = &removeFunc;
			} else if (streq(words[0], "njobs")){
				command = &njobsFunc;
			} else if (streq(words[0], "drain")){
				command = &drainFunc;
			} else if (streq(words[0], "quit")){
				return EXIT_SUCCESS;
			} else {
				fprintf(stderr, "... Command \'%s\' not recognized\n", words[0]);
				continue;
			}
			// Call command:
			int commandReturn = (*command)(commandList, words, nwords, jobqueue);
			if (commandReturn == EXIT_FAILURE){
				// Optional space for general command failure error checking
			}
		} else {
			// Exit prompt
			printf("\n");
			return EXIT_SUCCESS;
		}
	}
}
