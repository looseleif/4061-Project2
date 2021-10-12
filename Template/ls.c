#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <stdbool.h>
#include <dirent.h>

void ls(char *path, bool recurse_flag) {
	DIR *mydir; //DIR is a type that represents the directory
    	struct dirent *directory; //A variable that represents the directory as a whole

	printf("before %s\n", path);

	if(path != NULL) { 
		printf("inside \n");
    		mydir = opendir(path); //This sets mydir equal to a pointer to the specified path

		//readdir returns a pointer to the current position in the directory
    		while((directory = readdir(mydir)) != NULL) //readdir returns a null at the end of the directory
    		{	
			printf("While loop \n");
        		printf(" %s\n", directory->d_name); //d_name is an array that holds all the names of the directory
    		}
		
		printf("done \n");
    		closedir(mydir);
	}
	else if(path = NULL) {
		printf("path is NULL \n");
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