#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

void *philosopher(void *, int flag);
pthread_mutex_t forks[5];
void putDown(int philNo, int flag);
pthread_t philosophers[5];
void pickUp(int philNo, int flag);

void *philosopher(void *philNo, int flag) {
	while (1) {
		sleep(1);
		pickUp(philNo,1);
		sleep(1);
		printf("Philosopher %d will eat\n", philNo+1);
		sleep(1);
		putDown(philNo, 0);
	}
}

void putDown(int philNo, int flag) {
		printf("Philosopher %d will put down their forks\n", (philNo+1));
		pthread_mutex_unlock(&forks[(philNo + 1) % 5]);
		pthread_mutex_unlock(&forks[(philNo + 5) % 5]);
}

void pickUp(int philNo, int flag) {
    int left = (philNo + 5) % 5;
	int right = (philNo + 1) % 5;
    flag = philNo%2;
    switch (flag)
    {
    case 1:
        pthread_mutex_lock(&forks[right]);
        printf("Philosopher %d picked up fork %d\n", (philNo+1), right+1);
        pthread_mutex_lock(&forks[left]);
        printf("Philosopher %d picked up fork %d\n", (philNo+1), left+1);
        break;

    case 0:
        pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), left+1);
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), right+1);
        break;
    }
}

int main() {
    int i = 0;
	while(i++<5) {
		pthread_mutex_init(&forks[i], NULL);
	}
	i = 0;
	while(i++<5) {
        pthread_create(&philosophers[i], NULL, philosopher, (void *)(i));
	}
    i = 0;
	while(i++<5) {
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}