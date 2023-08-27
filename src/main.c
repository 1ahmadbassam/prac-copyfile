#include <stdio.h>
#include <stdlib.h>

/* Cross-compatibility between systems */
#ifdef _WIN32
#include <io.h>
#define R_OK 4
#define access _access
#define realpath _fullpath
#else
	#include <unistd.h>
#endif

#include "includes/includes.h"
#include "includes/string.h"

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

	dest = malloc(strsize(src) + 5);
	
	/* Determine file extension position */
	fileextpos = srcsize;
	while (fileextpos >= 0 && src[fileextpos] != '.') fileextpos--;
	if (fileextpos < 0) fileextpos = srcsize;
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
	
	char			buf[BUF_SIZE]; 

	FILE 			*srcfile, 
					*destfile;

	if (argc <= 1) {
		printf("Required parameter missing: source file\n");
		printf("Usage: copyfile <source file path> <optional destination file path>\n");
		printf("NOTE: If destination file exists, then it WILL BE OVERWRITTEN.");
		return 0;
	} else if (argc == 2) {
		src = argv[1];
		dest = appendcopyname(src);

		printf("Copying %s to %s", src, dest);
	} else {
		if (argc > 3) printf("Ignoring additional arguments...\n");
		src = argv[1];
		dest = argv[2];

		printf("Copying %s to %s", src, dest);
	}

	src = realpath(0, src, 0);
	dest = realpath(0, dest, 0);

	if (!compstr(src, dest)) {
			printf("\nERR: source and destination filenames are the same.");
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
					readel = fread(buf, BUF_SIZE, 1, srcfile);
					while (readel == 1) {
						if (!fwrite(buf, BUF_SIZE, 1, destfile)) {
							printf("\nERR: Unknown error. An unknown error occured while writing.");
							fclose(srcfile);
							fclose(destfile);
							return -54;
						}
						readel = fread(buf, BUF_SIZE, 1, srcfile);
					}
					for (i = 0; i < BUF_SIZE; i++) {
						if (buf[i] == EOF) break;
						if (!fwrite(&buf[i], 1, 1, destfile)) {
							printf("\nERR: Unknown error. An unknown error occured while writing.");
							fclose(srcfile);
							fclose(destfile);
							return -54;
						}
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
		} else {
			printf("\nERR: Invalid mode requested. Contact program support team.");
		}
	} else { 
		printf("\nERR: File cannot be accessed for reading. Error code %d.", canaccessfile);
		return -51;
	}
    printf("\nSUCCESS: File copy successful. Copied %d bytes.", bytes);
	return 0;
}
