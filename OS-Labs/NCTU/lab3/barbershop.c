#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

sem_t semCustomer;
sem_t semBarber;
sem_t mutex;
int n = 10;         //number of chairs in the waiting room...
int waitingCustomer = 0;    //number of customers waiting
int runTime = 1000;     //running time of this program
int curRunningTime = 0;
float avgSerTime, avgDelTime, avgSleep;
time_t t1, t2, t3, t4;
int count = 0;

struct Client {
    int id;
};

void *barber(void *param) {
    while(1) {
        // waiting
        t3 = clock();
        sem_wait(&semCustomer);
        sem_wait(&mutex);
        t4 = clock();
        avgSleep += t4 - t3;
        
        waitingCustomer--;
        t2 = clock();
        avgSerTime += t2 - t1;
        
        // start job
        sem_post(&semBarber);
        sem_post(&mutex);
        t2 = clock();
        avgDelTime += t2 - t1;
        
        count++;
        
        // completed
        //printf("Barber completed job. There are %d customers waiting now...\n", waitingCustomer);
        
    }

    pthread_exit(NULL);  
}

void *customer(void *param) {
    
    float randNum = rand() / (RAND_MAX + 1.0);
    
    sem_wait(&mutex);

    if (waitingCustomer < n) {

        //printf("\%d customers waiting...\n", (*semCustomer.__size)+1);
        
        waitingCustomer++;
        t1 = clock();
        
        sem_post(&semCustomer); 
        sem_post(&mutex);
       
        sem_wait(&semBarber);
    }
    else {
        sem_post(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t curCustomer;
    pthread_t Barber;
    
    int clients, totalCus = 0;
	
    printf("Main thread beginning\n");

    sem_init(&mutex, 1, 1);
    sem_init(&semCustomer, 1, 0);
    sem_init(&semBarber, 1, 0);
    
    pthread_create(&Barber, NULL, barber, NULL);
   
    //Create barber thread. 
    time_t start = clock();
    while (1) {
        struct Client c;
        c.id = ++totalCus;
        
        // create customer thread
        pthread_create(&curCustomer, NULL, customer, &c.id);
        
        time_t end = clock();
        if (end - start >= runTime) break;
    }
    
    printf("Executing end!\n");
    
    printf("avgSerTime: %f\n", avgSerTime / count);
    printf("avgDelTime: %f\n", avgDelTime / count);
    printf("Total Customers: %d\n", totalCus);
    printf("avgSleep: %f\n", avgSleep / count);
    
    return 0;
}