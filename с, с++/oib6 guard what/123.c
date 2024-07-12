#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define constanta 100
int check(char* input, char* passm) {
	if (strcmp(passm, input)) { return 1; }
	else { return 0; }}
void f_xor(char* passm) {
	for (int i = 0; i < 64 && passm[i] != '\0'; i++) { passm[i] ^= constanta; } }
int main() {
	FILE* passm_file = fopen("C:/Users/vas/source/repos/oib6 guard what/oib4 guard what/password.txt", "rb");
	char passm[64], input[64];
	fgets(passm, 64, passm_file);
	f_xor(passm);
	fclose(passm_file);
	printf("Please, enter passm: ");
	scanf("%s", input);
	f_xor(input);
	if (check(input, passm) != 0) { printf("passm is incorrect\n"); }
	else { printf("passm is correct\n"); }
	return 0; }