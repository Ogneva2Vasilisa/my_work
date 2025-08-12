#ifdef BLOB
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif


#ifdef IMPLICIT
#include <dlfcn.h>
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H


#define LOAD_SYMBOL(handle, name) \
    *(void**)(&name) = dlsym(handle, #name); \
    if (!name) { \
        fprintf(stderr, "Error loading symbol: %s\n", #name); \
        exit(1); \
    }

// Глобальные переменные для хранения указателей на функции
void* libpng_lib = NULL;
void* freetype_lib = NULL;


png_structp (*png_create_write_structMY)(png_const_charp, png_voidp, png_error_ptr, png_error_ptr) = NULL;
png_infop (*png_create_info_structMY)(png_const_structrp) = NULL;
void (*png_destroy_write_structMY)(png_structp, png_infop) = NULL;
void (*png_init_ioMY)(png_structp, FILE*) = NULL;
void (*png_set_IHDRMY)(png_const_structrp, png_inforp, png_uint_32, png_uint_32, int, int, int, int, int) = NULL;
void (*png_write_infoMY)(png_structrp, png_const_inforp) = NULL;
void (*png_write_imageMY)(png_structrp, png_bytepp) = NULL;
void (*png_write_endMY)(png_structrp, png_inforp) = NULL;
void (*png_set_user_limitsMY)(png_structrp, png_uint_32, png_uint_32) = NULL;

// Указатели на функции freetype
FT_Error (*FT_Init_FreeTypeMY)(FT_Library*) = NULL;
FT_Error (*FT_New_FaceMY)(FT_Library, const char*, FT_Long, FT_Face*) = NULL;
FT_Error (*FT_Set_Pixel_SizesMY)(FT_Face, FT_UInt, FT_UInt) = NULL;
void (*FT_Done_FaceMY)(FT_Face) = NULL;
void (*FT_Done_FreeTypeMY)(FT_Library) = NULL;
FT_Error (*FT_Load_CharMY)(FT_Face, FT_ULong, FT_Int32) = NULL;

// Загрузка библиотек и функций
void load_libraries() {
    // Загружаем libpng
    libpng_lib = dlopen("./libs/libpng/libpng.so", RTLD_LAZY);
    if (!libpng_lib) {
        fprintf(stderr, "Error loading libpng: %s\n", dlerror());
        exit(1);
    }

    // Загружаем функции libpng
    png_create_write_structMY = dlsym(libpng_lib, "png_create_write_struct");
    png_create_info_structMY = dlsym(libpng_lib, "png_create_info_struct");
    png_destroy_write_structMY = dlsym(libpng_lib, "png_destroy_write_struct");
    png_init_ioMY = dlsym(libpng_lib, "png_init_io");
    png_set_IHDRMY = dlsym(libpng_lib, "png_set_IHDR");
    png_write_infoMY = dlsym(libpng_lib, "png_write_info");
    png_write_imageMY = dlsym(libpng_lib, "png_write_image");
    png_write_endMY = dlsym(libpng_lib, "png_write_end");

    // Загружаем freetype
    freetype_lib = dlopen("./libs/freetype/dynamic/libfreetype.so", RTLD_LAZY);
    if (!freetype_lib) {
        fprintf(stderr, "Error loading freetype: %s\n", dlerror());
        dlclose(libpng_lib);
        exit(1);
    }

    // Загружаем функции freetype
    FT_Init_FreeTypeMY = dlsym(freetype_lib, "FT_Init_FreeType");
    FT_New_FaceMY = dlsym(freetype_lib, "FT_New_Face");
    FT_Set_Pixel_SizesMY = dlsym(freetype_lib, "FT_Set_Pixel_Sizes");
    FT_Done_FaceMY = dlsym(freetype_lib, "FT_Done_Face");
    FT_Done_FreeTypeMY = dlsym(freetype_lib, "FT_Done_FreeType");
    FT_Load_CharMY = dlsym(freetype_lib, "FT_Load_Char");
}

// Освобождение библиотек
void unload_libraries() {
    if (libpng_lib) dlclose(libpng_lib);
    if (freetype_lib) dlclose(freetype_lib);
}

#else
#include <string.h>
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#define FT_Load_CharMY FT_Load_Char
#define FT_Init_FreeTypeMY FT_Init_FreeType
#define FT_New_FaceMY FT_New_Face
#define FT_Set_Pixel_SizesMY FT_Set_Pixel_Sizes
#define FT_Done_FaceMY FT_Done_Face 
#define FT_Done_FreeTypeMY FT_Done_FreeType

#define png_create_write_structMY png_create_write_struct
#define png_create_info_structMY png_create_info_struct
#define png_destroy_write_structMY png_destroy_write_struct
#define png_init_ioMY png_init_io
#define png_set_IHDRMY png_set_IHDR
#define png_write_infoMY png_write_info
#define png_write_imageMY png_write_image
#define png_write_endMY png_write_end
#define png_set_user_limitsMY png_set_user_limits

#endif



#ifdef BLOB
#include <sys/stat.h>
#include <sys/types.h>
//#include <fcntl.h>
FILE* stderr;

typedef int (*puts_t)(const char *s);
typedef FILE* (*fopen_t)(const char * fname, const char * modeopen);
typedef int (*fclose_t)(int* filestream);
typedef void* (*malloc_t)(size_t s);
typedef void (*free_t)(void* ptrmem);
typedef int (*printf_t)(const char* __fmt, va_list args);
typedef void* (*memcpy_t)(void* dest, const void* src, size_t size);
typedef void* (*memmove_t)(void* dest, const void* src, size_t size);
typedef int (*strncmp_t)(const char* dest, const char* src, size_t size);
typedef int (*strcmp_t)(const char* dest, const char* src);
typedef void (*longjmp_t)(struct __jmp_buf_tag __env[1], int status);
typedef void* (*memset_t)(void* dest, int ch, size_t size);
typedef char* (*strstr_t)(const char* src, const char* dst);
typedef int (*memcmp_t)(const void* lhs, const void* rhs, size_t count);
typedef long (*strtol_t)(const char* str, char** str_end, int base);
typedef void (*qsort_t)(void* ptr, size_t count, size_t size, int(*__comp)(const void*, const void*));
typedef size_t (*strlen_t)(const char* str);
typedef char* (*strrchr_t)(const char* str, int ch);
typedef char* (*strncpy_t)(char* dest, const char* src, size_t count);
typedef char* (*strcat_t)(char* dest, const char* src);
typedef char* (*getenv_t)(const char* name);
typedef char* (*strcpy_t)(char* dest, const char* src);
typedef void* (*memchr_t)(const void* ptr, int ch, size_t count);
typedef int (*_setjmp_t)(struct __jmp_buf_tag[1]);
typedef void* (*realloc_t)(void* ptr, size_t new_size);
typedef int (*munmap_t)(void* start, size_t length);
typedef int (*open_t)(const char* pathname, int flags);
typedef int (*fstat_t)(int fd, struct stat* buff);
typedef void* (*mmap_t)(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
typedef ssize_t (*read_t)(int fd, void* buff, size_t count);
typedef int* (*__errno_location_t)(void);
typedef int (*fcntl_t)(int fd, int cmd, ...);
typedef int (*close_t)(int);
typedef double (*frexp_t)(double arg, int* exp);
typedef double (*modf_t)(double arg, double* iptr);
typedef int (*fprintf_t)(FILE* stream, const char* fmt, va_list args);
typedef int (*fputc_t)(int ch, FILE* stream);
typedef void (*abort_t)(void);
typedef char* (*strerror_t)(int errnum);
typedef size_t (*fread_t)(void* buffer, size_t size, size_t count, FILE* stream);
typedef struct tm* (*gmtime_t)(const time_t* timer);
typedef int (*fflush_t)(FILE* stream);
typedef int (*ferror_t)(FILE* stream);
typedef int (*remove_t)(const char* filename);
typedef double (*atof_t)(const char* str);
typedef size_t (*fwrite_t)(const void* buff, size_t size, size_t count, FILE* stream);
typedef double (*pow_t)(double base, double exp);
typedef double (*floor_t)(double arg);
typedef time_t (*time_type)(time_t* t);
typedef int (*sprintf_t)(char* buffer, const char* fmt, ...);
typedef int (*snprintf_t)(char* buffer,size_t count, const char* fmt, ...);

puts_t xputs;
puts_t *pxprintf = &xputs;
fopen_t xfopen;
fopen_t *pxfopen = &xfopen;
fclose_t xfclose;
fclose_t *pxfclose = &xfclose;
malloc_t xmalloc;
malloc_t *pxmalloc = &xmalloc;
free_t *xfree;
free_t *pxfree = &xfree;
printf_t xprintf;
printf_t *pxputs = &xprintf;
memcpy_t xmemcpy;
memcpy_t *pxmemcpy = &xmemcpy;
memmove_t xmemmove;
memmove_t *pxmemmove = &xmemmove;
strncmp_t xstrncmp;
strncmp_t *pxstrncmp = &xstrncmp;
strcmp_t xstrcmp;
strcmp_t *pxstrcmp = &xstrcmp;
longjmp_t xlongjmp;
longjmp_t *pxlongjmp = &xlongjmp;
memset_t xmemset;
memset_t *pxmemset = &xmemset;
strstr_t xstrstr;
strstr_t *pxstrstr = &xstrstr;
memcmp_t xmemcmp;
memcmp_t *pxmemcmp = &xmemcmp;
strtol_t xstrtol;
strtol_t *pxstrtol = &xstrtol;
qsort_t xqsort;
qsort_t *pxqsort = &xqsort;
strlen_t xstrlen;
strlen_t *pxstrlen = &xstrlen;
strrchr_t xstrrchr;
strrchr_t *pxstrrchr = &xstrrchr;
strncpy_t xstrncpy;
strncpy_t *pxstrncpy = &xstrncpy;
strcat_t xstrcat;
strcat_t *pxstrcat = &xstrcat;
getenv_t xgetenv;
getenv_t *pxgetenv = &xgetenv;
strcpy_t xstrcpy;
strcpy_t *pxstrcpy = &xstrcpy;
memchr_t xmemchr;
memchr_t *pxmemchr = &xmemchr;
_setjmp_t xsetjmp;
_setjmp_t *pxsetjmp = &xsetjmp;
realloc_t xrealloc;
realloc_t *pxrealloc = &xrealloc;
munmap_t xmunmap;
munmap_t *pxmunmap = &xmunmap;
open_t xopen;
open_t *pxopen = &xopen;
fstat_t xfstat;
fstat_t *pxfstat = &xfstat;
mmap_t xmmap;
mmap_t *pxmmap = &xmmap;
read_t xread;
read_t *pxread = &xread;
__errno_location_t xerrno_location;
__errno_location_t *pxerrno_location = &xerrno_location;
fcntl_t xfcntl;
fcntl_t *pxfcntl = &xfcntl;
close_t xclose;
close_t *pxclose = &xclose;
frexp_t xfrexp;
frexp_t *pxfrexp = &xfrexp; 
modf_t xmodf;
modf_t *pxmodf = &xmodf;
fprintf_t xfprintf;
fprintf_t *pxfprintf = &xfprintf;
fputc_t xfputc;
fputc_t *pxfputc = &xfputc;
abort_t xabort;
abort_t *pxabort = &xabort;
strerror_t xstrerror;
strerror_t *pxstrerror = &xstrerror;
fread_t xfread;
fread_t *pxfread = &xfread;
gmtime_t xgmtime;
gmtime_t *pxgmtime = &xgmtime;
fflush_t xfflush;
fflush_t *pxfflush = &xfflush;
ferror_t xferror;
ferror_t *pxferror = &xferror;
remove_t xremove;
remove_t *pxremove = &xremove;
atof_t xatof;
atof_t *pxatof = &xatof;
fwrite_t xfwrite;
fwrite_t *pxfwrite = &xfwrite;
pow_t xpow;
pow_t *pxpow = &xpow;
floor_t xfloor;
floor_t *pxfloor = &xfloor;
time_type xtime;
time_type *pxtime = &xtime;
sprintf_t xsprintf;
sprintf_t *pxsprintf = &xsprintf;
snprintf_t xsnprintf;
snprintf_t *pxsnprintf = &xsnprintf;

FILE* fopen(const char * fname, const char * modeopen) {
    return (*pxfopen)(fname, modeopen);
}

int puts(const char *s) {
    return (*pxprintf)(s);
}

int fclose(FILE* filestream) {
    return (*pxfclose)(filestream);
}

void* malloc(size_t s) {
    return (*pxmalloc)(s);
}

void free(void* ptrmem) {
    (*pxfree)(ptrmem);
}

int printf(const char* __fmt, ...) {
    int __res;
    va_list __args;
    va_start(__args, __fmt);
    __res = (*pxputs)(__fmt, __args);
    va_end(__args);
    return __res;
}

void __stack_chk_fail(void) {}

void* memcpy(void* dest, const void* src, size_t size) {
    return (*pxmemcpy)(dest, src, size);
}

void* memmove(void* dest, const void* src, size_t size) {
    return (*pxmemmove)(dest, src, size);
}

int strncmp(const char* dest, const char* src, size_t size) {
    return (*pxstrncmp)(dest, src, size);
}

int strcmp(const char* dest, const char* src) {
    return (*pxstrcmp)(dest, src);
}

void longjmp(struct __jmp_buf_tag __env[1], int status) {
    (*pxlongjmp)(__env, status);
}

void* memset(void* dest, int ch, size_t size) {
    return (*pxmemset)(dest, ch, size);
}

char* strstr(const char* __str, const char* __substr) {
    return (*pxstrstr)(__str, __substr);
}

int memcmp(const void* lhs, const void* rhs, size_t count) {
  return (*pxmemcmp)(lhs, rhs, count);
}

long strtol(const char* str, char** str_end, int base) {
    return (*pxstrtol)(str, str_end, base);
}

void qsort(void* __ptr, size_t __count, size_t __size, int(*__comp)(const void*, const void*)) {
    return (*pxqsort)(__ptr, __count, __size, __comp);
}

size_t strlen(const char* str) {
    return (*pxstrlen)(str);
}

char* strrchr(const char* str, int ch) {
    return (*pxstrrchr)(str, ch);
}

char* strncpy(char* dest, const char* src, size_t count) {
    return (*pxstrncpy)(dest, src, count);
}

char* strcat(char* dest, const char* src) {
    return (*pxstrcat)(dest, src);
}

char* getenv(const char* name) {
    return (*pxgetenv)(name);
}

char* strcpy(char* dest, const char* src) {
    return (*pxstrcpy)(dest, src);
}

void* memchr(const void* ptr, int ch, size_t count) {
    return (*pxmemchr)(ptr, ch, count);
}

int _setjmp(struct __jmp_buf_tag __env[1]) {
    return (*pxsetjmp)(__env);
}

void* realloc(void* ptr, size_t new_size) {
    return (*pxrealloc)(ptr, new_size);
}

int munmap(void* start, size_t length) {
    return (*pxmunmap)(start, length);
}

int open(const char* pathname, int flags) {
    return (*pxopen)(pathname, flags);
}

int fstat(int fd, struct stat* buff) {
    return (*pxfstat)(fd, buff);
}

void*
mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) {
    return (*pxmmap)(addr, length, prot, flags, fd, offset);
}

ssize_t read(int fd, void* buff, size_t count) {
    return (*pxread)(fd, buff, count);
}

int* __errno_location(void) {
    return (*pxerrno_location)();
}

int fcntl(int fd, int cmd, ...) {
    int __res;
    va_list __args;
    va_start(__args, cmd);
    __res = (*pxfcntl)(fd, cmd, *__args);
    va_end(__args);
    return __res;
}

int close(int fd) {
    return (*pxclose)(fd);
}

double frexp(double arg, int* exp) {
    return (*pxfrexp)(arg, exp);
}

double modf(double arg, double* iptr) {
    return (*pxmodf)(arg, iptr);
}

int fprintf(FILE* stream, const char* fmt, ...) {
    int __res;
    va_list __args;
    va_start(__args, fmt);
    __res = (*pxfprintf)(stream, fmt, __args);
    va_end(__args);
    return __res;
}

int fputc(int ch, FILE* stream) {
    return (*pxfputc)(ch, stream);
}

void abort(void) {
    (*pxabort)();
}

char* strerror(int errnum) { 
    return (*pxstrerror)(errnum);
}

size_t fread(void* buffer, size_t size, size_t count, FILE* stream) {
    return (*pxfread)(buffer, size, count, stream);
}

struct tm* gmtime(const time_t* timer) {
    return (*pxgmtime)(timer);
}

int fflush(FILE* stream) {
    return (*pxfflush)(stream);
}

int ferror(FILE* stream) {
    return (*pxferror)(stream);
}

int remove(const char* filename) {
    return (*pxremove)(filename);
}

double atof(const char* str) {
    return (*pxatof)(str);
}

size_t fwrite(const void* buff, size_t size, size_t count, FILE* stream) {
    return (*pxfwrite)(buff, size, count, stream);
}

double pow(double base, double exp) {
    (*pxpow)(base, exp);
}

double floor(double arg) {
    (*pxfloor)(arg);
}

time_t time(time_t* t) {
    return (*pxtime)(t);
}

int sprintf(char* __buffer, const char* __fmt, ...) {
    int __res;
    va_list __args;
    va_start(__args, __fmt);
    __res = (*pxsprintf)(__buffer, __fmt, *__args);
    va_end(__args);
    return __res;
  }
int snprintf(char* __buffer, size_t count, const char* __fmt, ...) {
    int __res;
    va_list __args;
    va_start(__args, __fmt);
    __res = (*pxsnprintf)(__buffer, count, __fmt, *__args);
    va_end(__args);
    return __res;
  }

typedef struct {
	int (*puts_t)(const char *s);
	FILE* (*fopen_t)(const char * fname, const char * modeopen);
	int (*fclose_t)(FILE* filestream);
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


#endif


typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} Pixel;

char create_png_with_text(const char* filename, const char* font_path, const char* text, int font_size) {

    printf("Start \n");

    // FT_Library ft_lib;
    // FT_Init_FreeTypeMY(&ft_lib);

    // printf("Loading..\n");
    // FT_Face face;

    // 
    FT_Library ft_lib = 0;
    FT_Init_FreeType(&ft_lib);

    // Загрузка шрифта arial.ttf из текущей папки
    FT_Face face = 0;

    printf(" . \n");
    FT_New_FaceMY(ft_lib, font_path, 0, &face);
    
    printf("ready\n");
    FT_Set_Pixel_Sizes(face, 10, 0);

    printf("Loading..\n");
    
    // Установка размера шрифта

    // Расчет размеров изображения
    int padding = font_size;
    int width = 0;
    int height = 0;
    
    // Вычисляем общую ширину и максимальную высоту текста
    for (int i = 0; text[i] != '\0'; i++) {
        if (FT_Load_CharMY(face, text[i], FT_LOAD_RENDER)) {
            continue;
        }
        width += face->glyph->bitmap.width;
        if (face->glyph->bitmap.rows > height) {
            height = face->glyph->bitmap.rows;
        }
        printf("\n w=%d h=%d\n", width, height);
    }
    
    printf(" . \n");

    // Проверка на пустой текст
    if (width == 0 || height == 0) {
        width = 100;  // Минимальные размеры если текст пустой
        height = 30;
        
    }
    
    width += 2 * padding;
    width += (int)width/2;
    height += 2 * padding;
    printf("\n w=%d h=%d\n", width, height);

    // Создание изображения
    Pixel* image = (Pixel*)malloc(width * height * sizeof(Pixel));
    if (!image) {
        fprintf(stderr, "Could not allocate memory for image\n");
        FT_Done_Face(face);
        FT_Done_FreeType(ft_lib);
        return 0;
    }
    printf(" . ");

    // Заполнение фона (белый с прозрачностью)
    for (int i = 0; i < width * height; i++) {
        image[i].red = 255;
        image[i].green = 255;
        image[i].blue = 255;
        image[i].alpha = 0;
    }

    printf(" . ");
    // Рендеринг текста (если текст не пустой)
    if (strlen(text) > 0) {
        int x_offset = padding;
        for (int i = 0; text[i] != '\0'; i++) {
            if (FT_Load_CharMY(face, text[i], FT_LOAD_RENDER)) {
                continue;
            }

            FT_Bitmap* bitmap = &face->glyph->bitmap;
            
            for (int row = 0; row < bitmap->rows; row++) {
                for (int col = 0; col < bitmap->width; col++) {
                    int img_x = x_offset + col + face->glyph->bitmap_left;
                    int img_y = padding + row + (height - 2 * padding - bitmap->rows) / 2 + 
                                (face->size->metrics.ascender >> 6);
                    
                    if (img_x >= 0 && img_x < width && img_y >= 0 && img_y < height) {
                        unsigned char val = bitmap->buffer[row * bitmap->pitch + col];
                        int idx = img_y * width + img_x;
                        
                        // Черный текст с альфа-каналом
                        image[idx].red = 0;
                        image[idx].green = 0;
                        image[idx].blue = 0;
                        image[idx].alpha = val;
                    }
                }
            }
            
            x_offset += face->glyph->advance.x >> 6;
        }
    }

    // Сохранение PNG
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Could not open file %s for writing\n", filename);
        free(image);
        FT_Done_FaceMY(face);
        FT_Done_FreeTypeMY(ft_lib);
        return 0;
    }
    printf(" . ");

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fprintf(stderr, "Could not allocate png write struct\n");
        fclose(fp);
        free(image);
        FT_Done_FaceMY(face);
        FT_Done_FreeTypeMY(ft_lib);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fprintf(stderr, "Could not allocate png info struct\n");
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        free(image);
        FT_Done_FaceMY(face);
        FT_Done_FreeTypeMY(ft_lib);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error during png creation\n");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        free(image);
        FT_Done_FaceMY(face);
        FT_Done_FreeTypeMY(ft_lib);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Установка параметров PNG
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Подготовка строк изображения для libpng
    png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)(image + y * width);
    }
    printf(" . \n");
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    // Очистка
    free(row_pointers);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    free(image);
    FT_Done_FaceMY(face);
    FT_Done_FreeTypeMY(ft_lib);

    return 1;
}

void print_usage(const char* program_name) {
    printf("Usage: %s <font_path> <output.png> <text> [font_size]\n", program_name);
    printf("Example: %s /usr/share/fonts/FreeSans.ttf output.png \"Hello World\" 24\n");
}

#ifdef BLOB
int _start(params* p) {
#else
int main(char argc, char* argv[]){
#endif
    #ifdef BLOB
    pxprintf = &p->puts_t;
    (*pxprintf)("asasd\n");
    pxfopen = &p->fopen_t;
    pxfclose = &p->fclose_t;
    pxmalloc = &p->malloc_t;
    pxfree = &p->free_t;
    pxputs = &p->printf_t;
    pxmemcpy = &p->memcpy_t;
    pxmemmove = &p->memmove_t;
    pxstrncmp = &p->strncmp_t;
    pxstrcmp = &p->strcmp_t;
    pxlongjmp = &p->longjmp_t;
    pxmemset = &p->memset_t;
    pxstrstr = &p->strstr_t;
    pxmemcmp = &p->memcmp_t;
    pxstrtol = &p->strtol_t;
    pxqsort = &p->qsort_t;
    pxstrlen = &p->strlen_t;
    pxstrrchr = &p->strrchr_t;
    pxstrncpy = &p->strncpy_t;
    pxstrcat = &p->strcat_t;
    pxgetenv = &p->getenv_t;
    pxstrcpy = &p->strcpy_t;
    pxmemchr = &p->memchr_t;
    pxsetjmp = &p->_setjmp_t;
    pxrealloc = &p->realloc_t;
    pxmunmap = &p->munmap_t;
    pxopen = &p->open_t;
    pxfstat = &p->fstat_t;
    pxmmap = &p->mmap_t;
    pxread = &p->read_t;
    pxerrno_location = &p->__errno_location_t;
    pxfcntl = &p->fcntl_t;
    pxclose = &p->close_t;
    pxfrexp = &p->frexp_t; 
    pxmodf = &p->modf_t;
    pxfprintf = &p->fprintf_t;
    pxfputc = &p->fputc_t;
    pxabort = &p->abort_t;
    pxstrerror = &p->strerror_t;
    pxfread = &p->fread_t;
    pxgmtime = &p->gmtime_t;
    pxfflush = &p->fflush_t;
    pxferror = &p->ferror_t;
    pxremove = &p->remove_t;
    pxatof = &p->atof_t;
    pxfwrite = &p->fwrite_t;
    pxpow = &p->pow_t;
    pxfloor = &p->floor_t;
    pxtime = &p->time_type;
    pxsprintf = &p->sprintf_t;
    const char *font_path = p->namefile;
    const char *text = p->mystring;

    #else
    const char *font_path = argv[1];
    const char *text = argv[2];
    #endif
    
    printf("Hello\n");
    //char* text = "Hello";
    #ifdef IMPLICIT
    load_libraries();
    #endif


    int font_size = 12;  // Значение по умолчанию

    char output_filename [15] ="output.png";
    
    printf("Creating PNG with text...\n");
    printf("Font: %s\n", font_path);
    printf("Output file: %s\n", output_filename);
    printf("Text: %s\n", text);
    printf("Font size: %d\n", font_size);

    if (create_png_with_text(output_filename, font_path, text, font_size)) {
        printf("PNG created successfully!\n");
        return 0;
    } else {
        printf("Failed to create PNG.\n");
        return 1;
    }


    #ifdef IMPLICIT
    unload_libraries();
    #endif
}