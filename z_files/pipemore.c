/*
 ============================================================================
 Name        : Project_1.c
 Author      : s4ndm4n
 Version     : 1.0B
 Copyright   :
 Description : Using Pipes and Dup in C, Ansi-style
 ============================================================================


/*
 * Write a main program that takes as the only command line argument the name of an input file
 * Setup an unnamed pipe, create a child process, setup stdin of child to read from pipe
 * and stdout of the parent to write to the pipe
 * in the child exec (execl) more  parent will open file and read bufsiz characters at a time
 * parent exits after child is complete
 */


//[1] Includes for regular C tasks as well as Bare metal control
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//[2] Define values for the termination ends of loops
#define READ_END 0
#define WRITE_END 1


//[3] Main program takes args from cmd
int main (int argc, char *argv[])
{

	//[4] Ensure the correct arguement is given
	if(argc == 2)// If the number of arguments supplied is correct
	{
		printf("The arguement supplied is %s\n", argv[1]);
	}
	else if (argc == 1)// If there are too few arguments,
	{
		printf("There are not enough arguements\n");
		printf("Usage: progname <filename>\n");
		exit(2);// Exit now
	}
	else if(argc >2)// If too many arguments are given
	{
		printf("Too many arguments,\n");
		printf("Did you mean to use \"\\\"?");// If the filename has a space then \ should be used (the\ path\ name/file.txt)
		exit(2);// Exit now
	}

    //[5] Variable Initialization
	int BUFF_SZ = 0;//BUFSIZ;// Sets BUFF_SZ to the system BUFSIZ ( As dictated by system and compiler )
	_Bool complete = false;// A Bool value for use in the buffer creation if BUFF_SZ is not set
	pid_t fork_return;// Process ID of the forks
	int returned;// Used to determine when parent will exit
    FILE *infile;// The allocation of a file into memory
	char c;// Character of text from file

    // The pipe variables
    int fildes[2];// File descriptors
    int status; // The pipe Status
    // Status will return -1 if there is an error
    status = pipe(fildes);

    // Catch if filename given is correct
    int errnum;
    infile = fopen (argv[1], "r");
    if (infile == NULL)// If infile returns NULL there is an error
    	{
        	errnum = errno;
        	fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
        	exit(-2);
    	}
    fclose(infile);

    // Forks the process and returns a value
    fork_return = fork();

    // If fork fails
    if (fork_return == -1)
    	{
            printf("Error in Fork");
            exit(1);
        }

	//[6] sets up the buffer for later use
		// if buffer is set then use buffer size
		// if buffer is not set then the default will be the exact size of the file input
	int buf_count = 0;
	_Bool count_stop = false;
	while (count_stop == false)
	    {
	        if (BUFF_SZ > 0)// If BUFF_SZ was set previously
	    		{
	    			buf_count = BUFF_SZ;
	    			count_stop = true;
	    		}
	    	else // If BUFF_SZ was not set
	    		{
	    			infile = fopen(argv[1], "r");
	    			while (count_stop == false)
	    				{
	    					c = fgetc(infile);

	    					if(c==EOF)// If end of file is reached
								{
									count_stop = true; // Stop
								}
	    					else
								{
									buf_count++; // Keep going
								}
	    				}
	    			fclose(infile);// Close the infile
	    		}
	        count_stop = true;// Stop
	    }
	char buf[buf_count]; // Sets the actual buffer size for the Child and parent

	//[7] This is the child process
	if (fork_return == 0)
		{

			close(fildes[1]);// Close stdout
			dup2(fildes[0],0);// Copy stdin to pipe out
			execl("/usr/bin/more", "more",0);// Exec the "more" program
			close(fildes[0]);// Close the stdin file descriptor
		}// End child

	//[8]This is the parent process
	if (fork_return > 0)
		{printf("the program gets here");

			int fh;// File variable for manipulation
			int reads;// Integer for a while loop
			fh = open(argv[1], O_RDONLY);// Open file for reading into buffer

			// Parent gets here and replaces stdout with pipe in
			dup2(fildes[1], 1);

			// Closes the unneeded part of the pipe
			close(fildes[0]);

			//[9] While not the end of file (EOF) continue to read into buffer
			while (reads = read(fh, buf, sizeof(buf)))
				{
					buf[reads] = '\0';// Adds a NULL byte to create mutable Strings
					write(fildes[1],buf,sizeof(buf));// Writes to pipe
				}
			close(fh);// Closes the file
			close(fildes[1]);// Closes the pipe in
			close(1);// Closes the stdout
			//[10] Waits for the child to close before closing gracefully
            do
                returned = wait(NULL);
            while (returned != fork_return);
            return 0;
	}// End parent
}// End main
