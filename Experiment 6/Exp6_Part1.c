#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>

#define n 200

int main()
{
    int segment_id = shmget(IPC_PRIVATE,sizeof(int),S_IRUSR | S_IWUSR);
    int segment_id1 = shmget(IPC_PRIVATE,sizeof(int),S_IRUSR | S_IWUSR);
    int* mem =(int*) shmat(segment_id, NULL, 0);
    int* flag =(int*) shmat(segment_id1, NULL, 0);
    *mem = 0;
    *flag = 0;
    if (fork() > 0)
    {
        while ((*mem) < n)
        {
            if ( *flag == 0 || *flag == 3)
            {
                *mem += 1;
                printf("Proccess %d, Counter = %d\n", getpid(), *mem);
                *flag = 1;
            }
        }
    }
    else
    {
        if (fork() > 0)
        {
            while ((*mem) < n)
            {
                if (*flag == 1)
                {
                    printf("Proccess %d, Counter = %d\n", getpid(), *mem);
                    *flag = 2;
                }
            } 
        }
        else
        {
            while ((*mem) < n)
            {
                if (*flag == 2)
                {
                    printf("Proccess %d, Counter = %d\n", getpid(), *mem);
                    *flag = 3;
                }
            }
        }
        
    }
    return 0;
}
