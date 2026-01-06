#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <Windows.h>
#include <string>// для std::getline
#include <vector>
#include <zip.h>
#include <time.h>
#include <filesystem>


using namespace std;

int check_zip();
int readfile();
void removeFile(const char* zipPath);
int addFileToZip(const char* filePath, const char* zipPath);
int addDirectoryToZip(const char* directoryPath, const char* zipPath);
int add_my_dir(string target);
int check_dir();
int check_zip();
int main_test_console();