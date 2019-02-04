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
// process status handler
void handleProcStatus(int pid, int status){
	if (WIFEXITED(status)){
		printf("%s: process %d exited normally with status %d\n", SHELL, pid, WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)){
		printf("%s: process %d killed by signal %d\n", SHELL, pid, WTERMSIG(status));
	} else if (WIFSTOPPED(status)){
		printf("%s: process %d stopped by signal %d\n", SHELL, pid, WSTOPSIG(status));
	} else {
		printf("%s: process %d exited in an obscure way\n", SHELL, pid);
	}
}
// command func pointer
typedef int (*commandFunc)(char**);

int startFunc(char* words[]){
	int status;
	int rc = fork();
	if (rc < 0){
		debug("Fork failed");
		exit(1);
	}	else if (rc == 0){
		printf("%s: process %d started\n", SHELL, (int)getpid());
		execvp(words[1], &words[1]); // run prog
	} else {
		int rc_wait = wait(&status);
	} 
	return EXIT_SUCCESS;
}
int waitFunc(char* words[]){
	int status;
	int rc_wait = wait(&status);
	if (rc_wait < 0){
		printf("%s: No children.\n", SHELL);
		return EXIT_FAILURE;
	} 
	handleProcStatus(rc_wait, status);	
	return EXIT_SUCCESS;
}
int waitforFunc(char* words[]){
	int status;
	if (words[1] == NULL){
		fprintf(stderr, "%s: No pid specified\n", SHELL);
		return EXIT_FAILURE;
	}
	int rc_waitfor = waitpid(atoi(words[1]), &status, 0);
	if (rc_waitfor < 0){
		printf("%s: No such process.\n", SHELL);
		return EXIT_FAILURE;
	} 
	handleProcStatus(rc_waitfor, status);
	return EXIT_SUCCESS;
}
int runFunc(char* words[]){
	int status;
	int rc_run = fork();
	if (rc_run < 0){
		debug("Fork failed");
		exit(1);
	}	else if (rc_run == 0){
		execvp(words[1], &words[1]); // run prog
	} else {
		int rc_waitfor = waitpid(rc_run, &status, 0);
	}
	handleProcStatus(rc_run, status);
	return EXIT_SUCCESS;
}
int watchdogFunc(char* words[]){
	printf("Watchdogging ...\n");
	return EXIT_SUCCESS;
}

// helper functions
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
	words[nwords] = NULL;
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
			commandFunc command;

			// Retrieve program
			if (words[0] == NULL){ // spaces or enter key
				continue;
			} else if (streq(words[0], "start")){
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
				fprintf(stderr, "%s: Command \'%s\' not recognized\n", SHELL, words[0]);
				continue;
			}
			int commandReturn = (*command)(words);

			// Error check command (exit if failed)


		} else {
			printf("\n");
			return EXIT_SUCCESS;
		}
	}
}
