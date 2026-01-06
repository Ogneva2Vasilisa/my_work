//#define _CRT_UNUSED
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <locale.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include "md5.c"
#include <stdio.h>

FILE* slov;
char* stroka;

const unsigned long long n = 100000000;//214194124
__device__ const int len_slov = 20; //-----------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!

clock_t start_time;
clock_t end_time;
clock_t start_time1;
clock_t end_time1;

__device__ __host__ void print_hash(uint8_t* p) {
    for (unsigned int i = 0; i < 16; ++i) {
        printf("%02x", p[i]);
    }
    printf("\n");
}
__device__ __host__ void fprint_hash(FILE* file, uint8_t* p) {
    for (unsigned int i = 0; i < 16; ++i) {
        fprintf(file,"%02x", p[i]);
    }
    fprintf(file,"\n");
}

cudaError_t addWithCuda( char* a, uint8_t* result, unsigned long long size);

__global__ void addKernel(char *a, uint8_t* res)
{
    long long i = threadIdx.x + blockIdx.x * blockDim.x;
    if (i < n)
    {        
        //printf("%d : %d : %d\n", blockIdx.x, threadIdx.x, i);
        md5String (a + i * len_slov, res + i *16);  
        __syncthreads();
    }
}

int main()
{
    const long long arraySize = n;
    printf("kol-vo slov %lld\n", n);
    //char a[arraySize] ;
    setlocale(LC_ALL, "Ru");
    uint8_t* result;
    result = (uint8_t*)malloc(16 * n);
    stroka = (char*)malloc(len_slov*arraySize);
    memset(result, 0, 16 * n);
    long long i = 0;
    //чтение из файла и запись в строку

    {
        FILE* mf;       // Переменная, в которую поочередно будут помещаться считываемые строки
        char str[25];   //Указатель, в который будет помещен адрес массива, в который считана строка, или NULL если достигнут коней файла или произошла ошибка
        char* estr;     // Открытие файла с режимом доступа «только чтение» и привязка к нему потока данных
        printf("Открытие файла : ");
        mf = fopen("itog.txt", "r");

        // Проверка открытия файла
        if (mf == NULL) { printf("ошибка\n"); return -1; }
        else printf("выполнено\n");
        printf("Считаны строки : \n");

        //Чтение (построчно) данных из файла в бесконечном цикле
        while (i != n)
        {
            // Чтение одной строки  из файла
            estr = fgets(str, sizeof(str), mf);
            //Проверка на конец файла или ошибку чтения
            if (estr == NULL)
            {
                // Проверяем, что именно произошло: кончился файл или это ошибка чтения
                if (feof(mf) != 0)
                {
                    //Если файл закончился, выводим сообщение о завершении чтения и выходим из бесконечного цикла
                    printf("\nЧтение файла закончено\n");
                    break;
                }
                else
                {
                    //Если при чтении произошла ошибка, выводим сообщение об ошибке и выходим из бесконечного цикла
                    printf("\nОшибка чтения из файла\n");
                    break;
                }
            }
            //Если файл не закончился, и не было ошибки чтения выводим считанную строку  на экран           
            // i - итератор по кол-ву слов
            int k = 0;//итератор по готовому слову
            for (int j = 0; j < strlen(str) - 1; j++, k++)
            {
                if (str[j] != '\n') {
                    *(stroka + i * len_slov + k) = str[j];
                }
            }
            while (k < len_slov)
            {
                *(stroka + i * len_slov + k) = 0;
                k++;
            }
            i++;
        }
        // Закрываем файл
        printf("Закрытие файла : ");
        if (fclose(mf) == EOF) printf("ошибка\n");
        else printf("выполнено\n");
    }
    
    //==============================================================================================================

    printf("\n");
    start_time = clock();
    // Add vectors in parallel.
    cudaError_t cudaStatus = addWithCuda(stroka, result, arraySize);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addWithCuda failed!");
        return 1;
    }
    end_time = clock();
    printf("\nTime exec itog: %0.f ms\n", (double)(end_time - start_time));
    for (int i = 0; i < len_slov*arraySize; i++)
    {
        //printf("%d %c, ", stroka[i], stroka[i]);
    }

    printf("\n");
    //консоль 10 первых слов
    {
        for (i = 0; i < 10; i++)
        {
            printf("%s ", (stroka + i * len_slov));
            printf(" ");
            print_hash(result + i * 16);
        }
    }
    //запись все в файл
    printf("Запись выходных данных\n");
    FILE* out_file = fopen("output.txt", "w");
    {
        for (int i = 0; i < 10; i++) {//-------------------------------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //printf("%s ", (c + i * 20));
            fprintf(out_file, "%s ", (stroka + i * len_slov));
            fprint_hash(out_file, (result + i * 16));
        }
    }
    fclose(out_file);
    printf("Запись выполнена\n");
    
    printf("\nTime exec itog: %0.f ms\n", (double)(end_time - start_time));

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }
    free(result);
    free(stroka);

    return 0;
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t addWithCuda(char *a, uint8_t* result, unsigned long long size)
{
    char *dev_a = 0;
    uint8_t *dev_res = 0;
    cudaError_t cudaStatus;

    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        goto Error;
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(char) * len_slov);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_res, size * sizeof(uint8_t)*16);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(char) * len_slov, cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
     
    start_time1 = clock();

    const unsigned long long BLOCK_SIZE = 1024;
    //const unsigned long long BLOCK_COUNT = 10;
    // Launch a kernel on the GPU with one thread for each element.
    addKernel<<<(n + BLOCK_SIZE) / BLOCK_SIZE, BLOCK_SIZE >>>(dev_a, dev_res);//первое число
    
    end_time1 = clock();
    printf("\nTime exec: %0.f ms\n", (double)(end_time1 - start_time1));
    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }
    
    // cudaDeviceSynchronize waits for the kernel to finish, and returns any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    cudaStatus = cudaMemcpy(result, dev_res, size * sizeof(uint8_t)*16, cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }
    

Error:
    cudaFree(dev_a);
    cudaFree(dev_res);
    
    return cudaStatus;
}
