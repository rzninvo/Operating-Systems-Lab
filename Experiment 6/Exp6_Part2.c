#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
  
pthread_t philosopher[5]; 
pthread_mutex_t chopstick[5]; 
  


void* Action(void* arg) 
{ 
    int id = *((int*)arg);
    free(arg);
    int c0, c1;
    sleep(1);
    printf("Philosopher %d is thinking!\n", id + 1);
    if (id == 4)
    {
        pthread_mutex_lock(&chopstick[4]); 
        pthread_mutex_lock(&chopstick[0]); 
        c0 = 4;
        c1 = 0;
    }
    else
    {
        c0 = id;
        c1 = id + 1;
        pthread_mutex_lock(&chopstick[id]); 
        pthread_mutex_lock(&chopstick[id+1]); 
    }
    
    printf("Philosopher %d is using chopstick[%d] and chopstick[%d]!\n", id + 1, c0, c1);
    sleep(1);
    if (id == 4)
    {
        pthread_mutex_unlock(&chopstick[4]); 
        pthread_mutex_unlock(&chopstick[0]); 
    }
    else
    {
        pthread_mutex_unlock(&chopstick[id]); 
        pthread_mutex_unlock(&chopstick[id+1]); 
    }
    printf("Philosopher %d has finished eating!\n", id + 1);
    return NULL; 
} 
  
int main(void) 
{ 
    int counter;
    int error; 
  
    counter = 0;
    for (int i = 0 ; i < 5; i++)
    {
        if (pthread_mutex_init(&chopstick[i], NULL) != 0) { 
            printf("\n mutex init has failed\n"); 
            return 1; 
        }   
    }
  
    while (counter < 5) { 
        int *arg = malloc(sizeof(*arg));
        *arg = counter;
        error = pthread_create(&(philosopher[counter]), 
                               NULL, 
                               Action, arg); 
        if (error != 0) 
            printf("\nThread can't be created :[%s]", 
                   strerror(error)); 
        counter++;
    } 
  
    for (int i = 0 ; i < 5; i++)
    {
        pthread_join(philosopher[i], NULL); 
    }
    for (int i = 0 ; i < 5; i++)
        pthread_mutex_destroy(&chopstick[i]); 
  
    return 0; 
} 