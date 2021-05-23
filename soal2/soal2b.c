#include <stdio.h>
#include <sys/shm.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>

#define R 4
#define C 6

typedef long long ll;
int nilaibaru[4][6],beda,zero;

ll factorial(int n) {
    if (n == 0) return 1;
    return n*factorial(n-1);
}

ll factorial2(int n){
	if (n == beda) return 1;
        return n*factorial2(n-1);
}

void *finalmatriks(void* argv){
	ll n = *(ll*)argv;
	if(zero){
		printf("0 ");
	}
	else if(beda<1){
		printf("%lld ", factorial(n));
	} 
	else printf("%lld ", factorial2(n));


void main()
{
    //Shared Memory
    key_t key = 1234;
    int (*value)[10];
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, 0, 0);

	printf("Hasil Perkalian antara matriks A dan B adalah: \n");
	for(int i=0;i<R;i++){
		for(int j=0;j<C;j++){
			printf("%d ", value[i][j]);
		}
	printf("\n");
	}

	printf("Input matriks baru: \n");
    for(int x=0;x<R;x++){
		for(int y=0;y<C;y++){
			scanf("%d", &nilaibaru[x][y]);
		}
	}

	printf("\n");
	pthread_t thread_id[R*C];
    int cnt=0;
    for(int i = 0; i < R; i++){
        for(int j = 0; j < C; j++){
        	zero=0;
            ll *val = malloc(sizeof(ll[4][6]));

            *val = value[i][j];
            beda = value[i][j] - nilaibaru[i][j];

            if(value[i][j]==0 || nilaibaru[i][j]==0) zero=1;

            pthread_create(&thread_id[cnt], NULL, &finalmatriks, val);
            sleep(1);
            cnt++;
        }
        printf("\n");
    }
    for (int i = 0; i<cnt; i++){
		pthread_join(thread_id[i], NULL);
	}
}
