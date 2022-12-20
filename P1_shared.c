#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/shm.h>  
#include<string.h>  
#include <stdbool.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    char *shared_memory;   
    int shmid;  
    shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT); 
    shared_memory=(char *)shmat(shmid,NULL,0);
    char strings[50][7];
    char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 6; j++) {
            strings[i][j] = alphabet[rand() % 52];
        }
        strings[i][6]='\0';
    }
    for(int i=0;i<50;i+=5)
    {
        for(int j=0;j<5;j++)
        {
            strcpy(shared_memory,strings[i+j]);
            while(*shared_memory!=-1);
        }
        printf("Maximum ID recieved: %d\n",i+5);
    }
    return 0;
}
