#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <errno.h>
#include <queue>
#include <malloc.h>
#include <time.h>

int N_A;
int* A_A;
int shag = 0;
HANDLE mutex;
HANDLE event;
HANDLE mutex1;
HANDLE event1;

std::queue<int*> tasks;
std::queue<int*> tasks_merge;


void merge(int low, int mid, int hight) {
    int n1 = mid - low + 1;
    int n2 = hight - mid;
    int* left = (int*)calloc(n1, sizeof(int));
    int* right = (int*)calloc(n2, sizeof(int));
    int i = 0;
    int j = 0;
    for (i = 0; i < n1; i++)
        left[i] = A_A[i + low];
    for (i = 0; i < n2; i++)
        right[i] = A_A[i + mid + 1];
    int k = low;
    i = 0;
    j = 0;
    while (i < n1 && j < n2)
    {
        if (left[i] <= right[j])
            A_A[k++] = left[i++];
        else
            A_A[k++] = right[j++];
    }
    while (i < n1)
        A_A[k++] = left[i++];
    while (j < n2)
        A_A[k++] = right[j++];
    /*WaitForSingleObject(mutex1, INFINITE);
    PulseEvent(event1);
    ReleaseMutex(mutex1);*/
}
void one_iter_merge(int* start1, int len1, int* start2, int len2)
{
    int* temp1 = new int[len1];
    int* temp2 = new int[len2];
    for (int i = 0; i < len1; i++)
    {
        temp1[i] = start1[i];
    }
    for (int i = 0; i < len2; i++)
    {
        temp2[i] = start2[i];
    }
    int a = 0;
    int b = 0;
    while (a < len1 || b < len2)
    {
        if ((a < len1) && !(b < len2))
        {
            start1[a + b] = temp1[a];
            a++;
            continue;
        }
        if (!(a < len1) && (b < len2))
        {
            start1[a + b] = temp2[b];
            b++;
            continue;
        }
        if (temp1[a] >= temp2[b])
        {
            start1[a + b] = temp2[b];
            b++;
            continue;
        }
        if ((temp1[a] < temp2[b]))
        {
            start1[a + b] = temp1[a];
            a++;
            continue;
        }
    }
}

void mergeSort(int l, int r)
{
    int mid = l + (r - l) / 2;
    //printf("%d %d- Sort\n", l, r);
    if (r - l > 1000)
    {
        if (l < r) {
            //int mid = l + (r - l) / 2;
            int* range1 = (int*)calloc(2, sizeof(int));
            range1[0] = l;
            range1[1] = mid;
            WaitForSingleObject(mutex, INFINITE);
            tasks.push(range1);
            PulseEvent(event);
            ReleaseMutex(mutex);

            int* range2 = (int*)calloc(2, sizeof(int));
            range2[0] = mid + 1;
            range2[1] = r;
            //printf("uu %d %d\n", mid + 1, r);
            WaitForSingleObject(mutex, INFINITE);
            tasks.push(range2);
            PulseEvent(event);
            ReleaseMutex(mutex);
        }
    }
    else {
        if (l < r) {
            //int mid = (l + r) / 2; // определяем середину последовательности
            // и рекурсивно вызываем функцию сортировки для каждой половины
            mergeSort(l, mid);
            mergeSort(mid + 1, r);
            merge(l, mid, r);
        }
    }

    return;
}


DWORD WINAPI thread_entry(void* param) {

    while (!tasks.empty()) {
        //WaitForSingleObject(mutex, INFINITE);
        while (tasks.empty()) {
            WaitForSingleObject(event, INFINITE);
        }
        WaitForSingleObject(mutex, INFINITE);

        if (tasks.empty()) return 0;
        int masks_f[2] = { tasks.front()[0], tasks.front()[1] };
        tasks.pop();
        //получили задачу
        ReleaseMutex(mutex);
        mergeSort(masks_f[0], masks_f[1]); //!!!!!!!!!! ,будем вызывать, если  masks_f[1] - masks_f[0]<1000
    }
    //printf("kill\n");
    return 0;
}


int main() {
    int N_threads=0, N_threads_res = 0;
    HANDLE* tids;
    clock_t t;
    int i = 0;
    FILE* in = fopen("input.txt", "r");
    fscanf(in, "%d", &N_threads);
    N_threads_res = N_threads;
    fscanf(in, "%d", &N_A);
    A_A = (int*)calloc(N_A, sizeof(int));
    for (i; i < N_A; i++) {
        fscanf(in, "%d", &A_A[i]);
    }
    fclose(in);

    //параметры синхронизации
    event = CreateEvent(NULL, FALSE, TRUE, NULL);
    mutex = CreateMutex(NULL, FALSE, NULL);
    shag = N_A;
    int N_merge_costyl = 1;
    while (shag > 1000) {
        shag = shag / 2;
        N_merge_costyl = N_merge_costyl * 2;
        //printf("shag = %d \n", shag);//50000 -> 3125 (нечетное) -> 1562(.5)-> 781
    }
    N_merge_costyl = N_merge_costyl * shag;
    //int N_merge = (int)(N_A / shag) * shag;
    int param[2] = { 0,  N_merge_costyl - 1 };
    tasks.push(param);
    // Создание потоков
    if (N_A < 1000) {
        N_threads = 1;
    }
    tids = (HANDLE*)calloc(N_threads, sizeof(HANDLE));
    for (i = 0; i < N_threads; i++) {
        tids[i] = CreateThread(0, 0, thread_entry, NULL, 0, NULL);
        if (tids[i] == 0) {
            return -1;
        }
    }
    printf("%d %d %d\n", shag, N_threads, N_A);
    t = clock();
    WaitForMultipleObjects(N_threads, tids, TRUE, INFINITE);
    //итоговый merge всех подмассивов

    while (shag < N_merge_costyl) {
        for (i = 0; i < N_merge_costyl; i = i + shag * 2)
        {
            //printf("%d %d %d %d %d\n", shag, i, i + shag, i + shag, i + shag * 2, N_merge_costyl);
            //merge(i, i + shag-1, i + shag * 2 - 1);
            one_iter_merge(A_A + i, shag, A_A + i + shag, shag);
        }
        //N_merge = N_merge / 2;
        shag = shag * 2;
    }

    ////итоговый merge всех подмассивов и остатка! (всегда длинной<1000)
    mergeSort(N_merge_costyl, N_A-1);
    //printf("% d % d % d % d\n", 0, N_merge - 1, N_A - 1, N_merge);
    merge(0, N_merge_costyl -1, N_A - 1);

    t = clock() - t;
    CloseHandle(mutex);
    CloseHandle(event);

    for (i = 0; i < N_threads; i++)
        CloseHandle(tids[i]);


    FILE* out = fopen("output.txt", "w");
    fprintf(out, "%d\n%d\n", N_threads_res, N_A);
    for (i = 0; i < N_A; i++) {
        fprintf(out, "%d ", A_A[i]);
    }
    fprintf(out, "\n");
    fclose(out);
    free(A_A);
    FILE* tim = fopen("time.txt", "w");
    fprintf(tim, "%d", t * 1000 / CLOCKS_PER_SEC);
    fclose(tim);


    return 0;
}