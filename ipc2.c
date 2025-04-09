#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>

void main(){
    char buff[100];
    void *shared_memory;
    int shmid=shmget((key_t)234,1034,0666|IPC_CREAT);
    shared_memory=shmat(shmid,NULL,0);
    printf("Data entered in the shared memory is %s\n",(char*)shared_memory);
}
