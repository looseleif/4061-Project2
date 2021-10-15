#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <stdbool.h>
#include <dirent.h>



//int currentDirStorInt = 0;


void ls(char *path, bool recurse_flag) 
{
	char path1[2000];
   	struct dirent *dp;
   	DIR *dfd;
 	printf("1\n");
  	if ((dfd = opendir(path)) == NULL) 
	{
      		printf("1\n");
		fprintf(stderr, "lsrec: can't open %s\n", path);
		printf("1\n");
      		return;
   	}
 	printf("2\n");
   	while ((dp = readdir(dfd)) != NULL) 
	{
      		printf("3\n");
		if (dp->d_type == DT_DIR)
		{
         		printf("4\n");
			path1[0] = '\0';
         		if (strcmp(dp->d_name, ".") == 0 || 
             		strcmp(dp->d_name, "..") == 0)
            			continue;
         		sprintf(path1, "%s/%s", path, dp->d_name);
         		ls(path1, true);
			printf("5\n");
      		}
      		else
         		printf("%s/%s\n", path, dp->d_name);
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
