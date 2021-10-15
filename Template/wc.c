#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include<string.h>
#include <fcntl.h> // open?

void wc(int mode, char* path){
	
	// MODE 0 = only file path provided
	// MODE 1 = count lines
	// MODE 2 = count words
	// MODE 3 = count characters

	int fd[2];

	int lineCount = 0;
	int wordCount = 0;
	int characterCount = 0;

	char buf[1];

	char prev[1];

	if (path == NULL) {

		printf("This means we must use STDIN for input\n");
		
		// set path equal to the piped input from STDIN

		if (dup2(fd[0], STDIN_FILENO) < 0) {

			printf("ERROR: Failed dup2...");

		}

	}
	else {

		fd[0] = open(path, O_RDONLY); // NEED ERROR HANDLE

	}

	if (mode == 0) {

		// printf("mode 0\n");

		if (read(fd[0], buf, sizeof(buf)) != 0) {

			lineCount++;

			if ( (buf[0] != ' ') && (buf[0] != '\n') && (buf[0] != '\r') ) {

				wordCount++;

			}

			if (buf[0] != 10 && buf[0] != 13 && buf[0] != 0) {

				characterCount++;

			}

			prev[0] = buf[0];

			while (read(fd[0], buf, sizeof(buf))) {

				if (buf[0] == '\n') {

					lineCount++;

				}

				if ((prev[0] != ' ' && buf[0] == ' ') || buf[0] == '\n' || buf[0] == '\t') {

					wordCount++;

				}

				if (buf[0] != 10 && buf[0] != 13 && buf[0] != 0) {

					characterCount++;

				}

				prev[0] = buf[0];

			}

		}

		printf("\t%d\t%d\t%d\n", lineCount, wordCount, characterCount);

		// display all three col

	}
	else if (mode == 1) {

		// printf("1\n");

		if (read(fd[0], buf, sizeof(buf)) != 0) {

			lineCount = 1;

			while (read(fd[0], buf, sizeof(buf))) {

				if (buf[0] == '\n') {

					lineCount++;

				}

			}

		}

		printf("\t%d\n", lineCount);

		// lines

	}
	else if (mode == 2) {

		// printf("2\n");

		while (read(fd[0], buf, sizeof(buf))) {

			if (buf[0] == ' ' || buf[0] == '\n' || buf[0] == '\t') {

				wordCount++;

			}

		}

		printf("\t%d\n", wordCount);

		// words

	}
	else if (mode == 3) {

		// printf("3\n");

		while (read(fd[0], buf, sizeof(buf))) {

			//printf("the buffer contains[%d]\n", buf[0]);

			if (buf[0] != 10 && buf[0] != 13 && buf[0] != 0) {

				characterCount++;

			}

		}

		printf("\t%d\n", characterCount);

	}

}

int main(int argc, char** argv){
	if(argc>2){
		if(strcmp(argv[1], "-l") == 0) { 
			wc(1, argv[2]);
		} else if(strcmp(argv[1], "-w") == 0) { 
			wc(2, argv[2]);
		} else if(strcmp(argv[1], "-c") == 0) { 
			wc(3, argv[2]);
		} else {
			printf("Invalid arguments\n");
		}
	} else if (argc==2){ // BEING USED WITH PIPES
	 	if(strcmp(argv[1], "-l") == 0) { 
			wc(1, NULL);
		} else if(strcmp(argv[1], "-w") == 0) { 
			wc(2, NULL);
		} else if(strcmp(argv[1], "-c") == 0) { 
			wc(3, NULL);
		} else {
    		wc(0, argv[1]); // WHEN PASSSED WITH ONLY FILE NAME
    	}
  	} else {
  		wc(0,NULL);
  	}

	return 0;
}