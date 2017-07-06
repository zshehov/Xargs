#include <err.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

void exec_command(char** final_arr, char** stdin, int* current_arg, int argc_new){
	int pid;
	if ( ( pid = fork() ) == 0){ // in the child process

		for(int j = 0; j <= *current_arg; ++j)
			final_arr[j + argc_new - 1] = stdin[j];	
				
		execvp(final_arr[0], final_arr);
	}
	
	int status;
	wait(&status);
	*current_arg = 0;

	for(int k = 0; k < *current_arg; ++k)
		free(stdin[k]);
}

int main(int argc, char* argv[]){
	vector buff;//vector for reading a word
	init(&buff, 20); // initializes the vector structure with variable capacity
	
	char* stdin[4];//array collecting 3 arguments for command
	stdin[3] = (char*)NULL;
	
	char* final_arr[argc + 3];//combination of argv+stdin
	int argc_new = argc;
	
	int word_beg = 0;
	int current_arg = 0;
	char c;
	
	if(argc == 1){//command to execute is echo
		final_arr[0] = "echo";
		argc_new = 2;
	}
	else{//command to execute is argv[1]
		final_arr[0] = argv[1];
	}
	
  	for(int j = 1 ; j < argc - 1; ++j)
  		final_arr[j] = argv[j + 1]; 

	
	while ( read(0, &c, 1) ){//while reading from stdin
		if(c == ' '  || c == '\n' || c == '\t'){//first space after end of word
			if( word_beg == 1 ){
				word_beg = 0;
				stdin[current_arg] = buff.copy_arr(&buff);
				++current_arg;
			}
		}
		
		else{ //reading chars from word
			word_beg = 1;
			buff.append(&buff, c);
		}
			
		if( current_arg == 3 )//3 arguments colleceted, executing command
			exec_command(final_arr, stdin, &current_arg, argc_new);
	}	
	
	
	if( current_arg != 0 ){ //executing command once more if less than 3 arguments left
		stdin[current_arg] = (char*) NULL;
		exec_command(final_arr, stdin, &current_arg, argc_new);
	}
}
