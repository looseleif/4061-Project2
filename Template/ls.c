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
	int count = 0;
  	if ((mydir = opendir(path)) == NULL) 
	{
		fprintf(stderr, "lsrec: can't open %s\n", path);
      		return;
   	}
	
	printf("In directory: %s\n", path);
	
   	while ((directoryPointer = readdir(mydir)) != NULL) 
	{
		if (directoryPointer->d_type == DT_DIR)
		{
			count++;
			recursiveDirectoryStorage[0] = '\0';
         		if (strcmp(directoryPointer->d_name, ".") == 0 || 
             		strcmp(directoryPointer->d_name, "..") == 0)
            			continue;
         		sprintf(recursiveDirectoryStorage, "%s/%s", path, directoryPointer->d_name);
         		ls(recursiveDirectoryStorage, true);
			printf("Back in directory: %s\n", path);

      		}
      		else
         		printf("%s\n", directoryPointer->d_name);
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
