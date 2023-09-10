#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char* argv[]){

	FILE *file;
	char line[1024];
	
	if(argc < 2){
		printf("wgrep: searchterm [file ...]\n");
		exit(1);
	}
	
	if(argc < 3) { //wgrep from stdin
	
		while (fgets(line , sizeof(line) , stdin )!= NULL){
		
	      		if (strstr(line , argv[1] )!= NULL)
	      		{
		 		printf("%s",line);
	     		}
	  	}
	
	}
	
	for (int i = 2; i < argc; i++){
	
		file = fopen(argv[i], "r");
		if (file == NULL){
			printf("wgrep: cannot open file\n");
			exit(1);
		}
		while (fgets(line , sizeof(line) , file )!= NULL){
		
	      		if (strstr(line , argv[1] )!= NULL)
	      		{
		 		printf("%s",line);
	     		}
	  	}
	
	}

}
