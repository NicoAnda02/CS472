#include<stdio.h>
#include<stdlib.h>
#include<string.h>


 int main(int argc, char *argv[]) {
 	FILE *fileIn; 
 	int i = 0; 
 	char prev = 0, curr = 0;
 	int count = 0;	
 	
 	if(argc < 2) {
 		printf("wzip: file1 [file2 ...]\n");
 		exit(1);
 	}
 	for(i = 1; i < argc; i++) 	{
 		fileIn = fopen(argv[i], "r");
 		
 		if(fileIn == NULL) {
 		printf("wzip: cannot open file\n");
 		exit(1);
 		} 
		while(1) {
 			curr = fgetc(fileIn);
 			if(curr == EOF) {

 				if(count > 0) {
 					fwrite(&count, 1, 4, stdout);
 					fwrite(&prev, 1, 1, stdout);
 				}
 				break;
 			}
 			if(curr != prev) {

 				if(count > 0) {
 					fwrite(&count, 1, 4, stdout);
 					fwrite(&prev, 1, 1, stdout);
 				}
 				count = 0;
 			}
 			count++;
 			prev = curr;
 		}
 		fclose(fileIn);
 		
 	}
	return 0;
 }
