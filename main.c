#include<err.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

int main(int argc,char* argv[]){
	char* stdin[4];//array collecting 3 arguments for command
	stdin[3]=(char*)NULL;
	vector buff;//vector for reading a word
	init(&buff,20);
	char* final_arr[argc+3];//combination of argv+stdin
	int argc_new=argc;

	if(argc==1){//command to execute is echo
		final_arr[0]="echo";
		argc_new=2;
	}
	else{//command to execute is argv[1]
		final_arr[0]=argv[1];
	}
  for(int j=1;j<argc-1;++j){
  	final_arr[j]=argv[j+1]; 
  } 
	
		int word_beg=0;
		char c;
		int i=0;
		while ( read(0,&c,1)){//while reading from stdin
			if(c==' '  || c=='\n' || c=='\t'){//first space after end of word
				if( word_beg==1 ){
						word_beg=0;
						stdin[i]=buff.copy_arr(&buff);
						i++;
				}
	
			}
			
			else{ //reading chars from word
					word_beg=1;
					buff.append(&buff,c);
			}
			if( i==3 ){//3 arguments colleceted, executing command
				int pid;
				pid=fork();
				if( pid==0 ){
					for(int j=0;j<=i;++j){
						final_arr[j+argc_new-1]=stdin[j];	
					}
					execvp(final_arr[0],final_arr);
				}
				int status;
				wait(&status);
				i=0;
				for(int k=0;k<i;++k){
					free(stdin[k]);
				}

			}
		}
		
		if(i!=0){//executing command once more if less than 3 arguments left
			
				int pid;
				pid=fork();
				stdin[i]=(char*)NULL;
				if( pid==0 ){
					for(int j=0;j<=i;++j){
						final_arr[j+argc_new-1]=stdin[j];	
					}
					execvp(final_arr[0],final_arr);
				}
				int status;
				wait(&status);
				i=0;
				for(int k=0;k<i;++k){
						free(stdin[k]);
				}
		}	
	
}


