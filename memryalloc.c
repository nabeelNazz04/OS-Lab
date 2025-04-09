#include<stdio.h>
#include<stdlib.h>

struct process{
    int size;
    int block;
    int allocated;
};
struct memory{
    int size;
    int occupied;
};
typedef struct process process;
typedef struct memory memory;

void acceptmemory(memory blocks[],int n){
    for(int i=0;i<n;i++){
        printf("Enter the size of block %d:",i+1);
        scanf("%d",&blocks[i].size);
    }
}
void acceptprocess(process p[],int n){
    printf("\n");
    for(int i=0;i<n;i++){
        printf("Enter the size of the process P%d",i+1);
        scanf("%d",&p[i].size);
    }
}
void clearprocess(process p[],int n){
    for(int i=0;i<n;i++){
        p[i].allocated=0;
    }
}
void clearmemory(memory blocks[],int n){
    for(int i=0;i<n;i++){
        blocks[i].occupied=0;
    }
}

void firstfit(memory blocks[],process p[],int np,int nm){
    for(int i=0;i<np;i++){
        for(int j=0;j<nm;j++){
            if(blocks[j].occupied!=1 && p[i].size<=blocks[j].size){
                blocks[j].occupied=1;
                p[i].allocated=1;
                p[i].block=j+1;
                break;
            }
        }
    }
}
void bestfit(memory blocks[],process p[],int np,int nm){
    int block=-1;
    for(int i=0;i<np;i++){
        int small=9999;
        for(int j=0;j<nm;j++){
            if(blocks[j].size<small && blocks[j].occupied != 1 && p[i].size<=blocks[j].size){
                small=blocks[j].size;
                block=j;
            }
        }
        if(block!=-1){
            blocks[block].occupied=1;
            p[i].allocated=1;
            p[i].block=block+1;
        }
    }
}

void worstfit(memory blocks[],process p[],int np,int nm){
    int block=-1;
    for(int i=0;i<np;i++){
        int large=0;
        for(int j=0;j<nm;j++){
            if(blocks[j].size>large && blocks[j].occupied!=1 && p[i].size<=blocks[j].size){
                large=blocks[j].size;
                block=j;
            }
        }
        if(block!=-1){
            blocks[block].occupied=1;
            p[i].allocated=1;
            p[i].block=block+1;
        }
    }
}

void display(process p[],int n){
    for(int i=0;i<n;i++){
        printf("\nP%d",i+1);
        if(p[i].allocated==0){
            printf("Not allocated");
        }
        else{
            printf(" block %d",p[i].block);
        }
    }
}

int main(){
    int np,nm;
    printf("Enter the number of partitions/memory blocks:");
    scanf("%d",&nm);
    memory blocks[nm];
    acceptmemory(blocks,nm);

    printf("Enter the number of processes:");
    scanf("%d",&np);
    process p[np];
    acceptprocess(p,np);

    int op;
    while(1){
        clearprocess(p,np);
        clearmemory(blocks,nm);
        printf("\n1.First fit\n2.Best fit\n3.Worst fit\n4.Exit\nEnter your choice:");
        scanf("%d",&op);
        switch(op){
            case 1:
                firstfit(blocks,p,np,nm);
                break;
            case 2:
                bestfit(blocks,p,np,nm);
                break;
            case 3:
                worstfit(blocks,p,np,nm);
                break;
            case 4:
                exit(0);
            default:
                printf("Invalid");
        }
        display(p,np);
    }

    return 0;
}

