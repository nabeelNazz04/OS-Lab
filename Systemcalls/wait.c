#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
void main(){
    printf("Hello World\n");
    int p=fork();
    wait(NULL);//Parent waits until child process executes
    if(p>0){
        printf("Parent");
    }
    else if(p==0){
        printf("Child");
    }
    else{
        printf("Fork error");
    }
}