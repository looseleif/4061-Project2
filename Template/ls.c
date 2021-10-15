#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <stdbool.h>
#include <dirent.h>



//int currentDirStorInt = 0;


void ls(char *path, bool recurse_flag) 
{
	char path[MAX_PATH];
   	struct dirent *dp;
   	DIR *dfd;
 
  	if ((dfd = opendir(dir)) == NULL) 
	{
      		fprintf(stderr, "lsrec: can't open %s\n", dir);
      		return;
   	}
 
   	while ((dp = readdir(dfd)) != NULL) 
	{
      		if (dp->d_type == DT_DIR)
		{
         		path[0] = '\0';
         		if (strcmp(dp->d_name, ".") == 0 || 
             		strcmp(dp->d_name, "..") == 0)
            			continue;
         		sprintf(path, "%s/%s", dir, dp->d_name);
         		lsrec(path);
      		}
      		else
         		printf("%s/%s\n", dir, dp->d_name);
      	}
   closedir(dfd);
	
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
