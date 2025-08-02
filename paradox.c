//mostly helpers and things for the whole project

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char* strup(char* str) {
	char* upd = malloc(sizeof(str));
	for (int i = 0; str[i] != '\0'; i++) {
		upd[i] = toupper((unsigned char)str[i]);
	}
	upd[strlen(upd)-1] = '\0';

	return upd;
}

char* readfile(const char* filename) {
	FILE* file = fopen(filename, "rb");
	if (!file) {fprintf(stderr, "File opening failed\n"); fflush(stderr); return NULL;}

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	char* buffer = malloc(size+1);
	if (!buffer) {fclose(file); return NULL;}

	fread(buffer, 1, size, file);
	buffer[size] = '\0';

	fclose(file);
	return buffer;
}