#include<stdio.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<unistd.h>

int main(){
    int i;
    void *shared_memory;
    char buff[100];
    int shmid;
    shmid=shmget((key_t)234,1034,0666|IPC_CREAT);
    shared_memory=shmat(shmid,NULL,0);
    printf("Process attached at %p",&shared_memory);
    printf("\nEnter some data to write to shared memory\n");
    fgets(buff,sizeof(buff),stdin);
    strcpy(shared_memory,buff);
    printf("You wrote %s\n", (char*)shared_memory);
    return 0;
}