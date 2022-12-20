#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/shm.h>  
#include<string.h>

int main()
{
    char *shared_memory;  
    char buff[6];  
    int shmid;  
    shmid=shmget((key_t)2345, 1024, 0666|IPC_CREAT);
    shared_memory=(char *)shmat(shmid,NULL,0);
    printf("Data read from shared memory is : %s\n",shared_memory); 
    return 0;
}
