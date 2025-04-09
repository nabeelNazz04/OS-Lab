#include<stdio.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<unistd.h>

void main(){
    int i;
    void *shared_memory;
    char buff[100];
    int shmid;
    shmid=shmget((key_t)234,1034,0666|IPC_CREAT);
    printf("Shm id:%d\n",shmid);
    shared_memory=shmat(shmid,NULL,0);
    printf("Process attached at: %p\n",shared_memory);
    printf("Enter some data to enter to the shared memory:\n");
    read(0,buff,100);
    strcpy(shared_memory,buff);
    printf("Entered:%s\n",(char *)shared_memory);
}