#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread>
#include <string.h>
#include <semaphore.h>
#include <fnctl.h>

int  slot_array[5];
int main()
{
    int smid;
    key_t key;
    char * shm;
    char *s;

    key = 1337;

    if(shmid = shmget(key, sizeof(slot_array), IPC_CREAT | 0666) <0)
    {
        perror("shmget")
        exit(1);
    }
    *slot_array = (int*) shmat
