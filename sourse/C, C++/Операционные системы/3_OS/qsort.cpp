#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <malloc.h>
#include <time.h>

int N_A;
int* A_A;

void merge(int* a, int low, int mid, int high) {
	int n1 = mid - low + 1;
	int n2 = high - mid;
	int* left = (int*)calloc(n1, sizeof(int));
	int* right = (int*)calloc(n2, sizeof(int));
	int i=0;
    int j=0;
    for (i = 0; i < n1; i++)
		left[i] = a[i + low];


	for (i = 0; i < n2; i++)
		right[i] = a[i + mid + 1];

	int k = low;
    i=0;
    j=0;

	while (i < n1 && j < n2)
	{
		if (left[i] <= right[j])
			a[k++] = left[i++];
		else
			a[k++] = right[j++];
	}

	while (i < n1)
		a[k++] = left[i++];
	while (j < n2)
		a[k++] = right[j++];

	free(left);
	free(right);
}


void qs(int* numbers, int left, int right) {

    int pivot; // разрешающий элемент
    int index; // индекс разрешающего элемента
    int l_hold = left; //левая граница
    int r_hold = right; // правая граница
    pivot = numbers[left];
    while (left < right) // пока границы не сомкнутся
    {
        while ((numbers[right] > pivot) && (left < right))
            right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[left] = numbers[right]; // перемещаем элемент [right] на место разрешающего
            left++; // сдвигаем левую границу вправо
        }
        while ((numbers[left] < pivot) && (left < right))
            left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
        if (left != right) // если границы не сомкнулись
        {
            numbers[right] = numbers[left]; // перемещаем элемент [left] на место [right]
            right--; // сдвигаем правую границу влево
        }
    }
    numbers[left] = pivot; // ставим разрешающий элемент на место
    index = left; 
    left = l_hold;
    right = r_hold;
    if (left < index) // Рекурсивно вызываем сортировку для левой и правой части массива
        qs(numbers, left, index - 1);
    if (right > index)
        qs(numbers, index + 1, right);

}


void* thread_entry(void* param) {
	int* masks_f = (int*)param;
    qs(A_A,masks_f[0],masks_f[1]);
	//printf("%d %d -----------\n",masks_f[0],masks_f[1]);
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
            //printf("%d %d %d %d\n",N_threads, N_A, masks[0],masks[1]);
            if ( 0 != pthread_create(&tids[i], 0, thread_entry, (void*) masks) ) {
                    //printf("pthread_create failed. errno: %d\n", errno);
                    return -1;
                }
            
        }
        t = clock();

        for(i = 0; i < N_threads; i++)
            pthread_join(tids[i], 0);
            //printf("%d i\n",i);
        }
    else {
        t=clock();
    }

    for (int i = 1; i < N_threads; i++) {
      int med = (N_A / N_threads) * i - 1;
      int high = 0;
      if (i == N_threads - 1) high = N_A - 1;
      else high = (N_A / N_threads) * (i + 1) - 1;
		  merge((int *)A_A, 0, med, high);
	  }

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