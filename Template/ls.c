#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <stdbool.h>
#include <dirent.h>

void ls(char *path, bool recurse_flag) {
	DIR *mydir; //DIR is a type that represents the directory
    	struct dirent *directory; //A variable that represents the directory as a whole

	int excludePeriods = 0;
	
	if(path == NULL)
	{
		size_t Max_Path_Size = 100; //Just a random number I picked
		char curDir[Max_Path_Size]; //array that will represent our current directory
		
		if(getcwd(curDir, Max_Path_Size) == NULL) //This function returns NULL for an error
		{
			printf("CWD Error %s\n", curDir);
		}
		else //This code is copy and paste from above, may reformat to make more efficient
		{
			path = curDir; 
		}
	}
	
	

	if(recurse_flag == false) 
	{ 
    		mydir = opendir(path); //This sets mydir equal to a pointer to the specified path

		//readdir returns a pointer to the current position in the directory
    		while((directory = readdir(mydir)) != NULL) //readdir returns a null at the end of the directory
    		{	
			if(excludePeriods < 2) 
			{
				excludePeriods++;
			}
			else
			{
        			printf(" %s ", directory->d_name); //d_name is an array that holds all the names of the directory
			}
    		}
		
		printf("\n"); //Formatting
		
    		closedir(mydir);
	}
	else if(recurse_flag == true)
	{
		int isFolder = 0;
		
		mydir = opendir(path); //This sets mydir equal to a pointer to the specified path

		//readdir returns a pointer to the current position in the directory
    		while((directory = readdir(mydir)) != NULL) //readdir returns a null at the end of the directory
    		{	
			if(excludePeriods < 2) 
			{
				excludePeriods++;
			}
			else
			{	
				if(directory -> d_type == DT_NIR) //This checks if it is a folder
				{
					isFolder++;
					printf(directory + "\n");
					printf(mydir + "\n");
					
				}
        			printf(" %s ", directory->d_name); //d_name is an array that holds all the names of the directory
			}
    		}
		
		printf("\n"); //Formatting
		
    		closedir(mydir);
		
		
		
	}
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
