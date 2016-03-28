/* It is important to note that the locking mechanisms do not work in a cygwin environment */
#include <stdio.h> 
#include <stdlib.h>
#include <signal.h>        // Adds a signal handler for ^C interrupts
#include <string.h>        // Used for the naming of philosophers
#include <unistd.h>       // Used for the sleep function
#include <sys/wait.h>    //|Used for the wait function
#include <fcntl.h>          // Used for the "Semaphore" type structure


// Define colors for better readability
// This uses standard ansii code and is
// therefore portable and should work on all posix
// compliant terminals
#define KNRM "\x1B[0m"  // Returns to normal
#define KRED "\x1B[31m" // Red
#define KGRN "\x1B[32m" // Green
#define KYEL "\x1B[33m" // Yellow
#define KBLU "\x1B[34m" // Blue
#define KMAG "\x1B[35m" // Magenta
#define KCYN "\x1B[36m" // Cyan
#define KWHT "\x1B[37m" // White
#define KCLS "\033c"    // Clears the screen

// Closest thing to a class in C
struct Philosopher
{
	const char name[13];
	int FORK_L; // Number of the left fork
	int FORK_R;// Number of the right fork
	int did_eat;  // Tracks how many times Philosopher has eaten
	int thought; // Tracke how many times Philosopher has thought
	int hungry; // Tracks how many times Philosopher has been hungry
};

// Proto types
int think(struct Philosopher *phil);
int hungry(struct Philosopher *phil);
void eating(struct Philosopher *phil);
int lock(int fork);
int unlock(int fork);

// Creates a Global for reference in each function without passing
struct Philosopher philosophers[5]; 

// Returns a random number no greater than toplevel
int getrand(int toplevel)
{
        time_t t;
        //srand((unsigned) time(&t));
        srand(time(NULL)^ getpid()); // Randomizes the seed as we are dealing with forks()
        return (rand() % toplevel);
}
// Returns  once the Philosopher completes a  sleep
// Takes a reference to  the created philosophers array as arguement
int think(struct Philosopher *phil)
{
		printf("%s%s is now thinking\n",KCYN,phil->name); // Output with pretty colors
        phil->thought++; // Increments the thought counter
        sleep(getrand(10)); // "Think"
        printf("%s%s is now hungry\n",KBLU,phil->name); // More output with pretty colors
        return 0;
}
// The hungry function tries to pick up one fork at a time,
// if the fork is in use it returns a fail
// otherwise it tries the next fork
// on a fail it returns the fork to a free state
// Takes a reference to  the created philosophers array as arguement
int hungry(struct Philosopher *phil)
{
        phil->hungry++; // Increment hungry counter
        if(lock(phil->FORK_L)<0) // If locking the left fork returns -1 (already in use)
        {
				printf("\n\t%s%s has not picked up his left chopstick: %i\n", KMAG,phil->name, phil->FORK_L); // Outputs the chopstick that was not picked up
                return -1; // Fail and return to thinking
        }
        printf("\n\t%s%s has picked up his left chopstick: %i\n",KCYN,phil->name,phil->FORK_L); // Outputs the chopstic that was picked up
        if(lock(phil->FORK_R)<0) // If locking the right fork returns -1 (already in use)
        {
				printf("\n\t%s%s has not picked up his right chopstick: %i\n", KMAG,phil->name, phil->FORK_R); // Outputs the fork that was not picked up
                unlock(phil->FORK_L); // Fail and put the left fork back down
                return -1; // Fail and return to the thinking state
        }
        printf("\n\t%s%s has picked up his right chopstick: %i\n",KCYN,phil->name,phil->FORK_R); // Outputs the second shopstick that was picked up
        return 0; // returns success and the eating can start
}
// Performs the eating action with a random time
// Takes a reference to  the created philosophers array as arguement
void eating(struct Philosopher *phil)
{
        phil->did_eat++; // Increment the eating counter
        printf("%s%s is now eating\n",KGRN,phil->name); // Outputs pretty collors that the Philosopher is eating
        sleep(getrand(10)); // Sleeps for a given time to simulate eating
        unlock(phil->FORK_L); // Returns the left chopstick
        unlock(phil->FORK_R); // Returns the right chopstick
        printf("\n\t%s%s is done eating and has put down chopsticks: %i, %i\n",KWHT,phil->name,phil->FORK_L,phil->FORK_R); // announces that the Philosopher is cone eating
        return; // The eating function is finished
}

// Creates a  semaphopre like lock
// Takes the fork number to lock as an integer
int lock(int fork)
{
        int fd; // The file descriptor
        char *filename[11]; // A container for the length of the filename
		sprintf(filename,"/tmp/fork.%i", fork); // A "cheap" way to cast ints to C string arrays
															// The file will be named fork.x where x is the number of the chopstick
//        strcpy(filename,"/tmp/fork.%i",fork);
        mode_t mode=O_WRONLY; // The mode that the file is created with
        fd=creat(filename,mode); // Creates the file and thus the lockout
        close(fd); // Closes the file as we are only using it to lock other Philosophers out
        return fd; // Returns fd as -1 would mean fail
}
//  Creates a semaphore like unlock
//  Takes the fork number to unlock as an integer
int unlock(int fork)
{
        int fd; // The file descriptor
        char *filename[11]; // A container for the length of the filename
        sprintf(filename,"/tmp/fork.%i",fork); // A "cheap" way to cast ints to C string arrays
        fd = unlink(filename); // Unlocks the file
        return fd; // Returns the fd as a -1 would mean fail
}

//. The main of the program
int main()
{       
        pid_t pids[5]; // An array of pids for clean creation and destruction of forks
        int num_forks=5; // The numner of chopsticks at the table
		int j; // A counter for the creation of  forks and all of the program
		
        // Set up philosophers as:
        /* Philosopher 1, right fork 0, left fork 1
         * Philosopher 2, right fork 1, left fork 2
         * Philosopher 3, right fork 2, left fork 3
         * Philosopher 4, right fork 3, left fork 4
         * Philosopher 5, right fork 4, left fork 0
         */
		// Create all of the seperate forks
		// Will then initialize all the Philosophers
        for(j=0;j<num_forks;j++)
        {
				// If the creation of forks fail
                if((pids[j] = fork())<0)
                {
                        printf("%s%sError creating forks %s",KCLS,KRED,KNRM); // Clears the screen and prints a red error message
                }
				// If the creation of forks succeeds
                else if(pids[j]==0)
                { 
							char *philname[13]; // The container for the Philosophers name where the name is Philosopher X and X is the child that claimed that instance
							sprintf(philname,"Philosopher %i",(char)j+1); // A "cheap" way to cast int to C string array
																							// (char)j+1 creates Philosopher 1-5 apposed to 0-4
							strcpy(philosophers[j].name,philname); // Copies the created name into the container
							philosophers[j].FORK_L=j; // Sets the left hand fork for the Philosopher
							philosophers[j].FORK_R=((j+1)%5); // Sets the right hand fork
																				// Use mod to get 0 as last fork
							philosophers[j].did_eat=0; // Initializes the eat counter to 0
							philosophers[j].thought=0; // Initializes the thought counter fo 0
							philosophers[j].hungry=0; // Initializes the hungry counter to 0

							// An infinite loop to cycle through think, get hungry, eat, repeat
							 int complete = 0; // A counter to complete the program
							 while(complete < 3) // Runs untill there hace been 10 people who have eaten
							{
							think(&philosophers[j]); // Calls the think function sends a single object
							if((hungry(&philosophers[j]) )== 0) // Calls the hungry function where the Philosopher tries to pick up chopsticks a 
																			 // A fail (-1)  returns the Philosopher to the thinking state
							{
								complete++; // Adds 1 to the counter
								printf("%s%i",KRED, complete);
								eating(&philosophers[j]); // Calls the eating function since the philosopher has both chopsticks
							}
							}
							exit(0); // Escapes the forks() for a clean exit
				} // entds else
		}// end for
	int z; //clean unlocking counter
	for(z=1;z<num_forks;z++)
	{
		unlock(z); // Unlocks the forks
	}
	exit(0); // Ends the program
}
