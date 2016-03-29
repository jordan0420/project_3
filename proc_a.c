#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <bool.h>

#define slot_array_key 1337
#define Num_Companies 5
#define PERMS 0666

// Create a struct that holds a company id and charge
// This allows us to use threads to complete jobs (threads can only take i arg)
typedef struct str_thdata
{
    int company_id;
    int charge;
} thread_data;
// We create a struct that holds a single int
// The rationale behind this is that this allows for changes to be made with minimal impact
typedef struct shm_accounts
{
    int charge;
} company_account;

company_account *acounts;// un-used in program so far

int *slot_array;
pthread_mutex_t lock;
bool done_reading = FALSE
int shm_get(int key, void **start_ptr, int size);
int shm_rm(int shmid);
void move_array(int noneed);
void store_array(void *ptr);

int main()
{
    //pthread_mutex_t lock; // Possibly needs to be global
    int i;
    char c;
    int shmid;
    //char * line=NULL;
    //ssize_t len=0;
    //size_t read;
    
    //pthread_t thread1;
    pthread_t thread2;
    //thread data data;

    int buf;
    //FILE *ifp;

/*********************************************8/
/*
/* Below are the variables actually used
/*
/******************************************/
    int array_id;
    FILE *ifp;
    size_t read;
    char *line = NULL;
    ssize_t len=0;

    // Create a POSIX semaphore
    sem_t* mutex1 = sem_open("/mutex1\0", O_CREAT, 0666,1); //Could use PERMS here
    // Setup the shared memory segment
    array_id = shm_get(slot_array_key, (void **) &slot_array, 1);

    // Create local pthread objects
    pthread_t thread1;
    pthread_t thread2;
    thread_data data;
    pthread_create(&thread2, NULL, (void *) &move_array, (void *)0);

    // Create an pthread semaphore, still POSIX complieant, used in a single process instead of global
    //pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    // Begine the file read here
    ifp = fopen("z_files/data.txt","r");
    if(ifp == NULL)
    {
        printf("Can't open the input files");
        exit(-1);
    }
    while((read = getline(&line, &len, ifp)) != -1)
    {
        data.company_id = atoi(line);
        read = getline(&line, &len, ifp);
        data.charge = atoi(line);
        pthread_create(&thread1, NULL, (void *) &store_array, (void *) &data);
        pthread_join(thread1, NULL);
    }
    done_reading = true;


    sleep(5);
    // Begin cleanup here
    fclose(ifp);
    for(int i=0;i<Num_Companies;i++)
    {
        printf("%i----\n", slot_array[i]);
    }
    if(line)
    {
        free(line);
    }
    pthread_mutex_destroy(&lock);
    shm_rm(array_id);
    sem_close(mutex1);
    //sem_destroy(mutex1);
/*
    //Begin Reading into the file
    ifp = fopen("zfiles/data.txt","r");
    if(ifp == NULL)
    {
        printf("Can't open the input file");
        exit(-1);
    }
*/
// array_id = shm_get(slot_array_key, (void **) &slot_array, i);
    exit(1);
}



int shm_get(int key, void **start_ptr, int size)
{
    int shmid;
    shmid = shmget((key_t) key, size, PERMS | IPC_CREAT);
    (*start_ptr) = (void *) shmat(shmid, (int *) 0,0);
    return shmid;
}

int shm_rm(int shmid)
{
    return shmctl(shmid, IPC_RMID, (struct shmid_ds *) 0);
}
void move_array(int noneed)// Instead of int i should use struct like my previous idea
{
    while(!done_reading
    {
        pthread_mutex_lock(&lock)
    
    }
}
void store_array(void *ptr)
{
    pthread_mutex_lock(&lock);
    thread_data *data;
    data = (thread_data *)ptr;
    slot_array[data->company_id] = (slot_array[data->company_id] + data->charge);
    pthread_mutex_unlock(&lock);
    pthread_exit(0);
}
