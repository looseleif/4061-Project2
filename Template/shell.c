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
		exit(0);
	}

	//allocated memory for and stores the current working directory file path
	currentDirectory = (char *) malloc(sizeof(char) * (PATH_SIZE + 1));

	//error checking for malloc
	if(currentDirectory == NULL)
	{
		perror("ERROR: failed to allocate memory");
		free(currentDirectory);
		exit(0);
	}

	//gets current working directory & checks for error
	if(getcwd(currentDirectory, PATH_SIZE) == NULL)
	{
		perror("ERROR: failed to get current working directory");
		free(currentDirectory);
		exit(0);
	}

	//creating process to execute clear
	pid_t p1 = fork();
	if(p1 < 0) //error
	{
		perror("ERROR: failed to fork p1");
		free(currentDirectory);
		exit(0);
	}
	else if (p1 == 0) //child
	{
		free(currentDirectory);
		execl("/bin/clear", "/bin/clear", (char *) NULL);

		//error checking execl
		perror("ERROR: failed execute clear");
		exit(0);
	}
	else  //parent
	{
		//wait for p1, error checking
		if(waitpid(p1, NULL, 0) < 0) {perror("ERROR: failed to wait for child p1"); free(currentDirectory); exit(0);}

		// Initialization should end here. Now, we must print the cwd and wait for input. While loop might be a good idea

		//prints out the tag and the cwd
		printf("\n%s%s $ \n", SHELL_TAG, currentDirectory);

		//TODO: free memory in exit()
		free(currentDirectory);
		return 0;
	}
}