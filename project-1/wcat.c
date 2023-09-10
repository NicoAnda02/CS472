#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv){

	FILE *file;
	char *buffer = NULL;
	long bufsize;

	if (argc < 2){
		exit(0);
	}
	
	for (int i = 1; i < argc; i++){
	
		bufsize = -1;
		file = fopen(argv[i], "r");

		if (file == NULL){
			printf("wcat: cannot open file\n");
			exit(1);
		}

		if (fseek(file, 0L, SEEK_END) == 0){
			bufsize = ftell(file);
			if (bufsize == -1){	
				printf("Could not return size\n");
				exit(1);
			}
		}
		buffer = malloc(sizeof(char) * (bufsize + 1));

		if (fseek(file, 0L, SEEK_SET) != 0) {
			printf("An error occured\n");
			exit(1);
		}

		size_t newLen = fread(buffer, sizeof(char), bufsize, file);
		if (ferror(file) != 0){
			fputs("Error reading file", stderr);
		} else {
			buffer[newLen++] = '\0';
		}

		printf("%s", buffer);

		free(buffer);
		fclose(file);
	}

}
