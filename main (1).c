#include<stdio.h>
#include<error.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#define N 4
void main()
{
    int full,empty,mutex,pid,i;
    key_t key;
    void down(int);
    void up(int);
    
    key=ftok("eg.txt",'b');
    mutex=semget(key,1,IPC_CREAT|0666);
    printf("semaphore mutex created with id %d",mutex);
    semctl(mutex,0,SETVAL,1);
    printf("\nmutex initialized as 1\n");
    
    key=ftok("eg.txt",'c');
    full=semget(key,1,IPC_CREAT|0666);
    printf("semaphore mutex created with id %d",full);
    semctl(full,0,SETVAL,0);
    printf("\nfull initialized as 0\n");   
    
    key=ftok("eg.txt",'b');
    empty=semget(key,1,IPC_CREAT|0666);
    printf("semaphore empty created with id %d",empty);
    semctl(mutex,0,SETVAL,N);
    printf("\nempty initialized as N\n");
    fflush(stdout);    
    
    pid=fork();
    if(pid>0)
    {
        printf("producer running\n");
        for(i=0;i<N;i++)
        {
            down(empty);
            down(mutex);
            printf("producer producing item %d\n",i);
            up(mutex);
            up(full);
            sleep(2);
        }
    }
    else if(pid==0)
    {
        printf("consumer running\n");
        for(i=0;i<N;i++)
        {
            down(full);
            down(mutex);
            printf("consumer fetching item %d\n",i);
            up(mutex);
            up(empty);
            sleep(2);
        }
    }
    else
    printf("process creation error");
}
void down(int id)
{
    struct sembuf s[1]={0,-1,0};
    if(semop(id,&s[0],1)==-1)
    {
        printf("Error");
    }
    
}
void up(int id)
{
    struct sembuf s[1]={0,1,0};
    if(semop(id,&s[0],1)==-1)
    {
        printf("Error");
    }
    
}

