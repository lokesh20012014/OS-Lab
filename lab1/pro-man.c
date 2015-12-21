#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
	pid_t childid;
	int childec;
	
	// Attempt to fork
	childid = fork();
	
	// Parent execution
	if(childid > 0){
		printf("I'm parent. pid : %d\n", getpid());
		
		// Wait for child
		wait(&childec);
		printf("Waited for child, exite code : %d\n", childec);
	}

	// Child execution
	else if(childid == 0){
		printf("I'm child. pid : %d\n", getpid());
	}

	// Fork failed so, terminate
	else{
		printf("Failed to fork. Error : %s \n", strerror(errno));
		exit(0);
	}
	
	return 0;
}

