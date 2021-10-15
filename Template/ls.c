#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <stdbool.h>
#include <dirent.h>



//int currentDirStorInt = 0;


void ls(char *path, bool recurse_flag) 
{
	char recursiveDirectoryStorage[2000];
   	struct dirent *directoryPointer;
   	DIR *mydir;
	
	int excludePeriods = 0;

	if(path == NULL) //gets the cwd and sets it equal to path
	{
		size_t Max_Path_Size = 1000; //Just a random number I picked
		char curDir[Max_Path_Size]; //array that will represent our current directory
		
		if(getcwd(curDir, Max_Path_Size) == NULL) //This function returns NULL for an error
		{
			printf("CWD Error %s\n", curDir);
			return;
		}
		else 
		{
			path = curDir; 
		}
	}

  	if ((mydir = opendir(path)) == NULL) //error checking to see if the path is valid
	{
		printf("Can't open %s\n", path);
      		return;
   	}
	
	printf("In directory: %s\n", path); //formatting

   	while ((directoryPointer = readdir(mydir)) != NULL) 
	{
		if (directoryPointer->d_type == DT_DIR && recurse_flag == true)
		{
         		if(excludePeriods < 2)
			{
            			excludePeriods++;
			}
			else
			{
         			sprintf(recursiveDirectoryStorage, "%s/%s", path, directoryPointer->d_name);
         			ls(recursiveDirectoryStorage, true);
				printf("Back in directory: %s\n", path);
			}

      		}
      		else
		{
         		printf("%s\n", directoryPointer->d_name);
		}
      	}

   closedir(mydir);
	
}

int main(int argc, char *argv[]){
	if(argc < 2){ // No -R flag and no path name
		ls(NULL, false);
	} else if(strcmp(argv[1], "-R") == 0) { 
		if(argc == 2) { // only -R flag
			ls(NULL, true);
		} else { // -R flag with some path name
			ls(argv[2], true);
		}
	}else { // no -R flag but path name is given
    	ls(argv[1], false);
  }
	return 0;
}
