#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <errno.h>
//#include <pthread.h>
//#include <semaphore.h>
#include <malloc.h>
#include <time.h>

int N_A;
int* A_A;
HANDLE mutex;
HANDLE event;

void merge(int* a, int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;
    int* left = (int*)calloc(n1, sizeof(int));
    int* right = (int*)calloc(n2, sizeof(int));

    memcpy(left, a + low, n1);
    memcpy(right, a + mid + 1, n2);

    int k = low;
    int i = 0;
    int j = 0;

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

void mergeSort(int* a, int l, int r)
{
    if (l == r) return; // границы сомкнулись
    int mid = (l + r) / 2; // определяем середину последовательности
    // и рекурсивно вызываем функцию сортировки для каждой половины
    mergeSort(a, l, mid);
    mergeSort(a, mid + 1, r);
    int i = l;  // начало первого пути
    int j = mid + 1; // начало второго пути
    int* tmp = (int*)calloc(r, sizeof(int)); // дополнительный массив
    for (int step = 0; step < r - l + 1; step++) { // для всех элементов дополнительного массива 
        if ((j > r) || ((i <= mid) && (a[i] < a[j]))) {
            tmp[step] = a[i];
            i++;
        }
        else {
            tmp[step] = a[j];
            j++;
        }
    }
    // переписываем сформированную последовательность в исходный массив
    for (int step = 0; step < r - l + 1; step++)
        a[l + step] = tmp[step];
}


DWORD WINAPI thread_entry(void* param) {
    int* masks_f = (int*)param;
    //ReleaseMutex(mutex);
    mergeSort(A_A, masks_f[0], masks_f[1]);
    //SetEvent(event);
    printf("\n");
  /*  for (int i = 0; i < N_A; i++) {
        printf("%d ", A_A[i]);
    }
    printf("\n");*/

    //printf("%d %d -----------\n",masks_f[0],masks_f[1]);
    return 0;
}


int main() {
    int N_threads = 0;
    HANDLE* tids;
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

    tids = (HANDLE*)calloc(N_threads, sizeof(HANDLE));
    event = CreateEvent(NULL, FALSE, TRUE, NULL);
    mutex = CreateMutex(NULL, FALSE, NULL);


    //printf("create потоков\n");
    if (N_A >= 1000) 
    {
        // Создание потоков и диапозонов масок для них
        for (i = 0; i < N_threads; i++) {
            int* masks = (int*)calloc(2, sizeof(int));
            masks[0] = (N_A / N_threads) * i;
            masks[1] = masks[0] + (int)(N_A / N_threads) - 1;

            //printf("pthread_create \n");
            //printf("%d %d %d %d\n",N_threads, N_A, masks[0],masks[1]);
            
            tids[i] = CreateThread(0, 0, thread_entry, masks, 0, NULL);
            if (tids[i] == 0) {
                return -1;
            }

        }
        t = clock();

        WaitForMultipleObjects(N_threads, tids, TRUE, INFINITE);
    }
    else {
        t = clock();
    }

    for (int i = 1; i < N_threads; i++) {
        int med = (N_A / N_threads) * i - 1;
        int high = 0;
        if (i == N_threads - 1) high = N_A - 1;
        else high = (N_A / N_threads) * (i + 1) - 1;
        merge((int*)A_A, 0, med, high);
    }

    t = clock() - t;
    for (i = 0; i < N_threads; i++)
        CloseHandle(tids[i]);



    FILE* out = fopen("output.txt", "w");
    fprintf(out, "%d\n%d\n", N_threads, N_A);
    for (i = 0; i < N_A; i++) {
        fprintf(out, "%d ", A_A[i]);
    }
    fprintf(out, "\n");
    fclose(out);

    FILE* tim = fopen("time.txt", "w");
    fprintf(tim, "%d", t * 1000 / CLOCKS_PER_SEC);
    fclose(tim);
    free(A_A);

    return 0;
}