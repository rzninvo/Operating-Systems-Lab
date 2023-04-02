#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
 
void printHistogram(int *hist, int sample) {
    int i, j;
    int sum = 0;
    for (i = 0; i < 25; i++) {
        for (j = 0; j < (hist[i] / (sample * 1.0)) * 100 ; j++) {
            printf("*");
        }
        sum += hist[i];
        printf("\n");
    }
    printf("\nSamples = %d", sum);
}

int main(int argc, char** argv)
{
    srand(time(0));
    int segment_id = shmget(IPC_PRIVATE,25 * sizeof(int),S_IRUSR | S_IWUSR);
	int* hist =(int*) shmat(segment_id, NULL, 0);

    for (int i = 0 ; i < 25; i++)
        hist[i] = 0;

    for (int z = 0; z < 4; z++)
    {
        if (fork() > 0){
            wait(NULL);
            for (int i = 0 ; i < (atoi(argv[1]) / 8); i++)
            {
                int count = 0;
                for (int j = 0 ; j < 12; j++)
                {
                    count = ((rand()%100) <= 49) ? (count + 1): (count - 1);   
                }
                hist[count + 12]++;
            }
        }
        else
        {
            for (int i = 0 ; i < (atoi(argv[1]) / 8); i++)
            {
                int count = 0;
                for (int j = 0 ; j < 12; j++)
                {
                    count = ((rand()%100) <= 49) ? (count + 1): (count - 1);   
                }
                hist[count + 12]++;
            }
            exit(0);
        }
    }
    printHistogram(hist, atoi(argv[1]));
}