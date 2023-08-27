#include <stdio.h>
#include <stdlib.h>
#include "includes/string.h"

int strsize(const char* str) {
	int i = 0;

	while (str[i] != '\0') i++;
	return i + 1;
}

int strcopy(const char* src, char* dest, int start, int end) {
	int i;

	for (i = start; i < end; i++) {
		dest[i - start] = src[i];
	}
	if (dest[end - start - 1] != '\0') dest[end - start] = '\0';
	return 0;
}

const char* trim(const char* str) {
	int start = 0, size = strsize(str), end = size;
	char* trimmed;

	while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') start++;
	while (str[end] == ' ' || str[end] == '\t' || str[end] == '\n') end--;

	trimmed = malloc(end - start + 2);
	strcopy(str, trimmed, start, end);
	return trimmed;
}

int compstr(const char* str1, const char* str2) {
	int i;

	if (strsize(str1) != strsize(str2)) return 1;
	for (i = 0; i < strsize(str1); i++) {
		if (str1[i] != str2[i]) return 1;
	}
	return 0;
}	

int nullstr(const char* str, int size) {
	int i;

	for (i = 0; i < size; i++) {
		if (str[i] != EOF) {
			return 1;
		}
	}
	return 0;
}
