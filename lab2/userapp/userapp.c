#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define FIFO0	"/dev/fifo0"
#define FIFO1	"/dev/fifo1"
#define FIFO2	"/dev/fifo2"
#define FIFO3	"/dev/fifo3"

#define TEST_TEXT "Yolo. Let's test this with a long message :D\n"

/******************************************
	Test 1  :  Reading empty fifos
*******************************************/
void test1()
{
	FILE *fp0, *fp1;
	char ch;
	printf("\nCase: \tReading empty fifos\n\n");
	
	// Open fifos
	printf("Opening read ends of fifo 0 and fifo 1\n");
	fp0 = fopen(FIFO1, "r");
	fp1 = fopen(FIFO3, "r");
	
	// Check if opened
	if(fp0 == NULL || fp1 == NULL) {
		perror("Error while opening fifo.\n");
		exit(EXIT_FAILURE);
	}
	
	// Read fifos
	printf("Reading date. Read: \n");
	while((ch = fgetc(fp0)) != EOF )
		printf("%c",ch);
	while((ch = fgetc(fp1)) != EOF )
		printf("%c",ch);

	printf("Closing read ends of fifo 0 and fifo 1\n");
	fclose(fp0);
	fclose(fp1);
}


/******************************************
	Test 2  :  Writing long messages / full fifos
******************************************/
void test2()
{
	FILE *fp0, *fp1;
	char ch;
	pid_t childid;
	printf("\nCase: \tWriting longer data / full fifos behaviour test\n\n");
	
	// Open fifos
	printf("Opening read and write ends of fifo 0\n");
	fp0 = fopen(FIFO0, "w");
	fp1 = fopen(FIFO1, "r");
	
	// Check if opened
	if(fp0 == NULL || fp1 == NULL) {
		perror("Error while opening fifo.\n");
		exit(EXIT_FAILURE);
	}
	
	// Fork a new processes to for reader and writer
	printf("Forking new process for writer and parent as reader\n");
	childid = fork();
	
	// Parent execution - reader
	if(childid > 0){
		// Close write end
		fclose(fp0);
		
		printf("Reading from fifo. Read: \n");
		while((ch = fgetc(fp1)) != EOF )
			printf("%c",ch);
		fclose(fp1);
	}

	// Child execution - writer
	else if(childid == 0){
		// Close read end
		fclose(fp1);
		
		printf("Writing long data to fifo\n");
		fwrite(TEST_TEXT , 1 , sizeof(TEST_TEXT) , fp0);
		fclose(fp0);
	}

	// Fork failed so, terminate
	else{
		printf("Failed to fork.\n");
		exit(0);
	}

}



int main (){
	printf("\n\n*****   Running Test 1 ********");
	test1();
	
	
	printf("\n\n*****   Running Test 2 ********");
	test2();
}
