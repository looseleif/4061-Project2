#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>

void cd(char* arg){
	/*insert code here*/
}

int main(int argc, char** argv){

	if(argc<2){
		printf("Pass the path as an argument\n");
		return 0;
	}
	cd(argv[1]);
	return 0;
}