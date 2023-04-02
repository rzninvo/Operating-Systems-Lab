#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
 
void printHistogram(int *hist, int sample) {
    int i, j;
    for (i = 0; i < 25; i++) {
        for (j = 0; j < (hist[i] / (sample * 1.0)) * 100 ; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    int hist[25];
    srand(time(0));
    for (int i = 0 ; i < 25; i++)
        hist[i] = 0;
    for (int i = 0 ; i < atoi(argv[1]); i++)
    {
        int count = 0;
        for (int j = 0 ; j < 12; j++)
        {
            count = ((rand()%100) <= 49) ? (count + 1): (count - 1);   
        }
        hist[count + 12]++;
    }
    printHistogram(hist, atoi(argv[1]));
}