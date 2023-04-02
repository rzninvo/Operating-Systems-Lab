#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <semaphore.h>

const int SHMID = 1000;

int main()
{
	int segment_id = shmget(IPC_PRIVATE,4096,S_IRUSR | S_IWUSR);
	char* PB;
	char* shared_memory =(char*) shmat(segment_id, NULL, 0);
	if (fork())
	{
		while(1)
		{
			if (shared_memory != NULL)
			{
			printf("From B: %s", shared_memory);
			sprintf(shared_memory,"%s","hello\n");
			}
		}
	}
	else
	{
		while(1)
		{
			if (shared_memory != NULL)
			{
			printf("From A: %s", shared_memory);
			sprintf(shared_memory,"%s","hi\n");
			}
		}
	}
	wait(NULL);
}
