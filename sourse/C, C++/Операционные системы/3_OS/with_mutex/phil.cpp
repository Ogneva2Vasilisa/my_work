#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

int TOTAL;
int PHIL;

struct synch
{
    HANDLE sema;
    clock_t t;
    char up_down;//1 - ест, 0 - ждет
};


HANDLE waiter_sema; 
CRITICAL_SECTION cs;

clock_t start;

volatile int forks[5] = { 0 };
volatile struct synch* My_synch;

DWORD WINAPI slave_thread(void* param) {
    unsigned int my_min,name;
    //clock_t start_w;
    char flag = 0;
    while (1) {
        my_min = INFINITE;
        name = 5;
        //start_w = clock()-start;
        for (int i = 0; i < 5; i++) {
            if (i == 4) {
                flag = forks[0];
            }
            else {
                flag = forks[i + 1];
            }
            
            if (My_synch[i].t < my_min) {
                if ((flag == 0 && forks[i] == 0) /*&& My_synch[i].up_down == 0*/) {
                    my_min = My_synch[i].t;
                    name = i;
                }
            }
        }

        if (clock() - start >= TOTAL-PHIL) {
            printf("Exit...\n");
            WaitForSingleObject(waiter_sema, INFINITE);
            printf("..\n");
            //WaitForSingleObject(waiter_sema, INFINITE);
            break;
        }
        
        if (name < 5) {
            //printf("%d forks= %d %d,time= %d %d,sema= %d\n", name+1, flag, forks[name], my_min, My_synch[name].t,  My_synch[name].up_down);
            EnterCriticalSection(&cs);
            if (name == 4) forks[0] = 1;
            else forks[name+1] = 1;
            forks[name] = 1;
            LeaveCriticalSection(&cs);
            //My_synch[name].up_down = 1;
            //printf("--- forks =%d %d %d %d %d ---\n", forks[0], forks[1], forks[2], forks[3], forks[4]);
            My_synch[name].t = clock() - start;
            ReleaseSemaphore(My_synch[name].sema, 1, NULL);
            WaitForSingleObject(waiter_sema, INFINITE);
        }
    }
    //sema_up
    return 0;
}

DWORD WINAPI phil_thread(void* param) {
    int* name = (int*)param;
    while (1) {
        Sleep(PHIL); //думает
        //очередь на вилки
        //sema_down
        //My_synch[*name - 1].up_down = 0;
        WaitForSingleObject(My_synch[*name - 1].sema, INFINITE);
        //берет вилки
            printf("%ld:%d:T->E\n", clock() - start, *name);
        Sleep(PHIL ); //кушает
        printf("%ld:%d:E->T\n", clock() - start, *name);
        //отдает вилки
        EnterCriticalSection(&cs);
        if (*name == 5) forks[0] = 0;
        else forks[*name] = 0;
        forks[*name-1] = 0;
        LeaveCriticalSection(&cs);
        //My_synch[*name - 1].t = clock() - start;
        ReleaseSemaphore(waiter_sema, 1, NULL);
    }

    
}

int main(int argc, char** argv) {
    TOTAL = 0;
    PHIL = 0;
    //argc = 3;
    //argv[1] = (char* )"2000";
    //argv[2] = (char* )"20";
    if (argc == 3) {
        TOTAL = strtol(argv[1], 0, 10);
        PHIL = strtol(argv[2], 0, 10);
    }
    else { return -1; }

    printf("%d, %d\n", TOTAL, PHIL);
    start = clock();
    
    HANDLE waiter;
    HANDLE* phils = (HANDLE*)calloc(5, sizeof(HANDLE));
    if (phils == 0) return - 1;
    InitializeCriticalSection(&cs);
    My_synch = (struct synch*)malloc(5*sizeof(struct synch));
    waiter_sema = CreateSemaphore(0, 1, 2, 0);

    for (int i = 0; i < 5; i++) {
        int* name = (int*)calloc(1, sizeof(int));
        *name = i + 1;
        My_synch[i].sema = CreateSemaphore(0, 0, 1, 0);
        My_synch[i].up_down = 0;
        
        //sem_init(&sema[i], 0, 0);
        My_synch[i].t = clock() - start;
        phils[i] = CreateThread(NULL, 0, phil_thread, name, 0, NULL);
        if (phils[i] == 0) {
            return -1;
        }
        //free(name);

    }

    //sem_init(&slave_sema, 0, 2);
    //gettimeofday(&start, NULL);

    waiter = CreateThread(0, 0, slave_thread, NULL, 0, NULL);
    if (waiter == 0) {
        return -1;
    }

    WaitForSingleObject(waiter, INFINITE);
    //WaitForMultipleObjects(5, phils, TRUE, INFINITE);
    
    //pthread_join(slave, 0);

    /*for (int i = 0; i < 5; i++) {
        pthread_join(phils[i], 0);
    }*/
    
    DeleteCriticalSection(&cs);
    for (int i = 0; i < 5; i++) {
        CloseHandle(phils[i]);
        CloseHandle(My_synch[i].sema);
    }
    CloseHandle(waiter);
    //free(My_synch);
    free(phils);
    return 0;
}
