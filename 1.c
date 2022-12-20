#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

void *philosopher(void *);
void pickUp(int);
void putDown(int);

pthread_mutex_t forks[5];
pthread_t philosophers[5];

int main() {
	for (int i = 0; i < 5; ++i) {
		pthread_mutex_init(&forks[i], NULL);
	}
	
	for (int i = 0; i < 5; ++i) {
        pthread_create(&philosophers[i], NULL, philosopher, (void *)(i));
	}

	for (int i = 0; i < 5; ++i) {
		pthread_join(philosophers[i], NULL);
	}
	return 0;
}

void *philosopher(void *philNo) {
	while (1) {
		sleep(1);
		pickUp(philNo);
		sleep(1);
		printf("Philosopher %d will eat\n", philNo+1);
		sleep(1);
		putDown(philNo);
	}
}

void pickUp(int philNo) {
	int right = (philNo + 1) % 5;
	int left = (philNo + 5) % 5;
	if (philNo%2==1) {
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), right+1);
		pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), left+1);
		// printf("Philosopher %d will put down their forks\n", (philNo+1));
		// pthread_mutex_unlock(&forks[(philNo + 1) % 5]);
		// pthread_mutex_unlock(&forks[(philNo + 5) % 5]);
	}else {
		pthread_mutex_lock(&forks[left]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), left+1);
		pthread_mutex_lock(&forks[right]);
		printf("Philosopher %d picked up fork %d\n", (philNo+1), right+1);
		// printf("Philosopher %d will put down their forks\n", (philNo+1));
		// pthread_mutex_unlock(&forks[(philNo + 1) % 5]);
		// pthread_mutex_unlock(&forks[(philNo + 5) % 5]);
	}
}

void putDown(int philNo) {
		printf("Philosopher %d will put down their forks\n", (philNo+1));
		pthread_mutex_unlock(&forks[(philNo + 1) % 5]);
		pthread_mutex_unlock(&forks[(philNo + 5) % 5]);
}