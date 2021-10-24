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

int thePipeProcess(char* commands[50], int Index, int cmdCounter, char * TEMPLATE_DIR) {

	char** inPipe = (char**)malloc(sizeof(char**) * Index);;
	char** outPipe = (char**)malloc(sizeof(char**) * (cmdCounter - Index));; // check later
	char*** pipeSelect = (char***)malloc(sizeof(char***));
	int start, end;
	int ifRedirected = 0;
	int retVal = 0;
	int file_desc;
	int terminal = dup(STDOUT_FILENO);
	int inputFD = dup(STDIN_FILENO);

	int i;

	//fill inPipe with the command before the pipe
	for (i = 0; i < Index; i++) 
	{
		inPipe[i] = (char*)malloc(100);
		strcpy(inPipe[i], commands[i]); 

	}

	//fill outPipe with the command after the pipe
	int j;
	for (i = 0, j = Index + 1; j < cmdCounter; j++, i++)
	{
		outPipe[i] = (char*)malloc(100);
		strcpy(outPipe[i], commands[j]);
		
		//Checking for file redirection, copy of main
		if(strcmp(outPipe[i], ">") == 0) //if no append
		{
			fclose(fopen(commands[j+1], "w")); //clears the output file
			file_desc = open(commands[j + 1], O_WRONLY | O_CREAT); //opens the file
			if(file_desc < 0) //error checking
			{
 				printf("Error opening the file\n");
			}
			//for clearing purposes
			ifRedirected = 1;
		}
		else if (strcmp(outPipe[i], ">>") == 0) //if append
		{				
			file_desc = open(commands[j + 1], O_WRONLY | O_APPEND | O_CREAT); //opens the file
			if(file_desc < 0) //error checking
			{
 				printf("Error opening the file\n");
			}
			//for clearing purposes
			ifRedirected = 1;
		}
	}

	// DONE WITH CMD SEGMENTATION
	int fd[2];

	//Error checking
	if (pipe(fd) < 0) {

		printf("Command Error\n");
		free(inPipe);
		free(outPipe);
		return -1;
	}

	pid_t pipePID = fork();

	//Error checking
	if (pipePID < 0) {

		perror("Command Error");
		free(inPipe);
		free(outPipe);
		return -1;

	}
	//if it is the parent, it will set the pipeSelect string pointer equal to the outPipe
	else if (pipePID > 0) 
	{
		wait(NULL);

		pipeSelect[0] = outPipe;
		start = Index + 1;
		end = cmdCounter;

	}

	//child input
	else 
	{

		if (dup2(fd[1], STDOUT_FILENO) < 0) 
		{

			fprintf(stderr, "ERROR: Failed dup2... fd:\n");
		}
		
		//Sets the pipeSelect string pointer equal to the inPipe
		pipeSelect[0] = inPipe;
		start = 0;
		end = Index;

	}

	// OUTSIDE OF FORK CONDITIONS

	pid_t c2PID;
	
	//Checking for cd 
	if (strcmp(pipeSelect[0][0], "cd") == 0)
	{
		//if there are more than two arguments
		if ((end - start) != 2)
		{
			printf("Command error1\n");

		}

		//Error checking
		else if (chdir(pipeSelect[0][1]) < 0) 
		{
			perror("[4061-shell]: ");
		}
	}
	
	//Checking for cd or ls and executing
	else if ((strcmp(pipeSelect[0][0], "ls") == 0) || (strcmp(pipeSelect[0][0], "wc") == 0))
	{
		//Parent of two children
		if(pipePID > 0) //SUPER PARENT
		{
			c2PID = fork();

			//Error checking
			if (c2PID < 0) 
			{
				printf("Command error2\n");
			}	
		}
		
		//Runs only for child processes
		if (pipePID == 0 || c2PID == 0)
		{
			//Allocates memory for path and puts it in pipeSelect
			char* ABS_PATH_BUF = (char*)malloc(1000);
			sprintf(ABS_PATH_BUF, "%s/%s", TEMPLATE_DIR, pipeSelect[0][0]);
			pipeSelect[0][0] = ABS_PATH_BUF;

			//Runs only for the second child
			if (c2PID == 0) 
			{
				if (dup2(fd[0], STDIN_FILENO) < 0) 
				{
					fprintf(stderr, "ERROR: Failed dup2 for child c2 \n");
				}
			}
			
			//TESTING
			if (pipePID == 0) 
			{
				close(fd[0]);
				close(fd[1]);
			}
			
			
			if (c2PID==0 && ifRedirected) 
			{
				//Error checking
				if (dup2(file_desc, STDOUT_FILENO) < 0) 
				{

					fprintf(stderr, "ERROR: Failed dup2 for file redirection for c2 fd:\n");

				}
			}

			close(fd[0]);
			close(fd[1]);

			//Finds the position of the file redirection char
			int j = 0;
			while (j < (end - start) && (strcmp(pipeSelect[0][j], ">") != 0) && (strcmp(pipeSelect[0][j], ">>") != 0))
			{
				j++;
			}

			char** args = (char**)malloc(sizeof(char**) * j + 1);

			//Creates the argument array and sets it equal to pipeSelect
			//pipeSelect represents either the inPipe or outPipe
			int i;
			for (i = 0; i < j; i++)
			{
				args[i] = (char*)malloc(100);
				strcpy(args[i], pipeSelect[0][i]);
			}

			//Sets the last character in the argument array to NULL
			args[j] = (char*)NULL;

			execv(args[0], args);

			//Error checking
			printf("FAIL\n");
			free(ABS_PATH_BUF);
			free(args);
			retVal = -1;
		}

		//parent
		else
		{
			close(fd[0]);
			close(fd[1]);

			//wait for lsPID, error checking
			if (waitpid(c2PID, NULL, 0) < 0)
			{
				printf("Command error3\n");
				exit(0);
			}
		}
	}

	//If the input command is not wc, ls, or cd
	else
	{

		if(pipePID > 0) //SUPER PARENT
		{
			c2PID = fork();

			//Error checking
			if (c2PID < 0)
			{
				printf("C2 forking error\n");
			}	
		}

		//Runs only for child processes
		if (c2PID == 0 || pipePID == 0)
		{
			//Finds the position of the file redirection char 
			int j = 0;
			while (j < (end - start) && (strcmp(pipeSelect[0][j], ">") != 0) && (strcmp(pipeSelect[0][j], ">>") != 0))
			{
				j++;
			}

			//Runs only for the second child
			if (c2PID == 0) 
			{
				if (dup2(fd[0], STDIN_FILENO) < 0) 
				{

					fprintf(stderr, "ERROR: Failed dup2 for child 2 (misc)\n");

				}

				if (ifRedirected) 
				{
					if (dup2(file_desc, STDOUT_FILENO) < 0) 
					{
						fprintf(stderr, "ERROR: Failed dup2 for child 2 (misc and file redirection)\n");
					}

				}

				close(fd[0]);
				close(fd[1]);

			}

			char** args = (char**)malloc(sizeof(char**) * j + 1);

			//Creates the argument array and sets it equal to pipeSelect
			int i;
			for (i = 0; i < j; i++)
			{
				args[i] = (char*)malloc(100);
				strcpy(args[i], pipeSelect[0][i]);
			}

			//Sets the last character in the argument array to NULL 
			args[j] = (char*)NULL;
			
			execvp(args[0], args);

			//Error checking
			printf("FAIL\n");
			free(args);
			retVal = -1;
		}
		
		//parent
		else
		{
			close(fd[0]);
			close(fd[1]);
			
			//wait for miscPID, error checking
			if (waitpid(c2PID, NULL, 0) < 0)
			{
				printf("Command error\n");
				exit(0);
			}
		}
	}
	
	//Error checking
	if (dup2(terminal, 1) < 0) 
	{
		fprintf(stderr, "ERROR: Failed dup2... fd:\n");
	}

	//Error checking
	if (dup2(inputFD, 0) < 0) 
	{
		fprintf(stderr, "ERROR: Failed dup2... fd: \n");
	}
	
	//If the program did file redirection, close file_desc
	if(ifRedirected == 1)
	{	
		
		close(file_desc);
		ifRedirected = 0;

	}
	
	//Free everything
	free(inPipe);
	free(outPipe);
	free(pipeSelect);
	close(fd[0]);
	close(fd[1]);
	
	return retVal;
}


int main(){
	
	char * currentDirectory;			//stores the cwd path
	char * TEMPLATE_DIR;
	char SHELL_TAG[] = "[project-4061]";		//tag for printing out the cwd
	int MAX_COMMAND_SIZE = 1000;
	int PATH_SIZE = pathconf(".", _PC_PATH_MAX);	//gets the PC specific maximum path length for use in memory allocation
	int redirectionType = 0;
	int isPiping = 0;
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
        			counter = counter + 1;
        			commandSplit[counter] = strtok(NULL, " \n\r");
   			}
		
			int pipeIndex;
			
			//checking for file redirection
			for(int i = 0; i < counter && isPiping == 0; i++) 
			{
				//Checking for pipe
				if (strcmp(commandSplit[i], "|") == 0) 
				{
					isPiping = 1;
					pipeIndex = i;

					//Error checking
					if (thePipeProcess(commandSplit, pipeIndex, counter, TEMPLATE_DIR) < 0) 
					{
						printf("Command Error\n");
						free(currentDirectory);
						free(buf);
						free(TEMPLATE_DIR);
						return 0;
					};


				}
				
				//Checking for file redirection
				else if(strcmp(commandSplit[i], ">") == 0) //if no append
				{
					//clears the output file
					fclose(fopen(commandSplit[i+1], "w"));
					
					//opens the file
					file_desc = open(commandSplit[i+1], O_WRONLY | O_CREAT); 
					if(file_desc < 0) //error checking
					{
       	 					printf("Error opening the file\n");
					}
					dup2(file_desc, 1); //changes the output to the file
					ifRedirected = 1;
				}

				else if (strcmp(commandSplit[i], ">>") == 0) //if append
				{
					//opens the file
					file_desc = open(commandSplit[i+1], O_WRONLY | O_APPEND | O_CREAT);
					if(file_desc < 0) //error checking
					{
       	 					printf("Error opening the file\n");
					}
					dup2(file_desc, STDOUT_FILENO); //changes the output to the file
					ifRedirected = 1;
				}
		}
		
		//If we didn't pipe, check for commands and run them
		if (isPiping == 0) 
		{
			//Checking for cd and running it
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
			
			//Checking for ls and wc and executing them
			else if((strcmp(commandSplit[0], "ls") == 0) || (strcmp(commandSplit[0], "wc") == 0))
			{
				pid_t customPID = fork();

				if (customPID < 0) //error
				{
					printf("Command error\n");
				}
				else if (customPID == 0) //child
				{
					//Making the absolute path to the command and putting into commandsplit
					char * ABS_PATH_BUF = (char*) malloc(1000);
					sprintf(ABS_PATH_BUF, "%s/%s", TEMPLATE_DIR, commandSplit[0]);
					commandSplit[0] = ABS_PATH_BUF;

					//Finding the index value of the file redirection
					int j = 0;
					while(j < counter && (strcmp(commandSplit[j], ">") != 0) && (strcmp(commandSplit[j], ">>") != 0))
					{
						j++;
					}

					char ** args = (char **) malloc(sizeof(char**) * j + 1);

					//Creating the argument array
					int i;
					for(i = 0; i < j; i++)
					{
						args[i] = (char *) malloc(100);
						strcpy(args[i], commandSplit[i]);
					}
					args[j] = (char *) NULL;
				
					execv(args[0],args);

					//Error checking
					printf("FAIL\n");
					free(currentDirectory);
					free(buf);
					free(TEMPLATE_DIR);
					return 0;
				}

				//parent
				else 
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
				
				//Error checking
				if (miscPID < 0) 
				{
					printf("Command error\n");
				}
				
				//Child
				else if (miscPID == 0)
				{
					//Finding the index value of the file redirection char
					int j = 0;
					while(j < counter && (strcmp(commandSplit[j], ">") != 0) && (strcmp(commandSplit[j], ">>") != 0))
					{
						j++;
					}

					char ** args = (char **) malloc(sizeof(char**) * j + 1);

					//Creating the argument array
					int i;
					for(i = 0; i < j; i++)
					{
						args[i] = (char *) malloc(100);
						strcpy(args[i], commandSplit[i]);
					}
					
					//Setting the final char in the argument array to NULL
					args[j] = (char *) NULL;
			
					execvp(args[0],args);

					//Error checking
					printf("FAIL\n");
					free(currentDirectory);
					free(buf);
					free(TEMPLATE_DIR);
					return 0;
				}

				//parent
				else
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
			
			//Resets the file descriptor and resets the standard output if we file redirected
			if(ifRedirected == 1)
			{	
				close(file_desc);
				dup2(terminal,1);
				ifRedirected = 0;
			}
		
		}
		//for pipeProcess to not execute main code
		else 
		{
			isPiping = 0;
		}

		//gets current working directory & checks for error
		if(getcwd(currentDirectory, PATH_SIZE) == NULL)
		{
			perror("ERROR: failed to get current working directory");
			free(currentDirectory);
			free(TEMPLATE_DIR);
			exit(0);
		}
		
		//Prints the shell tag and the current path
		printf("%s%s $ ", SHELL_TAG, currentDirectory); 

		//Waits and gets our next input
		fgets(buf, MAX_COMMAND_SIZE, stdin);
	}

	//exit
	free(currentDirectory);
	free(buf);
	free(TEMPLATE_DIR);
	return 0;
	}
}