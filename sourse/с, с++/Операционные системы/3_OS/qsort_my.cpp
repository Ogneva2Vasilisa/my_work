#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <malloc.h>
#include <time.h>

int N_A;
int* A_A;

void qs(int* array, int i_low, int i_hight){
    
    //printf("!!!!!!!!!!!!!!!NEW QS!!!!!!!!!!!!!!!!!\n");
    // for (int k=i_low; k <= i_hight; k++) {
    //         printf("%d ",A_A[k]);
    //     }
    //     printf("\n");
    int i=i_low;
    int buf=0;
    int j=i_hight;
    int i_med=(i_low+i_hight)/2;
    //printf("!!!!!!!i=%d j=%d i_med=%d A[i_med]=%d!!!!!\n",i,j,i_med,array[i_med]);
    while(i<j-1){// bkb <
        while (array[i]<=array[i_med] && i<i_med){
            i++;
        }
        while (array[j]>array[i_med] && j>i_med){
            j--;
        }
        //swap
        if (i_low>=42000){
        printf("i=%d j=%d i_med=%d A[i_med]=%d\n",i,j,i_med,array[i_med]);
        printf("array[i]=%d array[j]=%d \n",array[i], array[j] );
        }
        // printf("i=%d j=%d i_med=%d A[i_med]=%d\n",i,j,i_med,array[i_med]);
        // printf("array[i]=%d array[j]=%d \n",array[i], array[j] );
        buf=array[i];
        array[i]=array[j];
        array[j]=buf;
        if (j==i_med)i_med=i;
        else {if (i==i_med)i_med=j;}

        // printf("\n");
        if (i_low>=42000)
        printf("i=%d j=%d i_med=%d A[i_med]=%d\n",i,j,i_med,array[i_med]);
        if (i>=j-1){
            qs(array,i_low,i_med);
            //printf("                        qs_l finish\n");
            qs(array,i_med+1,i_hight);
            return;
            //printf("                         qs_r finish\n");
        }

    }
    //return 0;
}


void* thread_entry(void* param) {
    //printf("printf(\n");
	int* masks_f = (int*)param;
	//int l = masks_f[0];
    qs(A_A,masks_f[0],masks_f[1]);
	printf("%d %d -----------\n",masks_f[0],masks_f[1]);
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
	
	tids = (pthread_t*)calloc(N_threads, sizeof(pthread_t));

    //printf("create потоков\n");
    if (N_A>=1000){
        // Создание потоков и диапозонов масок для них
        for (i = 0; i < N_threads; i++) {
            int* masks = (int*)calloc(2, sizeof(int));
            masks[0] = (N_A / N_threads) * i;
            masks[1] = masks[0]+(int) (N_A / N_threads)-1;
            
            //printf("pthread_create \n");
            printf("%d %d %d %d\n",N_threads, N_A, masks[0],masks[1]);
            if ( 0 != pthread_create(&tids[i], 0, thread_entry, (void*) masks) ) {
                    printf("pthread_create failed. errno: %d\n", errno);
                    return -1;
                }
            //printf("pthread_create ok. id: %d\n", i);
            //printf("\n");
            
        }
        t = clock();

        for(i = 0; i < N_threads; i++)
            pthread_join(tids[i], 0);
            printf("%d i\n",i);
        }
    else {
        t=clock();
    }

    // printf("\n");
    // for (i=0; i < N_A; i++) {
	// 	printf("%d ",A_A[i]);
	// }
    // printf("\n");

    qs(A_A,0,N_A-1);

    printf("\n");
    printf("\n");
    for (i=0; i < N_A; i++) {
		printf("%d ",A_A[i]);
	}
    printf("\n");
    

	t = clock() - t;


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