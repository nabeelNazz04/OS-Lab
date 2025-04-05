//Process id and parent process id
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
void main(){
    int pid=getpid();
    printf("Pid is %d\n",pid);
    int ppid=getppid();
    printf("PPid is %d",ppid);
}