#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <malloc.h>
#include <time.h>
#include <errno.h>

int N_A;
int* A_A;
int S;
volatile int itog;
sem_t my_sema;
int mask;

void* thread_entry(void* param) {
    //printf("printf(\n");
	int* masks_f = (int*)param;
	int mask_l = masks_f[0];
	int sum = A_A[0];
	for (int j = 1; j < N_A; j++) {
		if (mask_l % 2 == 0) {
			sum = sum - A_A[j];
		}
		else {
			sum = sum + A_A[j];
		}
		mask_l = mask_l / 2;
	}
	if (sum == S) {
		sem_wait(&my_sema);
        itog++;
        sem_post(&my_sema);
	}
	for (int i = masks_f[0] + 1; i < masks_f[1]; i++) {
		mask_l = i - 1;
		int mask_r = i;
		int j = 1;
		while (mask_l % 2 != mask_r % 2) {
			if (mask_r % 2 == 0) {
				sum = sum - (A_A[j] + A_A[j]); //меняем знак с + на -
			}  
			else {
				sum = sum + (A_A[j] + A_A[j]);  //меняем знак с - на +
			}
			mask_r = mask_r / 2;
			mask_l = mask_l / 2;
			j++;
		}
		if (sum == S) {
            sem_wait(&my_sema);
			itog++;
			sem_post(&my_sema);
		}
	}
	return 0;
}



int main() {
    int N_threads = 0;
    pthread_t* tids;
    int i = 0;
	FILE* in = fopen("input.txt", "r");
	fscanf(in, "%d", &N_threads);
	fscanf(in, "%d", &N_A);
	A_A = (int*)calloc(N_A, sizeof(int));
	for (i; i < N_A; i++) {
		fscanf(in, "%d", &A_A[i]);
	}
	fscanf(in, "%d", &S);
	fclose(in);
	
	tids = (pthread_t*)calloc(N_threads, sizeof(pthread_t));
	mask = 1;
	for (i = 0; i < N_A - 1; i++) {
		mask *= 2;
	}

	itog = 0;
    sem_init(&my_sema, 0, 1);
    //printf("create потоков\n");
    // Создание потоков и диапозонов масок для них
    for (i = 0; i < N_threads; i++) {
		int* masks = (int*)calloc(2, sizeof(int));
		masks[0] = (mask / N_threads) * i;
		if (i == N_threads - 1) {
			masks[1] = mask;
		}
		else {
            masks[1] = (mask / N_threads) * (i + 1);
        }
        //printf("pthread_create \n");
        printf("%d %d %d %d\n",N_threads, N_A, masks[0],masks[1]);
		if ( 0 != pthread_create(&tids[i], 0, thread_entry, (void*) masks) ) {
                printf("pthread_create failed. errno: %d\n", errno);
                return -1;
            }
        //printf("pthread_create ok. id: %d\n", i);
	}
	//printf("created потоков\n");
	clock_t t = clock();
    for(i = 0; i < N_threads; i++)
        pthread_join(tids[i], 0);

	t = clock() - t;

    sem_destroy(&my_sema);

	FILE* out = fopen("output.txt", "w");
    fprintf(out, "%d\n%d\n%d", N_threads, N_A, itog);
	fclose(out);

	FILE* tim = fopen("time.txt", "w");
	fprintf(tim, "%d", t * 1000 / CLOCKS_PER_SEC);
	fclose(tim);
	free(A_A);

	return 0;
}