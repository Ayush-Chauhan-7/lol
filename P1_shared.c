#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/shm.h>  
#include<string.h>  

int main()
{
    char *shared_memory;  
    char buff[6] = "Hello";  
    int shmid;  
    shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT); 
    shared_memory=(char *)shmat(shmid,NULL,0);  
    strcpy(shared_memory,buff);
    return 0; 
}
