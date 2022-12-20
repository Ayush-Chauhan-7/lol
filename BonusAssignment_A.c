#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

sem_t forks[5], mutex_lock;

void *philosopher(void *);
void states(int, int);
int main(){
    pthread_t tid[5];
	int num[5];
	int i = 0;
    int j = 0;
    int k = 0;

	for( ; i<5; i++){
		sem_init(&forks[i],0,1);
	}

	while((j++) < 5){
		num[j-1]=j-1;
		pthread_create(&tid[j-1],NULL,philosopher,(void *)&num[j-1]);
	}

	for( ; k<5; k++){
		pthread_join(tid[k],NULL);
	}
}



void *philosopher(void *num)
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
        states(0,*(int *)num);
		states(1,*(int *)num);
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

void states(int var, int phil){   
	char buffer[50]={'\0'};
    if(var == 0){
        sprintf(buffer, "Philosopher %d is eating\n", phil);
		write(1, buffer, 50);
    }
    else{
        sprintf(buffer, "Philosopher %d has finished eating\n", phil);
		write(1, buffer, 50);
    }
}