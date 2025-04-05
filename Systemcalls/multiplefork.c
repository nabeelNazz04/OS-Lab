#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
void main(){
    printf("Hello\n");
    fork();
    fork();
    fork();
    printf("Hi\n");
}