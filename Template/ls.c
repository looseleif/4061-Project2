#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <stdbool.h>
#include <dirent.h>



//int currentDirStorInt = 0;


void ls(char *path, bool recurse_flag) {
	
	char *recursiveDirectoryStorage[1000][1000];
	
	DIR *mydir; //DIR is a type that represents the directory
    	struct dirent *directory; //A variable that represents the directory as a whole

	int excludePeriods = 0;

	int currentDirStorInt = 0;
	
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
    		mydir = opendir(path); //This sets mydir equal to a pointer to the specified path. "." works better for some reason

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
		//for some reason putting a printf statement before this mydir line will cause a seg fault
		//so don't do it

		

		mydir = opendir(path); //This sets mydir equal to a pointer to the specified path. "." works better for some reason
		
		printf("Path is set to: %s\n", path);

		if (mydir == NULL) {

			printf("End of Directory Train \n");
			
		}
		else {

			//printf("PATH: \n"); //Formatting


			//readdir returns a pointer to the current position in the directory
			while ((directory = readdir(mydir)) != NULL) //readdir returns a null at the end of the directory
			{
				if (excludePeriods < 2)
				{
					excludePeriods++;
				}
				else
				{
					if (directory->d_type == DT_DIR) //This checks if it is a folder
					{
						sprintf(mydir, directory->d_name);
						recursiveDirectoryStorage[currentDirStorInt] = mydir;
						//printf("HAHAH: %s\n", recursiveDirectoryStorage);
						
						currentDirStorInt++;
						//ls(directory -> d_name, true);
					}
					printf(" %s ", directory->d_name); //d_name is an array that holds all the names of the directory


				}
			}

			printf("\n");

			for (int i = 0; i < currentDirStorInt; i++)
			{
				//printf("%s: \n", recursiveDirectoryStorage);
				ls(path, true);

			}

		}
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
