#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100

// errors
// Courtesy of Prof. Bui's Systems Programming course
#define debug(M, ...)\
	fprintf(stderr, "%s:%d:%s" M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define errInput(M, ...)\
	fprintf(stderr, "%s: " M "\n", __FILE__, ##__VA_ARGS__)
#define streq(s0, s1) (strcmp((s0), (s1)) == 0)

// name of shell
char SHELL[] = "myshell";

// possible programs the shell understands
char* PROGS[] = {
	"start",
	"wait",
	"waitfor",
	"run",
	"watchdog"
};
// command func pointer
typedef int (*commandFunc)(char**);

int startFunc(char* words[]){
	int rc = fork();
	if (rc < 0){
		debug("Fork failed");
		exit(1);
	}	else if (rc == 0){
		printf("%s: process %d started\n", SHELL, (int)getpid());
		execvp(words[1], &words[1]); // run prog
	} else {
		int rc_wait = wait(NULL);
	}
	return EXIT_SUCCESS;
}

int waitFunc(char* words[]){
	int rc_wait = wait(NULL);
	printf("%s: process %d exited .....\n", SHELL, rc_wait);
	return EXIT_SUCCESS;
}
int waitforFunc(char* words[]){
	printf("Waiting for ...\n");
	return EXIT_SUCCESS;
}
int runFunc(char* words[]){
	printf("Running ...\n");
	return EXIT_SUCCESS;
}
int watchdogFunc(char* words[]){
	printf("Watchdogging ...\n");
	return EXIT_SUCCESS;
}
// prototypes
int checkProgram(char* prog);
void printWords(char* words[]);
void parseWordsFromLine(char* words[], char line[]);

// helper functions
int checkProgram(char* prog){
	int progCount = sizeof(PROGS)/sizeof(PROGS[0]);
	int progValid = -1;
	for (int i = 0; i < progCount; i++){
		if (strcmp(PROGS[i], prog) == 0){
			progValid = 0;
			break;
		}
	}
	return progValid;
}
void printWords(char* words[]){
	for (int w = 0; words[w] != NULL; w++){
		printf("%s ",words[w]);
	}
	printf("\n");
}
void parseWordsFromLine(char* words[], char line[]){
	words[0] = strtok(line," \t\n");
	int nwords = 1;
	while ((words[nwords] = strtok(0, " \t\n")) != NULL){
		if (nwords == MAX_DISTINCT_WORDS)
			break;
		nwords++;
	}
	words[nwords] = 0; // null terminated
}

int main(int argc, char* argv[]){
	// Read from line
	while (true){
		if (fprintf(stdout, "%s> ", SHELL) < 0){
			debug("failed to print prompt %s", strerror(errno));
			return EXIT_FAILURE;
		}
		if (fflush(stdout) == EOF){
			debug("failed to flush stdout %s:", strerror(errno));
			return EXIT_FAILURE;
		}	
		char line[MAX_CHARACTER_INPUT];
		char* words[MAX_DISTINCT_WORDS];
		if (fgets(line, MAX_CHARACTER_INPUT, stdin) != NULL){
			parseWordsFromLine(words, line);
			// words parsed, now do stuff!!
			commandFunc command;

			// Retrieve program
			if (streq(words[0], "start")){
				command = &startFunc;
			} else if (streq(words[0], "run")){
				command = &runFunc;
			} else if (streq(words[0], "wait")){
				command = &waitFunc;
			} else if (streq(words[0], "waitfor")){
				command = &waitforFunc;
			} else if (streq(words[0], "watchdog")){
				command = &watchdogFunc;
			} else {
				errInput("Command not recognized");
			}
			int commandReturn = (*command)(words);
			/* printf("Command Return: %d\n",commandReturn); */

			// Error check command (exit if failed)




			/* if (checkProgram(words[0]) < 0){ */
			/* 	debug("program not recognized"); */
			/* 	return EXIT_FAILURE; */
			/* } */ 
		} else {
			// we've reached the end!
			return EXIT_SUCCESS;
		}
	}
}
