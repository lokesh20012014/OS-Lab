#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER 1024
#define SAMPLE_TEXT "YOLO!"

#define err_check(status, msg)\
	if(!(status))\
		fprintf(stderr, "Error %s. Error : %s \n", msg, strerror(errno));\

int main(){
	pid_t childid;
	int childec;
	int fd[2];
	char buffer[MAX_BUFFER];
	
	// Create a pipe
	pipe(fd);

	// Attempt to fork
	childid = fork();
	
	// Parent execution
	if(childid > 0){
		printf("I'm parent. pid : %d\n", getpid());
		
		// Close write pipe
		err_check(0 == close(fd[1]), "closing pipe");
		
		// Read from pipe
		err_check(0 <= read(fd[0], buffer, MAX_BUFFER), "reading pipe");
		
		// Print the data read from pipe
		printf("Read from pipe: %s\n", buffer);

		// Close read pipe
		err_check(0 == close(fd[0]), "closing pipe");

		// Wait for child
		wait(&childec);
		printf("Waited for child, exit code : %d\n", childec);
	}

	// Child execution
	else if(childid == 0){
		printf("I'm child. pid : %d\n", getpid());
		
		// Close read pipe
		err_check(0 == close(fd[0]), "closing pipe");

		// Write to pipe
		err_check(0 <= write(fd[1], SAMPLE_TEXT, sizeof(SAMPLE_TEXT)), "writing pipe");

		// Close write pipe
		err_check(0 == close(fd[1]), "closing pipe");
	}

	// Fork failed so, terminate
	else{
		printf("Failed to fork. Error! \n");
		perror("fork");
		exit(0);
	}
	
	return 0;
}

