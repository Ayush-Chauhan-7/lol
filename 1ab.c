#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>

void *philosopher(void *,int flag);
sem_t forks[5];
void states(int, int, int flag);
sem_t mutex_lock;

void states(int var, int phil, int flag){   
	char buffer[50]={'\0'};
    flag = 0;
    if(var == flag){
        sprintf(buffer, "Philosopher %d is eating\n", phil);
		write(1, buffer, 50);
    }
    else{
        sprintf(buffer, "Philosopher %d has finished eating\n", phil);
		write(1, buffer, 50);
    }
}

void *philosopher(void *num, int flag)
{
	while(1){
        if(*(int *)num == 4){
		    sem_wait(&forks[(*(int *)num+1)%5]);
            sem_wait(&forks[*(int *)num]);
        }
        else{
            sem_wait(&forks[*(int *)num]);
            sem_wait(&forks[(*(int *)num+1)%5]);
        }
        states(0,*(int *)num,0);
		states(1,*(int *)num,0);
        if(*(int *)num == 4){
		    sem_post(&forks[*(int *)num]);
            sem_post(&forks[(*(int *)num+1)%5]);
        }
		else{
            sem_post(&forks[(*(int *)num+1)%5]);
		    sem_post(&forks[*(int *)num]);
        }
        sleep(1);
	}
}

int main(){
    pthread_t tid[5];
	int num[5];
	int i = 0;
    int j = 0;
    int k = 0;

	while((i++)<5){
		sem_init(&forks[i],0,1);
	}

	while((j++) < 5){
		num[j-1]=j-1;
		pthread_create(&tid[j-1],NULL,philosopher,(void *)&num[j-1]);
	}

	while((k++) < 5){
		pthread_join(tid[k],NULL);
	}
}