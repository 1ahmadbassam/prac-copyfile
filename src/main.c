#include <stdio.h>
#include <stdlib.h>

#include "includes/includes.h"
#include "includes/string.h" 

/* Cross-compatibility between systems */
#ifdef _WIN32
#include <io.h>
#define R_OK 4
#define access _access
#define realpath 
#else
	#include <unistd.h>
#endif

#define BINARY 0
#define ASCII 1
/* Size of buffer technically controls read-speed in binary mode. */
#define BUF_SIZE 16

const char* appendcopyname(const char *src) {
	int				i,
					srcsize,
					fileextpos;

	const char 		COPYEXT[6] = "-copy";
	char			*dest;

	srcsize = strsize(src);
#define DEST_MALLOC
	dest = malloc(srcsize + 5);

	/* Determine file extension position */
	fileextpos = srcsize;
	while (fileextpos >= 0 && src[fileextpos] != '.') fileextpos--;
	if (fileextpos < 0) fileextpos = srcsize - 1;
	/* Copy file name to destination and add copy extension to name */
	strcopy(src, dest, 0, fileextpos);
	for (i = 0; i < 5; i++) dest[fileextpos + i] = COPYEXT[i];
	/* Append extension if needed */
	if (fileextpos < srcsize)
		for (i = fileextpos; i < srcsize; i++)
			dest[i + 5] = src[i];
	return dest;
}

int main(int argc, char *argv[])
{
	int 			i,
					canaccessfile,
					readchar,
					mode = BINARY;
	size_t			readel;	
	long 			bytes = 0;


	const char		*src,
		 			*dest;

	char			buf[BUF_SIZE],
					*temp,
					*temp2;

	FILE 			*srcfile,
					*destfile;

	if (argc <= 1) {
		printf("Required parameter missing: source file\n");
		printf("Usage: copyfile <source file path> <optional destination file path> <optional read mode>\n");
		printf("Valid read modes are: binary (default) OR ascii.\n");
		printf("NOTE: If destination file exists, then it WILL BE OVERWRITTEN.");
		return 0;
	} else if (argc == 2) {
		src = argv[1];
		dest = appendcopyname(src);

		printf("Copying %s to %s", src, dest);
	} else {
		if (argc > 4) printf("Ignoring additional arguments...\n");
		else if (argc == 4) {
			temp = (char*) trim(argv[3]);
			temp2 = (char*)tolowerstr(temp);
#ifdef TRIM_USES_MALLOC
			free(temp);
			temp = NULL;
#endif
			if (!compstr(temp2, "ascii")) {
				printf("Copying in ASCII mode.\n");
				mode = ASCII;
			} else if (compstr(temp2, "binary")) {
				printf("ERR: Invalid mode. Defaulting to binary mode.\n");
			} else printf("Copying in binary mode.\n");
#ifdef TOLOWERSTR_USES_MALLOC
			free(temp2);
			temp2 = NULL;
#endif
		}
		src = argv[1];
		dest = argv[2];

		printf("Copying %s to %s", src, dest);
	}
	
#ifdef _WIN32
	src = (char*) _fullpath(NULL, src, 0);
#else
	src = (char*) realpath(src, NULL);
#endif

#ifdef DEST_MALLOC
	#ifdef _WIN32
	temp = (char*) _fullpath(NULL, dest, 0);
	free((char*) dest);
	dest = temp;
	#else
	temp = (char*) realpath(dest, NULL);
	free((char*) dest);
	dest = temp;
	#endif
#else
	#ifdef _WIN32
	dest = (char*) _fullpath(NULL, dest, 0);
	#else
	dest = (char*) realpath(dest, NULL);
	#endif
#endif

	/* For safety purposes, reset the temp pointers */
	temp = NULL; 
	temp2 = NULL;

	if (!compstr(src, dest)) {
			printf("\nERR: source and destination filenames are the same.");
			free((char*) src);
			free((char*) dest);
			return -28;
	}

	canaccessfile = access(src, R_OK);

	if (!canaccessfile) {
		if (mode == ASCII) {
			srcfile = fopen(src, "r");
			if (srcfile) {
				printf("\nOK: File opened in ASCII mode for reading.");
				destfile = fopen(dest, "w");
				if (destfile) {
					printf("\nOK: File opened in ASCII mode for writing.");
					readchar = fgetc(srcfile);
					while (readchar != EOF) {
						fputc(readchar, destfile);
						bytes++;
						readchar = fgetc(srcfile);
					} 
					fclose(destfile);
				} else {
					printf("\nERR: Unknown error. File cannot be opened for writing.");
					fclose(srcfile);
					return -53;
				}
				fclose(srcfile);
			} else {
				printf("\nERR: Unknown error. File cannot be opened for reading.");
				return -52;
			}
		} else if (mode == BINARY) {
			srcfile = fopen(src, "rb");
			if (srcfile) {
				printf("\nOK: File opened in binary mode for reading.");
				destfile = fopen(dest, "wb");
				if (destfile) {
					printf("\nOK: File opened in binary mode for writing.");
					readel = fread(buf, sizeof(unsigned char), BUF_SIZE, srcfile);
					while (readel == BUF_SIZE) {
						i = fwrite(buf, sizeof(unsigned char), BUF_SIZE, destfile);
						if (i == EOF) {
							printf("\nERR: Unknown error. An unknown error occured while writing.");
							fclose(srcfile);
							fclose(destfile);
							free((char*) src);
							free((char*) dest);
							return -54;
						}
						bytes += BUF_SIZE;
						readel = fread(buf, sizeof(unsigned char), BUF_SIZE, srcfile);
					}
					i = fwrite(&buf, sizeof(unsigned char), readel, destfile);
					if (i == EOF) {
							printf("\nERR: Unknown error. An unknown error occured while writing.");
							fclose(srcfile);
							fclose(destfile);
							free((char*) src);
							free((char*) dest);
							return -54;
					}
					bytes += readel;
					fclose(destfile);
				} else {
					printf("\nERR: Unknown error. File cannot be opened for writing.");
					fclose(srcfile);
					free((char*) src);
					free((char*) dest);
					return -53;
				}
				fclose(srcfile);
			} else {
			printf("\nERR: Unknown error. File cannot be opened for reading.");
			free((char*) src);
			free((char*) dest);
			return -52;
			} 
		} else printf("\nERR: Invalid mode requested. Contact program support team.");
	} else {
		printf("\nERR: File cannot be accessed for reading. Error code %d.", canaccessfile);
		free((char*) src);
		free((char*) dest);
		return -51;
	}
    printf("\nSUCCESS: File copy successful. Copied %ld bytes.", bytes);
	free((char*) src);
	free((char*) dest);
	return 0;
}
