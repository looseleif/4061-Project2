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

	//printf("PipePro\n");

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

	//fill inPipe
	for (i = 0; i < Index; i++) {

		inPipe[i] = (char*)malloc(100);
		//printf("cmds: %s\n", commands[i]);
		strcpy(inPipe[i], commands[i]); 
		//printf("inPipe: %s\n", inPipe[i]);

	}

	//fill outPipe
	int j;
	for (i = 0, j = Index + 1; j < cmdCounter; j++, i++)
	{
		outPipe[i] = (char*)malloc(100);
		//printf("cmds: %s\n", commands[j]);
		strcpy(outPipe[i], commands[j]);
		//printf("outPipe: %s\n", outPipe[i]);

		if(strcmp(outPipe[i], ">") == 0) //if no append
		{
			fclose(fopen(outPipe[i+1], "w")); //clears the output file
			file_desc = open(outPipe[i+1], O_WRONLY | O_CREAT); //opens the file
			if(file_desc < 0) //error checking
			{
 				printf("Error opening the file\n");
			}
			dup2(file_desc, 1); //changes the output to the file
			ifRedirected = 1;
		}
		else if (strcmp(outPipe[i], ">>") == 0) //if append
		{				
			file_desc = open(outPipe[i+1], O_WRONLY | O_APPEND | O_CREAT); //opens the file
			if(file_desc < 0) //error checking
			{
 				printf("Error opening the file\n");
			}
			dup2(file_desc, STDOUT_FILENO); //changes the output to the file
			ifRedirected = 1;
		}
	}

	// DONE WITH CMD SEGMENTATION

	int fd[2];

	if (pipe(fd) < 0) {

		printf("Command Error\n");
		free(inPipe);
		free(outPipe);
		return -1;
	}

	pid_t pipePID = fork();

	if (pipePID < 0) {

		perror("Command Error");
		free(inPipe);
		free(outPipe);
		return -1;

	}
	else if (pipePID > 0) { // parent

		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);

		pipeSelect[0] = outPipe;
		start = Index + 1;
		end = cmdCounter;

	}
	else { //child INPUT

		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);

		pipeSelect[0] = inPipe;
		start = 0;
		end = Index;

	}

	// OUTSIDE OF FORK CONDITIONS

	pid_t c2PID;
	
	if (strcmp(pipeSelect[0][0], "cd") == 0)
	{
		if ((end - start) != 2)
		{
			printf("Command error\n");

		}
		else if (chdir(pipeSelect[0][1]) < 0)
		{
			perror("[4061-shell]: ");
		}
	}
	else if ((strcmp(pipeSelect[0][0], "ls") == 0) || (strcmp(pipeSelect[0][0], "wc") == 0))
	{
		if(pipePID > 0) //SUPER PARENT
		{
			c2PID = fork();
			if (c2PID < 0) //error
			{
			printf("Command error\n");
			}	
		}
		if (pipePID == 0 || c2PID == 0) //child
		{
			char* ABS_PATH_BUF = (char*)malloc(1000);
			sprintf(ABS_PATH_BUF, "%s/%s", TEMPLATE_DIR, pipeSelect[0][0]);
			pipeSelect[0][0] = ABS_PATH_BUF;

			int j = 0;
			while (j < (end - start) && (strcmp(pipeSelect[0][j], ">") != 0) && (strcmp(pipeSelect[0][j], ">>") != 0))
			{
				j++;
			}

			char** args = (char**)malloc(sizeof(char**) * j + 1);

			int i;
			for (i = 0; i < j; i++)
			{
				args[i] = (char*)malloc(100);
				printf("%d: %s\n", i, pipeSelect[0][i]);
				strcpy(args[i], pipeSelect[0][i]);
			}
			args[j] = (char*)NULL;

			execv(args[0], args);
			printf("FAIL\n");
			free(ABS_PATH_BUF);
			free(args);
			retVal = -1;
		}
		else //parent
		{
			//wait for lsPID, error checking
			if (waitpid(pipePID, NULL, 0) < 0)
			{
				printf("Command error\n");
				exit(0);
			}
			//wait for lsPID, error checking
			if (waitpid(c2PID, NULL, 0) < 0)
			{
				printf("Command error\n");
				exit(0);
			}
		}
	}
	else
	{

		if(pipePID > 0) //SUPER PARENT
		{
			c2PID = fork();
			if (c2PID < 0) //error
			{
			printf("Command error\n");
			}	
		}
		if (c2PID == 0 || pipePID == 0) //child
		{
			int j = 0;
			while (j < (end - start) && (strcmp(pipeSelect[0][j], ">") != 0) && (strcmp(pipeSelect[0][j], ">>") != 0))
			{
				j++;
			}

			char** args = (char**)malloc(sizeof(char**) * j + 1);

			int i;
			for (i = 0; i < j; i++)
			{
				args[i] = (char*)malloc(100);
				strcpy(args[i], pipeSelect[0][i]);
			}
			args[j] = (char*)NULL;

			execvp(args[0], args);
			printf("FAIL\n");
			free(args);
			retVal = -1;
			//execvp(commandSplit[0],commandSplit);
		}
		else //parent
		{
			if (waitpid(pipePID, NULL, 0) < 0)
			{
				printf("Command error\n");
				exit(0);
			}
			//wait for miscPID, error checking
			if (waitpid(c2PID, NULL, 0) < 0)
			{
				printf("Command error\n");
				exit(0);
			}
		}
	}
	
	free(inPipe);
	free(outPipe);
	free(pipeSelect);
	close(fd[0]);
	close(fd[1]);

	if(ifRedirected == 1)
	{	
		close(file_desc);
		dup2(terminal,1);
		dup2(inputFD,0);
		ifRedirected = 0;
	}

	return retVal;
}


int main(){
	
	char * currentDirectory;						//stores the cwd path
	char * TEMPLATE_DIR;
	char SHELL_TAG[] = "[project-4061]";			//tag for printing out the cwd
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
			//printf("%d: %s\n",counter, commandSplit[counter]);
        		counter = counter + 1;
        		commandSplit[counter] = strtok(NULL, " \n\r");
   			}
		
			int pipeIndex;
	
		for(int i = 0; i < counter && isPiping == 0; i++) //checking for file redirection
		{
			
			if (strcmp(commandSplit[i], "|") == 0) {

				isPiping = 1;
				pipeIndex = i;

				if (thePipeProcess(commandSplit, pipeIndex, counter, TEMPLATE_DIR) < 0) {

					printf("Command Error\n");
					free(currentDirectory);
					free(buf);
					free(TEMPLATE_DIR);
					return 0;

				};

				printf("ended Pipe\n");

				//return 0; // FOR DEV PURPOSES

			}
			else if(strcmp(commandSplit[i], ">") == 0) //if no append
			{
				fclose(fopen(commandSplit[i+1], "w")); //clears the output file
				file_desc = open(commandSplit[i+1], O_WRONLY | O_CREAT); //opens the file
				if(file_desc < 0) //error checking
				{
       	 				printf("Error opening the file\n");
				}
				dup2(file_desc, 1); //changes the output to the file
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
		if (isPiping == 0) 
		{
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
		
		}
		else {
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