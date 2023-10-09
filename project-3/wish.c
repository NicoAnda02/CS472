#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_PATHS 100
#define MAX_ARGS 50
char error_message[] = "An error has occurred\n";
int iterate;
FILE *pathFile; //file for storing search paths
char cwd[MAX_PATHS];
char *textname;
void execute_command(char* input, char* args[], int tf, char* outFile);

int main(int argc, char* argv[]) {
    FILE *fp;
    char* string = NULL;
    size_t size = 0;
    int linelen = 0, v;
    char* args[MAX_ARGS], *filename = malloc(100 * sizeof(char));
    char** parsed = malloc(30 * sizeof(char*));
    textname = getcwd(cwd, sizeof(cwd));
	strcat(textname, "/paths.txt");
    
    pathFile = fopen(textname, "w");
    if (pathFile == NULL) {
        printf("File error1\n");
        exit(0);
    }
    fprintf(pathFile, "/bin");
    fclose(pathFile);

    if (argc == 1) { //no batch file
        printf("wish> ");

        while ((linelen = getline(&string, &size, stdin)) > 0) { //input
        
            v = 0; //represents number of commands separated by &
            	   //resets to 0 every input

            if (strstr(string, "&") == NULL) { //parse each command by '&'
                parsed[v] = string;
            } else {
                while ((parsed[v] = strsep(&string, "&")) != NULL) {
                    if (strcmp(parsed[v], "") == 0) {
                        parsed[v] = NULL; //ends while loop search
                    } else {
                        v++; //increment search variable to execute
                        	 //commands
                    }
                }
            }

            if (v == 0) //base case iteration count
                v++;
            
            for (iterate = 0; iterate < v; iterate++) { //executes
                execute_command(parsed[iterate], args, 0, filename);
            }
            
            printf("wish> ");
        }
    } else if (argc == 2){ //batch file reader
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
        	write(STDERR_FILENO, error_message, strlen(error_message));
            exit(1);
        }

        while ((linelen = getline(&string, &size, fp)) != -1) {
            
            v = 0; //represents number of commands separated by &

            if (strstr(string, "&") == NULL) {
                parsed[v] = string;
            } else {
                while ((parsed[v] = strsep(&string, "&")) != NULL) {
                    if (strcmp(parsed[v], "") == 0) {
                        parsed[v] = NULL;
                    } else {
                        v++;
                    }
                }
            }

            if (v == 0)
                v++;
            
            for (iterate = 0; iterate < v; iterate++) { //executes
                execute_command(parsed[iterate], args, 0, filename);
            }
            
        }

        fclose(fp);
        exit(0);
    } else { //too many arguments
    	write(STDERR_FILENO, error_message, strlen(error_message));
        exit(1);
    }

    free(parsed);
    free(string);
    free(filename);
    remove(textname);
    return 0;
}

void execute_command(char* input, char* args[], int tf, char* outFile) {
//Function to execute each command line

    int status;
    int nArgs;
    char command[100]; //system command being run
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    
    if (strstr(input, "\n") != NULL) { //remove \n from input
                   input[strcspn(input, "\r\n")] = 0;
                }

                tf = 0;
                int i = 0, t = 0, j = 1, k = 0, redirFlag = -1;
                char* words[50], *rdwords[50]; //words refers to words in
                							   //command line separated by
                							   //space, rdwords are lines
                							   //separated by >

               if (strchr(input, '>') == NULL) { //there is no redirection
                while ((words[i] = strsep(&input, "   \t")) != NULL) {
                    if (strcmp(words[i], "") == 0) {
                        words[i] = NULL; //removes blank space
                    } else {
                        i++; //represents number of words
                             //if i = 0, there are no words
                    }
                }
               } else { //redirection command found
               
		           	while ((rdwords[t] = strsep(&input, ">")) != NULL) {
		           		redirFlag++; //0 if no redirection, 1 if there is
		           					 //2 if error
		           		if (t == 1) {
		           			char *fileError = rdwords[t];
		           			char *fileChecker[5];
		           			if (strcmp(fileError, "") == 0) {
		           				redirFlag = 2;
		           				break;
		           			}
		           			
		           			//work on error checking outFile
		           			while ((fileChecker[k] = strsep(&fileError, "   \t")) != NULL) {
                    			if (strcmp(fileChecker[k], "") == 0) {
                        			fileChecker[k] = NULL;
                    			} else {
                    				outFile = fileChecker[k];
                        			k++;
                    			}
                			}
                			if (k > 1 || strcmp(outFile, "") == 0)
                				redirFlag = 2;
		           		}
		           		
		           		if (t >= 2)
		           			break;
		           		t++;
		           	}
		           	
		           	if (strcmp(rdwords[0], "") == 0)
		           		redirFlag = 2;
		           	
		           	while ((words[i] = strsep(&rdwords[0], "   \t")) != NULL) {
		                if (strcmp(words[i], "") == 0) {
		                    words[i] = NULL; //removes blank space
		                } else {
		                    i++; //represents number of words
		                         //if i = 0, there are no words
		                }
		            }
               
               }

                if (words[0] == NULL && redirFlag == 2) {
		        	write(STDERR_FILENO, error_message, strlen(error_message));
                } else if (words[0] == NULL) { //empty shell input
//                printf("\n");
//                    write(STDERR_FILENO, error_message, strlen(error_message));
                } else if (strcmp(words[0], "exit") == 0) { //exit command
                    
                    if (words[1] != NULL) { //nothing should be after exit
//                    printf("2\n");
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        remove(textname);
                        exit(0);
                    }
                } else if (strcmp(words[0], "cd") == 0) { //cd command
                    if (words[1] == NULL || words[2] != NULL) {
                        write(STDERR_FILENO, error_message, strlen(error_message));
                    } else {
                        if (chdir(words[1]) == 0) { //cd successful
                            
                        } else { //directory not found
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        }
                    }
                } else if (strcmp(words[0], "path") == 0) { //change path
                    j = 0; // Initialize j to 0
                    pathFile = fopen(textname, "w");
                    if (pathFile == NULL) {
                            printf("File error2\n");
                            exit(0);
			 		}
                    
                    for (j = 0; j < i; j++) { //turn args into paths
                        if (words[j + 1] == NULL && j == 0) {
                            fprintf(pathFile, "%s", " ");
                            break;
                        }
			 	 		if (words[j + 1] != NULL)
			     			fprintf(pathFile, "%s\n", words[j + 1]);
                    }
                    fclose(pathFile);
                } else {
                
                    pathFile = fopen(textname, "r");
		        	if (pathFile == NULL) {
                    	printf("File error3\n");
                        exit(0);
		        }
                
                while ((read = getline(&line, &len, pathFile)) != -1) { //check each path on pathfile
			 
			    	if (strstr(line, "\n") != NULL) {
                    	line[strcspn(line, "\r\n")] = 0;
                    }
                            
                    strcpy(command, line);
                    strcat(command, "/");
                    strcat(command, words[0]);
                    strcat(command, "\0");

                    for (nArgs = 0; nArgs < i; nArgs++) {
                         if (nArgs == 0) {
                             args[nArgs] = command; //pass args to command in execv()
                         } else {
                         	args[nArgs] = words[nArgs]; //populate args
                            }
                        }

                        args[i] = NULL; //null terminate argument array

                        if (redirFlag >= 2) { //bad redirection command
                            write(STDERR_FILENO, error_message, strlen(error_message));
                        } else if (access(command, X_OK) == 0) {
                            if (fork() == 0) {
                                if (redirFlag == 0) { //no redirection
                                    execv(command, args);
                                } else { //redirection
                                    int fd = open(outFile, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
                                    dup2(fd, 1);
                                    dup2(fd, 2);
                                    close(fd);
                                    execv(command, args);
                                }
                            } else {
                                wait(&status); //wait for forked process
                                break;
                            }
                        } else { //command not executable
                            
                            int c = fgetc(pathFile);
							if (c == EOF) {
    							write(STDERR_FILENO, error_message, strlen(error_message));
							} else {
    							ungetc(c, pathFile);
							}
                        }
                    }
                    fclose(pathFile);
                }
    
}
