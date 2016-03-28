#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <fnctl.h>


#define SHMSZ 1024
typedef struct str_thdata
{
    int company_id;
    int charge;
} thread_data;
void store_array(void *ptr);

int slot_array[5];
pthread_mutex_t lock;
int main(int argc, char* argv[])
{
    // Shared memory Creation
    char c;
    int shmid;
    key_t key;
    char shm;
    char *s;
    //int slot_array[5];
    // Name the shared memory segment
    // Create the Semaphores

    sem_t* mutex1 = sem_open("mutex1\0", O_CREAT, 0666,1);

    key = 1337;
    //create the segment;
    if(shmid = shmget(key, sizeof(slot_array), IPC_CREAT | 0666) < 0)
    {
        perror("shmget");
        exit(1);
    }
    *slot_array = (int*) shmat(shmid, (void*)0,0);
   // if(slot_array=(int *)shmat(shmid,(char *)0,0)==(int*)-1)
   // {
    
    // Attach the segment
   // if (shm = shmat(shmid, NULL, 0)==(char *) -1)
   // {
   //     perror("shmat");
   //     exit(1);
   // }

   // slot_array[=(int *)shmat(shmid,(char *)0,0);
    //int *slot_array[5];
    //slot_array = shmat(shmid, (void *)0,0);

    char * line=NULL;
    ssize_t len = 0;
    size_t read;

    pthread_t thread1;
    thread_data data;



    //initialize the mutex
    if (pthread_mutex_init(&lock, NULL) !=0)
    {
        printf("\n Mutex Initialization Failed");
        return 1;
    }

    // Read from file
    int buf;
    FILE *ifp;
    ifp = fopen("z_files/data.txt", "r");
    if(ifp == NULL)
    {
        printf("Can't open the input file");
        exit(-1);
    }


    while ((read = getline(&line, &len, ifp)) != -1)
    {
        data.company_id = atoi(line);
        read = getline(&line, &len, ifp);
        data.charge = atoi(line);

        pthread_create(&thread1, NULL, (void *) &store_array, (void *) &data);
        pthread_join(thread1, NULL);
    }

    // Begin Cleaning up
    fclose(ifp);
    for(int i=0; i<5;i++)
    {
        printf("%i\n",slot_array[i]);
    }
    if(line)
    {
        free(line); //Clear the Buffer
    }
    pthread_mutex_destroy(&lock);

    exit(1);
}

void store_array(void *ptr)
{
    pthread_mutex_lock(&lock);
    thread_data *data;
    data = (thread_data *) ptr;
    slot_array[data->company_id] = (slot_array[data->company_id] + data->charge);
    pthread_mutex_unlock(&lock);
//    printf("%i will be billed: %i\n", data->sponsor, data->charge);
    pthread_exit(0);
}
