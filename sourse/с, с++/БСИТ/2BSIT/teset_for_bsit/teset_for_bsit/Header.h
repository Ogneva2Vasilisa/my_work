#pragma once

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
namespace fs = std::filesystem;
using namespace fs;
string zip_dir;
string target_dir;
vector <string> masks;

#define MAX_DIRS    10 
#define MAX_FILES   255 
#define MAX_BUFFER  4096 

int check_zip();
int readfile();
void removeFile(const char* zipPath);
int addFileToZip(const char* filePath, const char* zipPath);
int addDirectoryToZip(const char* directoryPath, const char* zipPath);
int add_my_dir(string target);
int check_dir();
int check_zip();