#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
void main(){
    printf("Hello\n");
    int p=fork();
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