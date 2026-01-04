#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <errno.h>
#include <queue>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <malloc.h>
#include <time.h>

int N_A;
int* A_A;
pthread_mutex_t g_queue_mutex;
pthread_cond_t g_cond_data_ready;
std::queue<int*> tasks;

void qs(int left, int right)
{
int i = left, j = right, x = A_A[(left + right) / 2], tmp;
do {
    while (A_A[i] < x) {
        i++;
    }
    while (A_A[j] > x) {
        j--;
    }
    if (i <= j) {
        tmp = A_A[i];
        A_A[i] = A_A[j];
        A_A[j] = tmp;
        i++;
        j--;
    }
} while (i <= j);


    if(j - left >1000) {
    //if(left < index-1) {
        int* range1=(int*)calloc(2,sizeof(int));
        range1[1]=j;
        range1[0]=left;
        //printf("m_l---%d %d \n",range1[0], range1[1]);
        pthread_mutex_lock(&g_queue_mutex);
		tasks.push(range1);
        //printf("c_s---%d %d \n",range1[0], range1[1]);
		pthread_cond_signal(&g_cond_data_ready);
        //printf("m_u---%d %d \n",range1[0], range1[1]);
        pthread_mutex_unlock(&g_queue_mutex);
    }
    else{
        if (j > left) // Рекурсивно вызываем сортировку для левой и правой части массива
        qs(left, j);
    }

    if(right - i>1000) {
    //if(right > index+1) {
        int* range2=(int*)calloc(2,sizeof(int));
        range2[0]=i;
        range2[1]=right;
        //printf("m_l---%d %d \n",range2[0], range2[1]);
        pthread_mutex_lock(&g_queue_mutex);
		tasks.push(range2);
		//printf("c_s---%d %d \n",range2[0], range2[1]);
		pthread_cond_signal(&g_cond_data_ready);
        //printf("m_u---%d %d \n",range2[0], range2[1]);
        pthread_mutex_unlock(&g_queue_mutex);
    }
    else {    
        if (right > i)
            qs(i, right);
    }

}


void* thread_entry(void *param) {
    while (!tasks.empty()) {
        // printf("m_l----------%d \n",pthread_self());
        // for (int i = 0; i < N_A; i++) {
        //         printf("%d ",A_A[i] );
        //     }
        // printf("\n");
        pthread_mutex_lock(&g_queue_mutex);
        if (tasks.empty()) {
            char flag = 1;
            for (int i = 0; i < N_A - 1; i++) {
                if (A_A[i] > A_A[i + 1]) {
                    flag = 0;
                    break;
                }
            }
            //pthread_mutex_unlock(&g_queue_mutex);
            if (flag) {
                //printf("ll-p%d \n",pthread_self());
                //pthread_mutex_lock(&g_queue_mutex);
                pthread_cond_broadcast(&g_cond_data_ready);
                pthread_mutex_unlock(&g_queue_mutex);
                return 0;
            }
            //printf("c_w---------- %d \n",pthread_self());  
            pthread_cond_wait(&g_cond_data_ready, &g_queue_mutex);
            
        }

        int masks_f[2] = { tasks.front()[0], tasks.front()[1] };
        //printf("%d %d -----------\n",masks_f[0],masks_f[1]);
        tasks.pop();
        //получили задачу
        //printf("m_u---------\n");
        pthread_mutex_unlock(&g_queue_mutex);
        qs(masks_f[0],masks_f[1]);
        
        ////printf("m_l---------- \n");
        //pthread_mutex_lock(&g_queue_mutex);
    }
        
    //printf("ll%d \n",pthread_self());
    //pthread_mutex_lock(&g_queue_mutex);
    //pthread_cond_broadcast(&g_cond_data_ready);
    //pthread_mutex_unlock(&g_queue_mutex);
    
    
	return 0;
}


int main() {
    int N_threads = 0;
    pthread_t* tids;
    clock_t t;
    int i = 0;
	FILE* in = fopen("input.txt", "r");
	fscanf(in, "%d", &N_threads);
	fscanf(in, "%d", &N_A);
	A_A = (int*)calloc(N_A, sizeof(int));
	for (i; i < N_A; i++) {
		fscanf(in, "%d", &A_A[i]);
	}
	fclose(in);
	pthread_mutex_init(&g_queue_mutex, 0);
    pthread_cond_init(&g_cond_data_ready, 0);

	tids = (pthread_t*)calloc(N_threads, sizeof(pthread_t));

    ////printf("create потоков\n");
    int param[2] = { 0, N_A - 1 };
	tasks.push(param);

    
    // Создание потоков и диапозонов масок для них
    for (i = 0; i < N_threads; i++) {        
        ////printf("pthread_create \n");
        //printf("%d , N_A=%d\n",N_threads,N_A);
        if ( 0 != pthread_create(&tids[i], 0, thread_entry, NULL) ) {
                ////printf("pthread_create failed. errno: %d\n", errno);
                return -1;
            }
        
    }
    t = clock();

    for(i = 0; i < N_threads; i++)
        pthread_join(tids[i], 0);
   

    // for (int i = 1; i < N_threads; i++) {
    //   int med = (N_A / N_threads) * i - 1;
    //   int high = 0;
    //   if (i == N_threads - 1) high = N_A - 1;
    //   else high = (N_A / N_threads) * (i + 1) - 1;
	// 	  merge((int *)A_A, 0, med, high);
	//   }

	t = clock() - t;
    pthread_mutex_destroy(&g_queue_mutex);
    pthread_cond_destroy(&g_cond_data_ready);


	FILE* out = fopen("output.txt", "w");
    fprintf(out, "%d\n%d\n", N_threads, N_A);
    for (i=0; i < N_A; i++) {
		fprintf(out,"%d ",A_A[i]);
	}
    fprintf(out, "\n");
	fclose(out);

	FILE* tim = fopen("time.txt", "w");
	fprintf(tim, "%d", t * 1000 / CLOCKS_PER_SEC);
	fclose(tim);
	free(A_A);

	return 0;
}