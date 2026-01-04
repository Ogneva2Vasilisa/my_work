/* Copyright © 2014, Owen Shepherd
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted without restriction.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include "elfload.h"

FILE *f;
void *buf;

// структура объявляется
typedef struct {
	int (*puts_t)(const char *s);
	FILE* (*fopen_t)(const char * fname, const char * modeopen);
	int (*fclose_t)(int* filestream);
 	void* (*malloc_t)(size_t s);
	void (*free_t)(void* ptrmem);
	int (*printf_t)(const char* __fmt, va_list args);
 	void* (*memcpy_t)(void* dest, const void* src, size_t size);
 	void* (*memmove_t)(void* dest, const void* src, size_t size);
 	int (*strncmp_t)(const char* dest, const char* src, size_t size);
 	int (*strcmp_t)(const char* dest, const char* src);
 	void (*longjmp_t)(struct __jmp_buf_tag __env[1], int status);
 	void* (*memset_t)(void* dest, int ch, size_t size);
 	char* (*strstr_t)(const char* src, const char* dst);
 	int (*memcmp_t)(const void* lhs, const void* rhs, size_t count);
 	long (*strtol_t)(const char* str, char** str_end, int base);
 	void (*qsort_t)(void* ptr, size_t count, size_t size, int(*__comp)(const void*, const void*));
 	size_t (*strlen_t)(const char* str);
 	char* (*strrchr_t)(const char* str, int ch);
 	char* (*strncpy_t)(char* dest, const char* src, size_t count);
 	char* (*strcat_t)(char* dest, const char* src);
 	char* (*getenv_t)(const char* name);
 	char* (*strcpy_t)(char* dest, const char* src);
 	void* (*memchr_t)(const void* ptr, int ch, size_t count);
 	int (*_setjmp_t)(struct __jmp_buf_tag[1]);
 	void* (*realloc_t)(void* ptr, size_t new_size);
 	int (*munmap_t)(void* start, size_t length);
 	int (*open_t)(const char* pathname, int flags);
 	int (*fstat_t)(int fd, struct stat* buff);
 	void* (*mmap_t)(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
 	ssize_t (*read_t)(int fd, void* buff, size_t count);
 	int* (*__errno_location_t)(void);
 	int (*fcntl_t)(int fd, int cmd, ...);
 	int (*close_t)(int);
 	double (*frexp_t)(double arg, int* exp);
 	double (*modf_t)(double arg, double* iptr);
 	int (*fprintf_t)(FILE* stream, const char* fmt, va_list args);
 	int (*fputc_t)(int ch, FILE* stream);
 	void (*abort_t)(void);
 	char* (*strerror_t)(int errnum);
 	size_t (*fread_t)(void* buffer, size_t size, size_t count, FILE* stream);
 	struct tm* (*gmtime_t)(const time_t* timer);
 	int (*fflush_t)(FILE* stream);
 	int (*ferror_t)(FILE* stream);
 	int (*remove_t)(const char* filename);
 	double (*atof_t)(const char* str);
 	size_t (*fwrite_t)(const void* buff, size_t size, size_t count, FILE* stream);
 	double (*pow_t)(double base, double exp);
 	double (*floor_t)(double arg);
 	time_t (*time_type)(time_t* t);
 	int (*sprintf_t)(char* buffer, const char* fmt, ...);
	int (*snprintf_t)(char* buffer,size_t count, const char* fmt, ...);
 	
	// параметры из cmd
	char* namefile;
 	char* mystring;
} params;

typedef void (*entrypoint_t)(params* p);

static bool fpread(el_ctx *ctx, void *dest, size_t nb, size_t offset)
{
    (void) ctx;

    if (fseek(f, offset, SEEK_SET))
        return false;

    if (fread(dest, nb, 1, f) != 1)
        return false;

    return true;
}

static void *alloccb(
    el_ctx *ctx,
    Elf_Addr phys,
    Elf_Addr virt,
    Elf_Addr size)
{
    (void) ctx;
    (void) phys;
    (void) size;
    return (void*) virt;
}

static void check(el_status stat, const char* expln)
{
    if (stat) {
        fprintf(stderr, "%s: error %d\n", expln, stat);
        exit(1);
    }
}

static void go(entrypoint_t ep, char* namefile, char* mystring)
{
    params p;
	//структура заполняется
    p.puts_t = puts;
    p.fopen_t = fopen;
    p.fclose_t = fclose;
    p.malloc_t = malloc;
    p.free_t = free;
    p.printf_t = vprintf;
    p.memcpy_t = memcpy;
    p.memmove_t = memmove;
    p.strncmp_t = strncmp;
    p.strcmp_t = strcmp;
    p.longjmp_t = longjmp;
    p.memset_t = memset;
    p.strstr_t = strstr;
    p.memcmp_t = memcmp;
    p.strtol_t = strtol;
    p.qsort_t = qsort;
    p.strlen_t = strlen;
    p.strrchr_t = strrchr;
    p.strncpy_t = strncpy;
    p.strcat_t = strcat;
    p.getenv_t = getenv;
    p.strcpy_t = strcpy;
    p.memchr_t = memchr;
    p._setjmp_t = _setjmp;
    p.realloc_t = realloc;
    p.munmap_t = munmap;
    p.open_t = open;
    p.fstat_t = fstat;
    p.mmap_t = mmap;
    p.read_t = read;
    p.__errno_location_t = __errno_location;
    p.fcntl_t = fcntl;
    p.close_t = close;
    p.frexp_t = frexp;
    p.modf_t = modf;
    p.fprintf_t = vfprintf;
    p.fputc_t = fputc;
    p.abort_t = abort;
    p.strerror_t = strerror;
    p.fread_t = fread;
    p.gmtime_t = gmtime;
    p.fflush_t = fflush;
    p.ferror_t = ferror;
    p.remove_t = remove;
    p.atof_t = atof;
    p.fwrite_t = fwrite;
    p.pow_t = pow;
    p.floor_t = floor;
    p.sprintf_t = sprintf;
    p.snprintf_t = snprintf;
	p.time_type = time;
    p.namefile = namefile;
    p.mystring = mystring;
    //передается в entry point эта структура
    ep(&p);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s [elf-to-load]\n", argv[0]);
        return 1;
    }

    f = fopen(argv[1], "rb");
    if (!f) {
        perror("opening file");
        return 1;
    }

    el_ctx ctx;
    ctx.pread = fpread;

    check(el_init(&ctx), "initialising");

    if (posix_memalign(&buf, ctx.align, ctx.memsz)) {
        perror("memalign");
        return 1;
    }

    if (mprotect(buf, ctx.memsz, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        perror("mprotect");
        return 1;
    }

    ctx.base_load_vaddr = ctx.base_load_paddr = (uintptr_t) buf;

    check(el_load(&ctx, alloccb), "loading");
    check(el_relocate(&ctx), "relocating");

    uintptr_t epaddr = ctx.ehdr.e_entry + (uintptr_t) buf;

    entrypoint_t ep = (entrypoint_t) epaddr;

    printf("Binary entrypoint is %" PRIxPTR "; invoking %p\n", (uintptr_t) ctx.ehdr.e_entry, ep);

    go(ep, argv[2], argv[3]);

    fclose(f);

    free(buf);

    return 0;
}
