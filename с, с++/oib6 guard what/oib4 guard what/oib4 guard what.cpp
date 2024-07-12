#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define constanta 100

int check(char* input, char* password) {
	if (strcmp(password, input))
		return 1;
	else
		return 0;
}

void f_xor(char* password) {
	for (int i = 0; i < 64 && password[i] != '\0'; i++) {
		password[i] ^= constanta;
	}
}

int main1() {
	FILE* password_file = fopen("password.txt", "rb");
	char password[64], input[64];

	fgets(password, 64, password_file);
	f_xor(password);
	fclose(password_file);

	printf("Please, enter password: ");
	scanf("%s", input);
	f_xor(input);

	if (check(input, password) != 0)
		printf("Password is incorrect\n");
	else
		printf("Password is correct\n");
	return 0;
}
