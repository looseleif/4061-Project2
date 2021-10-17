#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>

#include "util.h"

int main(){
	/*write code here*/

	
	char * currentDirectory;						//stores the cwd path
	char SHELL_TAG[] = "[project-4061]";			//tag for printing out the cwd
	int PATH_SIZE = pathconf(".", _PC_PATH_MAX);	//gets the PC specific maximum path length for use in memory allocation

	//error checking for pathconf
	errno = 0;
	if(PATH_SIZE < 0)
	{
		perror("ERROR: failed to retrieve maximum path size");
	}

	//allocated memory for and stores the current working directory file path
	currentDirectory = (char *) malloc(sizeof(char) * (PATH_SIZE + 1));

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

	//TODO: execute "clear" so the shell looks better

	//prints out the tag and the cwd
	printf("\n%s%s", SHELL_TAG, currentDirectory);

}