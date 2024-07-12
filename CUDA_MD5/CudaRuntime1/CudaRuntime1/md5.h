#ifndef MD5_H
#define MD5_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
    uint64_t size;        // Size of input in bytes
    uint32_t buffer[4];   // Current accumulation of hash
    uint8_t input[64];    // Input to be used in the next step
    uint8_t digest[16];   // Result of algorithm
}MD5Context;

__device__ void md5Init(MD5Context *ctx);
__device__ void md5Update(MD5Context *ctx, uint8_t *input, size_t input_len);
__device__ void md5Finalize(MD5Context *ctx);
__device__ void md5Step(uint32_t *buffer, uint32_t *input);

__device__ void md5String(char *input, uint8_t *result);
__device__ void md5File(FILE *file, uint8_t *result);

#endif
