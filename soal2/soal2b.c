#include <stdio.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <unistd.h>

#define R 4
#define C 6

unsigned long long angka;
int nilaibaru[4][6];
pthread_t thread,thread2;

unsigned long long factorial(unsigned long long a){
    if(a==0 || a==1) return 1;
    else return a*factorial(a-1);
}

void *hitungmatriks(void *arg)
{
	//template shared memory
        key_t key = 1234;
        int (*value)[10];
        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, 0, 0);
        
        printf("Input matriks baru: \n");
    	for(int x=0;x<R;x++){
			for(int y=0;y<C;y++){
				scanf("%d", &nilaibaru[x][y]);
			}
		}

    	printf("Hasil Faktorial Pertambahan Matriks tersebut adalah: \n");
        for(int i=0;i<R;i++){
		    for(int j=0;j<C;j++){
				if(value[i][j] != 0 && nilaibaru[i][j] != 0){
                	if(value[i][j] >= nilaibaru[i][j]){
                    	angka = (factorial(value[i][j])/factorial(value[i][j] - nilaibaru[i][j]));
                	}
                	else if(value[i][j] < nilaibaru[i][j]){
                    	angka = factorial(value[i][j]);
                	}
                	else if(value[i][j] == 0 && nilaibaru[i][j] == 0){
                    	angka = 0;
                	}
			    	printf("%llu\t", angka); 
				}
				else printf("0\t");
		    }
		    printf("\n");
	    }
    pthread_exit(0);
}

void main()
{
    //Shared Memory
    key_t key = 1234;
    int (*value)[10], nilaibaru[10];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, 0, 0);

	printf("Hasil Perkalian antara matriks A dan B adalah: \n");
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			printf("%d ", value[i][j]);
		}
	printf("\n");
	}

	printf("\n");

	pthread_create(&thread, NULL, hitungmatriks, NULL);
	pthread_join(thread,NULL);
}