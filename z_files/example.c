#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]){
int segment_id;
int *shared_memory[10];
const int size = 9096;

    segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR); 
    printf("argc = %d", argc);
    pid_t pid;
    pid=fork();

    if(pid == 0){ 
        /*child process*/
        /*writes child fib array*/
        int fibArray[10];
        int value = 10;
        fibArray[0]=1;
        fibArray[1]=2
        int j;
        for(j=2;j<value+1;j++){
            fibArray[j]=fibArray[j-1]+fibArray[j-2];            
        }    
        int i;        
        for(i=0; i<value+1; i++){
            printf("\nChild %d", i+1);
            printf("= %d",fibArray[i]);
        }
        /*child fib array ends*/
 
        /*writes the shared fib array*/
        *shared_memory[0]=1;
        *shared_memory[1]=2;
        int k;
        for(k=2;k<value;k++){
            *shared_memory[k] = *shared_memory[k-1] + *shared_memory[k-2];    
            printf("\n%d", k);
        }    
        int l;        
        for(l=0; l<value; l++){
            printf("\nChild shared %d", l+1);
            printf("= %d", *shared_memory[l]);
        }
        return 0;
    }
    else if(pid > 0){ /*parent process*/
        wait(NULL);
        int value = 10;    
        /*printf("\nPARENT: value = %d \n\n", value); /*LINE A*/
        int i;        
        for(i=0; i<value+1; i++){
            printf("\nParent %d", i+1);
            printf("= %d",*shared_memory[i]);
        }    
        return 0;
    } 

    /*segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
    shared_memory = (int *) shmat(segment_id, NULL, 0);
    /*sprintf(shared_memory, "Hi there!");
    printf("*%s\n", shared_memory);*/
    shmdt(shared_memory);
    /*shmct1(segment_id, IPC_RMID, NULL);*/
    return 0;
}
