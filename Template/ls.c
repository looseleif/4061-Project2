#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <stdbool.h>

void ls(char *path, bool recurse_flag) {
	/* Insert code here*/
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
