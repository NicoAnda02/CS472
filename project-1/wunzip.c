#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){

	FILE *fileIn;
	char curr;
	int count = 0;
	if(argc < 2){
		printf("wunzip: file1 [file2 ...]\n");
		exit(1);
	}
	
	for (int i = 1; i < argc; i++){
		
		fileIn = fopen(argv[i], "rb");
		
		if (fileIn == NULL){
			printf("wzip: cannot open file\n");
			exit(1);
		}

		while(1){ 
			int bytes = fread(&count, 1, 4, fileIn);
 			if(bytes != 4){
 				break; 		
			} 		
			fread(&curr, 1, 1, fileIn); 		
		 			
			int i = 0; 			
			for(i = 0; i < count; i++) {
 				fwrite(&curr, 1, 1, stdout); 		
			} 
		}
	
		fclose(fileIn); 
	}
	

}
