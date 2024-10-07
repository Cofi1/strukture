#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#define MAX 1024

int BrojRedova(const char*);

int main() {
	int brojredova = 0;
	brojredova = BrojRedova("filename.txt");
	return 0;
}

int BrojRedova(const char*) {
	FILE* fp = NULL;
	int brojredova = 0;
	char buffer[MAX] = {};
	fp = fopen(filename, "r");
	if (!fp) {
		return -1;
	}
	while (!feof(fp)) {
		fgets(buffer, MAX, fp);
		brojredova++;
	}
	fclose(fp);
	return brojredova;
}