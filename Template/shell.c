#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <fcntl.h>

#include "util.h"

int main(){
	/*write code here*/
	
	char * currentDirectory;						//stores the cwd path
	char * TEMPLATE_DIR;
	char SHELL_TAG[] = "[project-4061]";			//tag for printing out the cwd
	int MAX_COMMAND_SIZE = 1000;
	int PATH_SIZE = pathconf(".", _PC_PATH_MAX);	//gets the PC specific maximum path length for use in memory allocation
	int redirectionType = 0;
	int pipe = 0;
	int file_desc;
	int terminal = dup(STDOUT_FILENO);
	int ifRedirected = 0;

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

	//allocated memory for and stores the TEMPLATE filepath
	TEMPLATE_DIR = (char *) malloc(sizeof(char) * (PATH_SIZE + 1));

	//error checking for malloc
	if(TEMPLATE_DIR == NULL)
	{
		perror("ERROR: failed to allocate memory");
		free(currentDirectory);
		free(TEMPLATE_DIR);
		exit(0);
	}

	//gets current working directory & checks for error
	if(getcwd(currentDirectory, PATH_SIZE) == NULL)
	{
		perror("ERROR: failed to get current working directory");
		free(currentDirectory);
		free(TEMPLATE_DIR);
		exit(0);
	}

	//gets current working directory & checks for error
	if(getcwd(TEMPLATE_DIR, PATH_SIZE) == NULL)
	{
		perror("ERROR: failed to get current working directory");
		free(currentDirectory);
		free(TEMPLATE_DIR);
		exit(0);
	}

	//creating process to execute clear
	pid_t p1 = fork();
	if(p1 < 0) //error
	{
		perror("ERROR: failed to fork p1");
		free(currentDirectory);
		free(TEMPLATE_DIR);
		exit(0);
	}
	else if (p1 == 0) //child
	{
		free(currentDirectory);
		free(TEMPLATE_DIR);
		execl("/bin/clear", "/bin/clear", (char *) NULL);

		//error checking execl
		perror("ERROR: failed execute clear");
		exit(0);
	}
	else  //parent
	{
		//wait for p1, error checking
		//if(waitpid(p1, NULL, 0) < 0)
		if(wait(NULL) < 0)
		{
			perror("ERROR: failed to wait for child p1"); 
			free(currentDirectory); 
			free(TEMPLATE_DIR); 
			exit(0);
		}

		// Initialization should end here. Now, we must print the cwd and wait for input. While loop might be a good idea
		//prints out the tag and the cwd
		printf("\n%s%s $ ", SHELL_TAG, currentDirectory);

		char * buf = (char *) malloc(MAX_COMMAND_SIZE + 1);

		fgets(buf, MAX_COMMAND_SIZE, stdin); //recieve command input

		//error checking buf
		if(buf == NULL)
		{
			perror("ERROR: buf error");
			free(currentDirectory);
			free(buf);
			free(TEMPLATE_DIR);
			exit(0);
		}

		//will terminate when "exit" command is entered
		while(strcmp(buf, "exit\n") != 0)
		{
			char * commandSplit[50];
    		int counter = 0;
    
    		// Gets the first argument of the command (the command itself)
    		commandSplit[0] = strtok(buf, " \n\r");

    		// Get the rest of the arguments
    		while (commandSplit[counter] != NULL)
    		{
			//printf("%d: %s\n",counter, commandSplit[counter]);
        		counter = counter + 1;
        		commandSplit[counter] = strtok(NULL, " \n\r");
   			}
		   
		for(int i = 0; i < counter; i++) //checking for file redirection
		{
			if(strcmp(commandSplit[i], ">") == 0) //if no append
			{
				fclose(fopen(commandSplit[i+1], "w")); //clears the output file
				file_desc = open(commandSplit[i+1], O_WRONLY | O_CREAT); //opens the file
				if(file_desc < 0) //error checking
				{
       	 				printf("Error opening the file\n");
				}
				dup2(file_desc, 1); //changes the output to the file
				//printf("FD: %d \n", STDOUT_FILENO);
				//printf("FD: %d \n", terminal);
				//printf("FD: %d \n", file_desc);
				ifRedirected = 1;
			}
			else if (strcmp(commandSplit[i], ">>") == 0) //if append
			{
				
				file_desc = open(commandSplit[i+1], O_WRONLY | O_APPEND | O_CREAT); //opens the file
				if(file_desc < 0) //error checking
				{
       	 				printf("Error opening the file\n");
				}
				dup2(file_desc, STDOUT_FILENO); //changes the output to the file
				ifRedirected = 1;
			}
		}
		
		if(strcmp(commandSplit[0], "cd") == 0)
		{
			if(counter != 2)
			{
				printf("Command error\n");

			}
			else if(chdir(commandSplit[1]) < 0)
			{
				perror("[4061-shell]: ");
			}
		}
		else if((strcmp(commandSplit[0], "ls") == 0) || (strcmp(commandSplit[0], "wc") == 0))
		{
			pid_t customPID = fork();

			if (customPID < 0) //error
			{
				printf("Command error\n");
			}
			else if (customPID == 0) //child
			{
				char * ABS_PATH_BUF = (char*) malloc(1000);
				sprintf(ABS_PATH_BUF, "%s/%s", TEMPLATE_DIR, commandSplit[0]);
				commandSplit[0] = ABS_PATH_BUF;

				int j = 0;
				while(j < counter && (strcmp(commandSplit[j], ">") != 0) && (strcmp(commandSplit[j], ">>") != 0))
				{
					j++;
				}

				char ** args = (char **) malloc(sizeof(char**) * j + 1);

				int i;
				for(i = 0; i < j; i++)
				{
					args[i] = (char *) malloc(100);
					strcpy(args[i], commandSplit[i]);
				}
				args[j] = (char *) NULL;
			
				execv(args[0],args);
				printf("FAIL\n");
				free(currentDirectory);
				free(buf);
				free(TEMPLATE_DIR);
				return 0;
			}
			else //parent
			{
				//wait for lsPID, error checking
				if(waitpid(customPID, NULL, 0) < 0) 
				{
					printf("Command error\n"); 
					free(currentDirectory);
					free(buf);
					free(TEMPLATE_DIR);
					exit(0);
				}
			}
		}
		else
		{
			pid_t miscPID = fork();

			if (miscPID < 0) //error
			{
				printf("Command error\n");
			}
			else if (miscPID == 0) //child
			{
				int j = 0;
				while(j < counter && (strcmp(commandSplit[j], ">") != 0) && (strcmp(commandSplit[j], ">>") != 0))
				{
					j++;
				}

				char ** args = (char **) malloc(sizeof(char**) * j + 1);

				int i;
				for(i = 0; i < j; i++)
				{
					args[i] = (char *) malloc(100);
					strcpy(args[i], commandSplit[i]);
				}
				args[j] = (char *) NULL;
			
				execvp(args[0],args);
				printf("FAIL\n");
				free(currentDirectory);
				free(buf);
				free(TEMPLATE_DIR);
				return 0;
				//execvp(commandSplit[0],commandSplit);
			}
			else //parent
			{
				//wait for miscPID, error checking
				if(waitpid(miscPID, NULL, 0) < 0)
				{
					printf("Command error\n"); 
					free(currentDirectory); 
					free(buf); 
					free(TEMPLATE_DIR);
					exit(0);
				}
			}
		}

		if(ifRedirected == 1)
		{	
			close(file_desc);
			dup2(terminal,1);
			ifRedirected = 0;
		}

		//gets current working directory & checks for error
		if(getcwd(currentDirectory, PATH_SIZE) == NULL)
		{
			perror("ERROR: failed to get current working directory");
			free(currentDirectory);
			free(TEMPLATE_DIR);
			exit(0);
		}
			
		printf("%s%s $ ", SHELL_TAG, currentDirectory);   
		fgets(buf, MAX_COMMAND_SIZE, stdin);
	}

	//exit
	free(currentDirectory);
	free(buf);
	free(TEMPLATE_DIR);
	return 0;
	}
}