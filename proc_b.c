#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

#define slot_array_key 1337
#define Num_Companies 5
#define PERMS 0666

int *slot_array;
int shm_get(int key, void **start_ptr, int size);
int shm_rm(int shmid);

int main()
{

    int array_id;
    int out = 0;

    sem_t* mutex1 = sem_open("/mutex1\0", O_CREAT, 0666,1);

    array_id = shm_get(slot_array_key, (void **) &slot_array, 1);

    for(out=0;out<Num_Companies;out++)
    {
        printf("----%i\n",slot_array[out]);
    }

    sem_close(mutex1);
    sem_destroy(mutex1);
    shm_rm(array_id);
    exit(0);
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
    return shmctl(shmid, IPC_RMID, (struct shmid_ds *)0);
}

