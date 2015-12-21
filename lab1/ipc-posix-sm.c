#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define MAX_BUFFER 1024
#define SAMPLE_TEXT "YOLO!"
#define SMNAME "/yummysaahare"

#define err_check(status, msg)\
	if(!(status))\
		fprintf(stderr, "Error %s. Error : %s \n", msg, strerror(errno));\


int main(){
	pid_t childid;
	int childec;
	int smd;
	char buffer[MAX_BUFFER];

	// Create a SMO
	smd = shm_open(SMNAME, O_CREAT | O_RDWR, 0644);
	err_check(0 <= smd, "creating SMO");
	
	// Adjust size of the SMO
	err_check(0 <= ftruncate(smd, MAX_BUFFER), "setting SMO size");	
	
	// Attempt to fork
	childid = fork();
	
	// Parent execution
	if(childid > 0){
		printf("I'm parent. pid : %d\n", getpid());
		
		// Map SMO to process memory address
		err_check(MAP_FAILED != mmap(0, MAX_BUFFER, PROT_READ, MAP_SHARED, smd, 0), "mapping SMO to process memory");
		
		// Read from queue
		err_check(0 <= read(smd, buffer, MAX_BUFFER), "reading SMO");

		// Print the read data
		printf("Data received : %s \n", buffer);
		
		// Unmap SMO from local memory
		err_check(0 <= munmap(0, MAX_BUFFER), "unmapping SMO from local memory");		
		
		// Unlink SMO
		err_check(0 <= shm_unlink(SMNAME), "unlinking SMO");
		
		// Wait for child
		wait(&childec);
		printf("Waited for child, exit code : %d\n", childec);
	}

	// Child execution
	else if(childid == 0){
		printf("I'm child. pid : %d\n", getpid());
		
		// Map SMO to process memory address
		err_check(MAP_FAILED != mmap(0, MAX_BUFFER, PROT_READ, MAP_SHARED, smd, 0), "mapping SMO to process memory");
		
		// Unmap SMO from local memory
		err_check(0 <= munmap(0, MAX_BUFFER), "unmapping SMO from local memory");
		
		// Write to SMO
		err_check(0 <= write(smd, SAMPLE_TEXT, sizeof(SAMPLE_TEXT)), "writing SMO");
	}

	// Fork failed so, terminate
	else{
		printf("Failed to fork. Error code : %d \n", errno);
	}
	
	return 0;
}

