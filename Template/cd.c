#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <errno.h>

void cd(char* arg){
	/*insert code here*/
	
	//gets the PC specific maximum path length for use in memory allocation
	int PATH_SIZE = pathconf(".", _PC_PATH_MAX);

	//error checking for pathconf
	errno = 0;
	if(PATH_SIZE < 0)
	{
		perror("ERROR: failed to retrieve maximum path size");
	}

	//allocated memory for and stores the current working directory file path
	char * currentDirectory = (char *) malloc(sizeof(char) * (PATH_SIZE + 1));

	//error checking for malloc
	if(currentDirectory == NULL)
	{
		perror("ERROR: failed to allocate memory");
	}

	//gets current working directory & checks for error
	if(getcwd(currentDirectory, PATH_SIZE) == NULL)
	{
		perror("ERROR: failed to get current working directory");
	}

	//printf("%s \n", arg);

	//chdir & error checking
	if(chdir(arg) < 0)
	{
		perror("ERROR: failed to change directory");
	}

	//gets current working directory & checks for error
	if(getcwd(currentDirectory, PATH_SIZE) == NULL)
	{
		perror("ERROR: failed to get current working directory");
	}

	//prints current working directory and checks for errors
	if(printf("%s\n", currentDirectory) < 0)
	{
		perror("ERROR: failed to print current working directory");
	}
	
	free(currentDirectory);
}

int main(int argc, char** argv){

	if(argc<2){
		printf("Pass the path as an argument\n");
		return 0;
	}
	cd(argv[1]);
	return 0;
}