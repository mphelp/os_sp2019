#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100

/* Useful Macros */
// Courtesy of Prof. Bui's Systems Programming course
#define debug(M, ...)\
	fprintf(stderr, "%s:%d:%s" M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define errInput(M, ...)\
	fprintf(stderr, "%s: " M "\n", __FILE__, ##__VA_ARGS__)
#define streq(s0, s1) (strcmp((s0), (s1)) == 0)

// name of shell
char SHELL[] = "myshell";

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

/* Command Functions */
typedef int (*commandFunc)(char**);

// usage: start prog ...
int startFunc(char* words[]){
	int status;
	int rc = fork();
	if (rc < 0){
		debug("Fork failed");
		return EXIT_FAILURE;
	}	else if (rc == 0){
		printf("%s: process %d started\n", SHELL, (int)getpid());
		execvp(words[1], &words[1]);
		exit(1);
	} else if (wait(&status) < 0){
		debug("Wait failed");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
// usage: wait
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
// usage: waitfor pid
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
// usage: run prog ...
int runFunc(char* words[]){
	int status;
	int rc_run = fork();
	if (rc_run < 0){
		debug("Fork failed");
		return EXIT_FAILURE;
	}	else if (rc_run == 0){
		execvp(words[1], &words[1]);
		exit(1);
	} else if (waitpid(rc_run, &status, 0) < 0){
		debug("Wait failed");
		return EXIT_FAILURE;
	}
	handleProcStatus(rc_run, status);
	return EXIT_SUCCESS;
}

// usage: watchdog numSeconds prog ...
void sighandler(int signum){}
int watchdogFunc(char* words[]){
	int status;
	int rc_watch = fork();
	if (rc_watch < 0){
		debug("Fork failed");
		return EXIT_FAILURE;
	} else if (rc_watch == 0){ // child
		execvp(words[2], &words[2]);
		debug("Execvp failed");
		exit(1);
	} else { // parent
		signal(SIGCHLD, sighandler);
		int numSeconds = atoi(words[1]);
		if (numSeconds < 0){
			errInput("Watchdog period must be positive # seconds");
			return EXIT_FAILURE;
		}
		if (sleep(numSeconds) > 0){ // child completed
			if (wait(&status) < 0){
				debug("Wait failed");
				return EXIT_FAILURE;
			}	
			// print nothing when time limit not met
		} else { // child took too long
			if (kill(rc_watch, SIGKILL) < 0){
				debug("Kill failed");
				return EXIT_FAILURE;
			} 
			int rc_watch_wait = wait(&status);
			if (rc_watch_wait < 0){
				debug("Wait failed");
				return EXIT_FAILURE;
			}
			printf("%s: process %d exceeded the time limit, killing it...\n", SHELL, rc_watch);
			handleProcStatus(rc_watch, status);
		}
	}
	return EXIT_SUCCESS;
}

/* Command Line Parsing */
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
			} else if (streq(words[0], "quit") || streq(words[0], "exit")){
				return EXIT_SUCCESS;
			} else {
				fprintf(stderr, "%s: Command \'%s\' not recognized\n", SHELL, words[0]);
				continue;
			}
			// Call command:
			int commandReturn = (*command)(words);
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
