#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
int deadlock=0;
sem_t forks[5];
sem_t bowl1;
sem_t bowl2;
int bowl1un,bowl2un;
int sequencing[]={0,1,2,3,4};

void init_error_checker(int status,int flag){
    if(status){
        printf("Encountered error while executing sem_init()\n");
    }
}
void wait_error_checker(int status,int flag){
    if(status){
        printf("Encountered error while executing sem_wait()\n");
    }
}
void post_error_checker(int status, int flag){
    if(status){
        printf("Encountered error while executing sem_post()\n");
    }
}
void main_error_checker(int status, int type, int flag){
    if(status){
        if(type==-1){
            init_error_checker(status,0);
        }
        else if(type==0){
            wait_error_checker(status,0);
        }
        else if(type==1){
            post_error_checker(status,0);
        }
        else{
            printf("Unkown Error has occured\n");
        }
        exit(1);
    }
}

void *thinking_start(void *philosophers){
    int flag=0;
    int philosopher_pos=*(int*)philosophers;
    philosopher_pos++;
        while(100){
            int temp_b1;
            int temp_status;
            int temp_b2;
            if(!flag){
            printf("Philosopher at Position %d is Thinking right now\n",philosopher_pos);
            sleep(1);
            printf("Philosopher at Position %d is Hungry right now\n",philosopher_pos);
            int i = 0;
            temp_status=sem_wait(&forks[(philosopher_pos-1)%5]);
            main_error_checker(temp_status,0,0);
            printf("Philosoper at Position %d takes the Fork at Position %d\n",philosopher_pos,((philosopher_pos-1)%5)+1);
            i++;
            temp_status=sem_wait(&forks[(philosopher_pos)%5]);
            main_error_checker(temp_status,0,0);
            printf("Philosoper at Position %d takes the Fork at Position %d\n",philosopher_pos,(philosopher_pos%5)+1);
            i++;
            flag=1;
            sem_getvalue(&bowl1,&temp_b1);
            sem_getvalue(&bowl2,&temp_b2);
            }
            if(!!(bowl1un==temp_b1)){
                temp_status=sem_wait(&bowl1);
                main_error_checker(temp_status,0,0);
                printf("Philosopher at Position %d is Eating right now from Bowl1\n",philosopher_pos);
                sleep(1);
                temp_status=sem_post(&bowl1);
                main_error_checker(temp_status,1,1);
                temp_status=sem_post(&forks[(philosopher_pos-1)%5]);
                main_error_checker(temp_status,1,1);
                printf("Philosoper at Position %d puts the Fork at Position %d\n",philosopher_pos,((philosopher_pos-1)%5)+1);
                temp_status=sem_post(&forks[(philosopher_pos)%5]);
                main_error_checker(temp_status,1,1);
                printf("Philosoper at Position %d puts the Fork at Position %d\n",philosopher_pos,((philosopher_pos)%5)+1);
                flag=0;
            }
            else if(!!(bowl2un==temp_b2)){
                temp_status=sem_wait(&bowl2);
                main_error_checker(temp_status,0,0);
                printf("Philosopher at Position %d is Eating right now from Bowl2\n",philosopher_pos);
                sleep(1);
                temp_status=sem_post(&bowl2);
                main_error_checker(temp_status,1,1);
                temp_status=sem_post(&forks[(philosopher_pos-1)%5]);
                main_error_checker(temp_status,1,1);
                printf("Philosoper at Position %d puts the Fork at Position %d\n",philosopher_pos,((philosopher_pos-1)%5)+1);
                temp_status=sem_post(&forks[(philosopher_pos)%5]);
                main_error_checker(temp_status,1,1);
                printf("Philosoper at Position %d puts the Fork at Position %d\n",philosopher_pos,((philosopher_pos)%5)+1);
                flag=0;
            }
    }
}
int main(){
    pthread_t philosophers[5];
    int i = 0;
    int status;
    while(i<5){
        status=sem_init(&forks[i],0,1);
        i++;
        main_error_checker(status,-1,0);
    }
    main_error_checker(sem_init(&bowl1,0,1),-1,0);
    main_error_checker(sem_init(&bowl2,0,1),-1,0);
    sem_getvalue(&bowl1,&bowl1un);
    sem_getvalue(&bowl2,&bowl2un);
    printf("*************************************\n");
    printf("Number of Philosophers: %d\n",5);
    i = 0;
    printf("Number of Forks: %d\n",5);
    printf("*************************************\n");
    while(i<5){
        status=pthread_create(&philosophers[i],NULL,thinking_start,&sequencing[i]);
        i++;
        if(status){
            printf("Error! Error Encountered while creating the threads\n");
            return 1;
        }
    }
    i = 0;
    while(i<5){
        status=pthread_join(philosophers[i],NULL);
        i++;
        if(status){
            printf("Error! Error Encountered while joining the threads\n");
            return 1;
        }
    }
    return 0;
}