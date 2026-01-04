# Компилятор и флаги
CC = gcc
FLAGS = -I ./$(PATH_LIBPNG) -L ./$(BUILD_LIBS)/libpng/ -I ./$(PATH_FREETYPE)/include -L ./$(BUILD_LIBS)/freetype
FLAGS_B = -I ./$(PATH_ZLIB)/ -I ./$(PATH_LIBPNG) ./$(BUILD_LIBS)/libpng/libpng.a ./$(BUILD_LIBS)/zlib/libz.a -I ./$(PATH_FREETYPE)/include -L ./$(BUILD_LIBS)/freetype
LDLIBS = -lfreetype -lpng -lz -lm
LDLIBS_B = -lfreetype -lz -nostdlib

# Директории
SRC_DIR = src
BUILD_DIR = build
BUILD_LIBS = $(BUILD_DIR)/libs
PATH_ZLIB = libs/zlib
PATH_LIBPNG = libs/libpng
PATH_FREETYPE = libs/freetype
OBJ_DIR = $(BUILD_DIR)/obj

# Цели для разных версий
TARGET=BIN_DIR

# Исходные файлы и объекты
SRCS = $(wildcard $(SRC_DIR)/*.c)

TARGET_STATIC = $(BUILD_DIR)/app-static
TARGET_DYNAMIC = $(BUILD_DIR)/app-dynamic
TARGET_BLOB = $(BUILD_DIR)/app-blob

# Основная цель
all: static dynamic blob

# Статическая версия (полностью статически линкованная)
static: zlib.a libpng.a freetype.a
# Сборка исполняемого файла
	$(CC) -o $(TARGET_STATIC) src/main.c -static $(FLAGS) $(LDLIBS)

dynamic_neuavn: zlib.so libpng.so freetype.so
	$(CC) -o $(TARGET_DYNAMIC) src/main.c $(FLAGS) $(LDLIBS)

dynamic: zlib.so libpng.so freetype.so
	$(CC) -o $(TARGET_DYNAMIC) src/main.c $(FLAGS) $(LDLIBS) -DIMPLICIT

blob: blob.a blob.exp

blob.a: 
	make -C blob

blob.exp:
	$(CC) -o $(TARGET_BLOB) src/main.c $(FLAGS_B) $(LDLIBS_B) -nostdlib -pie -fPIE -DBLOB


 
# Создание директории
$(BIN_DIR): 
	mkdir -p $@

# Сборка библиотек
zlib.so: zlib.a

libpng.so: libpng.a 

zlib.a: 
	cmake $(PATH_ZLIB) -B $(BUILD_LIBS)/zlib
	make -C $(BUILD_LIBS)/zlib

libpng.a: 
	cmake $(PATH_LIBPNG) -B $(BUILD_LIBS)/libpng
	make -C $(BUILD_LIBS)/libpng

freetype.a:
	cmake $(PATH_FREETYPE) -B $(BUILD_LIBS)/freetype 
	make -C $(BUILD_LIBS)/freetype

freetype.so:
	cmake $(PATH_FREETYPE) -B $(BUILD_LIBS)/freetype/dynamic -D BUILD_SHARED_LIBS=true -D CMAKE_BUILD_TYPE=Release
	make -C $(BUILD_LIBS)/freetype/dynamic

# Очистка
clean:
	rm -rf $(BUILD_DIR)

# Пересборка
rebuild: clean all

.PHONY: all clean rebuild