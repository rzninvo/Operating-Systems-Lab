#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include<time.h>

#define NUMBER_OF_RESOURCES 5
/* this maybe any values >= 0 */
#define NUMBER_OF_CUSTOMERS 5
/* the available amount of each resource */
int available[NUMBER_OF_RESOURCES];
/* the maximum demand of each customer*/
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
/* the remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

pthread_mutex_t waitline[NUMBER_OF_RESOURCES]; 
pthread_mutex_t waitline2[NUMBER_OF_RESOURCES];

int status_check(int customer_num)
{
    int flag = 1;
    for (int i = 0 ; i < NUMBER_OF_RESOURCES; i++)
    {
        if (need[customer_num][i] != 0)
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

int check_customers()
{
    int flag = 1;
    for (int i = 0 ; i < NUMBER_OF_CUSTOMERS; i++)
    {
        flag = status_check(i);
    }
    return flag;
}

int safety_check()
{
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS];
    for (int i = 0 ; i < NUMBER_OF_CUSTOMERS; i++)
        finish[i] = 0;
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
        work[i] = available[i];
    for (int k = 0 ; k < NUMBER_OF_CUSTOMERS; k++)
    {
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        {
            if (finish[i] == 0)
            {
                int flag = 0; 
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) { 
                    if (need[i][j] > work[j]){ 
                        flag = 1; 
                        break; 
                    } 
                } 
    
                if (flag == 0) { 
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) 
                        work[j] += allocation[i][j]; 
                    finish[i] = 1; 
                } 
            }
        }
    }
    int flag = 1;
    for (int i = 0 ; i < NUMBER_OF_CUSTOMERS; i++)
    {
        if ( finish[i] == 0 )
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

void release_resources(int customer_num, int request[])
{
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
    {
        if (need[customer_num][i] == 0)
        {
            printf("Customer[%d] is WAITING in the line for releasing allocation[%d]\n", customer_num, i);
            pthread_mutex_lock(&waitline2[i]);
            printf("Customer[%d] is releasing allocation[%d]\n", customer_num, i);
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            pthread_mutex_unlock(&waitline2[i]);
            printf("Customer[%d] is DONE releasing allocation[%d]\n", customer_num, i);
        }
    }
}

int request_resources(int customer_num, int request[])
{
    int flag = 0;
    for (int i = 0 ; i < NUMBER_OF_RESOURCES; i++)
    {
            if (request[i] <= need[customer_num][i])
            {
                if (request[i] <= available[customer_num])
                {
                    if (safety_check() == 1)
                    {
                        printf("Customer[%d] is WAITING in line for resource [%d]\n", customer_num, i);
                        pthread_mutex_lock(&waitline[i]);
                        printf("Cusotmer[%d] is requesting %d amount of resource[%d]\n", customer_num, request[i], i);
                        available[i] -= request[i];
                        allocation[customer_num][i] += request[i];
                        need[customer_num][i] -= request[i];
                        if (safety_check() == 0)
                        {
                            available[i] += request[i];
                            allocation[customer_num][i] -= request[i];
                            need[customer_num][i] += request[i];
                            printf("Request[%d] of customer[%d] was NOT SAFE\n",  i, customer_num);
                            pthread_mutex_unlock(&waitline[i]);
                            continue;
                        }
                        else
                        {
                            release_resources(customer_num, request);
                            pthread_mutex_unlock(&waitline[i]);
                            printf("Customer[%d] is DONE with his request[%d]=%d\n", customer_num, i, request[i]);       
                        }
                    }
                    else
                    {
                        printf("Request[%d] of customer[%d] is NOT SAFE\n",  i, customer_num);
                        continue;
                    }
                }
            }
            else
            {
                printf("ERROR: customer[%d],request[%d]=%d was too ambitious!\n", customer_num, i, request[i]);
                flag = -1;
            } 
    }
    return flag;
}

void* start_thread(void* arg)
{
    int customer_num = *((int*)arg);
    int request[NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for (int j = 0 ; j < NUMBER_OF_RESOURCES; j++)
            request[j] = (rand() % (maximum[i][j] - 1)) + 1;
    }
    request_resources(customer_num, request);
    printf("\n-----------------\nCustomer[%d] REQUESTS DONE\n---------------\n", customer_num);
}

int main(int argc, char* * argv)
{
    srand(time(0));
    for (int i = 0 ; i < NUMBER_OF_RESOURCES; i++)
    {
        if (pthread_mutex_init(&waitline[i], NULL) != 0) { 
            printf("\n mutex init has failed\n"); 
            return 1; 
        }   
    }
    for (int i = 0 ; i < NUMBER_OF_RESOURCES; i++)
    {
        if (pthread_mutex_init(&waitline2[i], NULL) != 0) { 
            printf("\n mutex init has failed\n"); 
            return 1; 
        }   
    }
    if (argc < NUMBER_OF_RESOURCES)
    {
    printf("not enough arguments\n");
    return EXIT_FAILURE;
    }
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = strtol(argv[i + 1], NULL, 10);
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            maximum[i][j] = (rand() % 8) + 2;
            printf("Customer[%d] maximum %d from RESOURCE[%d]\n", i, maximum[i][j], j);
        }
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
        {
            allocation[i][j] = (rand() % (maximum[i][j] - 1)) + 1;
            printf("Customer[%d] allocated %d from RESOURCE[%d]\n", i, allocation[i][j], j);
        }
    }
    int counter = 0;
    while (1)
    {
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++)
        {
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++)
            {
                need[i][j] = maximum[i][j] - allocation[i][j];
                printf("Customer[%d] NEEDS %d from RESOURCE[%d]\n", i, need[i][j], j);
            }
        }
        if (check_customers())
            break;
        if (safety_check() == 0)
        {
            printf("Safety Check FAULT\n");
            break;
        }
        pthread_t customer[NUMBER_OF_CUSTOMERS]; 
        counter = 0;
        while (counter < NUMBER_OF_CUSTOMERS) { 
        int *arg = malloc(sizeof(*arg));
        *arg = counter;
        int error = pthread_create(&(customer[counter]), 
                               NULL, 
                               start_thread, arg); 
        if (error != 0) 
            printf("\nThread can't be created :[%s]", 
                   strerror(error)); 
        counter++;
        }
        for (int i = 0; i  < NUMBER_OF_CUSTOMERS; i++)
            pthread_join(customer[i], NULL);
    }
    for (int i = 0 ; i < NUMBER_OF_RESOURCES; i++)
        pthread_mutex_destroy(&waitline[i]);
    for (int i = 0 ; i < NUMBER_OF_RESOURCES; i++)
        pthread_mutex_destroy(&waitline2[i]);
    printf("DONE!\n");
    return 0;
}