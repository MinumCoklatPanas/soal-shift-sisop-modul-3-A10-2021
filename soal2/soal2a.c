#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
 
#define R 4
#define RC 3
#define C 6

int (*nilai)[10];
int res = 0;
pthread_t thread1, thread2, thread3;
int matriks1[R][RC] = { {1,2,3}, {2,3,4}, {3,2,1}, {2,1,3}};
int matriks2[RC][C] = { {1,2,1,2,1,2}, {2,1,2,1,2,1}, {3,2,3,2,3,2}};

void *showmatriks1(void *arg)
{
    int i,j,k;
    printf("Matriks 1 %dx%d:\n", R, RC);
	for(i=0; i < R; i++){
		for(j=0; j < RC; j++)
			printf("%d ",matriks1[i][j] );
		printf("\n");
	}
	return NULL;
}

void *showmatriks2(void *arg)
{
    int i,j,k;
    printf("\nMatriks 2 %dx%d:\n", RC, C);
	for(i=0; i < RC; i++){
		for(j=0; j < C; j++)
			printf("%d ",matriks2[i][j] );
		printf("\n");
	}
	return NULL;
}

void *perkalian(void *arg)
{
	for(int i=0;i<R;i++)
	{
		for(int j=0;j<C;j++)
		{
			for(int k=0;k<RC;k++)
			{
				res += matriks1[i][k] * matriks2[k][j];
			}
            nilai[i][j] = res; 
            res =0;
		}
	}
	return NULL;
}

int main()
{
    //Shared Memory
	key_t key = 1234;
    int shmid = shmget(key, sizeof(int[10][10]), IPC_CREAT | 0666);
    nilai = shmat(shmid, 0, 0);

	pthread_create(&thread1, NULL, showmatriks1, NULL);
	pthread_join(thread1,NULL);

	pthread_create(&thread2, NULL, showmatriks2, NULL);
	pthread_join(thread2,NULL);

    printf("\n");

	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			nilai[i][j] = 0;
		}
		pthread_create(&thread3, NULL, perkalian, NULL);
		pthread_join(thread3,NULL);
	}

	printf("Hasil Perkalian antara matriks A dan B adalah: \n");
	for(int i=0; i<R; i++){
		for(int j=0;j<C;j++){
			printf("%d ", nilai[i][j]);
		}
		printf("\n");
	}
}
