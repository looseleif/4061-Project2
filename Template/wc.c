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

	int i = 0;
	int counter = 0;
	int maximum = 0;

	int entered = 0;
	int start = 1;

	char buf[1];

	int piped = 0;


	size_t Max_Path_Size = 1000; //Just a random number I picked
	char curDir[Max_Path_Size]; //array that will represent our current directory

	//printf("hello1");

	if (path == NULL) {

		piped = 1;

	}
	else {

		if (getcwd(curDir, Max_Path_Size) == NULL) //Error checking
		{
			printf("CWD Error %s\n", curDir);
			return;
		}

		if ((fd[0] = openat(AT_FDCWD, path, O_RDONLY)) < 0) {

			printf("Can't open %s\n", path);
			return;

		}; 

	}

	if (piped) {

		if (mode == 0) {

			if (read(0, buf, sizeof(buf)) != 0) {

				characterCount = characterCount + 1;

				if (buf[0] != '\t' || buf[0] != '\n' || buf[0] != ' ' || buf[0] != '\0') {

					if (entered && buf[0] != '\n' && buf[0] != '\r') {

						entered = 0;
						wordCount = wordCount + 1;

					}

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}
				}
				else {

					entered = 1;

				}
			}

			//printf("%s\n", buf);

			while (read(0, buf, sizeof(buf)) != 0) {

				//printf("[%s]\n", buf);

				characterCount = characterCount + 1;


				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (entered) {

						entered = 0;
						wordCount = wordCount + 1;

					}

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}

				}
				else {

					entered = 1;

				}

			}

			if (entered) {

				wordCount++;

			}

			// used to count max digits to use...

			int list[3] = { lineCount, wordCount, characterCount };

			int tempNum;

			for (i = 0; i < 3; i++) {


				tempNum = list[i];

				counter = 0;

				while (tempNum != 0) {

					tempNum = tempNum / 10;
					counter++;

				}

				if (counter > maximum) {

					maximum = counter;

				}

			}


			printf("%*d %*d %*d\n", maximum, lineCount, maximum, wordCount, maximum, characterCount);

			// display all three col

		}
		else if (mode == 1) {

			// printf("1\n");

			if (read(0, buf, sizeof(buf)) != 0) {

				if (buf[0] != ' ' || buf[0] != '\t' || buf[0] != '\n' || buf[0] != '\0') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}
				}

			}

			//printf("%s\n", buf);

			while (read(0, buf, sizeof(buf)) != 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}

				}

			}

			printf("%d\n", lineCount);

			// lines

		}
		else if (mode == 2) {

			if (read(0, buf, sizeof(buf)) != 0) {

				if (buf[0] != ' ' || buf[0] != '\t' || buf[0] != '\n' || buf[0] != '\0') {

					if (entered && buf[0] != '\n' && buf[0] != '\r') {

						entered = 0;
						wordCount = wordCount + 1;

					}

				}
				else {

					entered = 1;

				}
			}

			while (read(0, buf, sizeof(buf)) != 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (entered) {

						entered = 0;
						wordCount = wordCount + 1;

					}

				}
				else {

					entered = 1;

				}

			}

			if (entered) {

				wordCount++;

			}

			// used to count max digits to use...

			printf("%d\n", wordCount);

			// words

		}
		else if (mode == 3) {

			if (read(0, buf, sizeof(buf)) != 0) {

				characterCount = characterCount + 1;

			}


			while (read(0, buf, sizeof(buf)) != 0) {

				characterCount = characterCount + 1;

			}

			printf("%d\n", characterCount);

			// words

		}
	}
	else {

	if (mode == 0) {

			if (read(fd[0], buf, sizeof(buf)) != 0) {

				characterCount = characterCount + 1;

				if (buf[0] != '\t' || buf[0] != '\n' || buf[0] != ' ' || buf[0] != '\0') {

					if (entered && buf[0] != '\n' && buf[0] != '\r') {

						entered = 0;
						wordCount = wordCount + 1;

					}

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}
				}
				else {

					entered = 1;

				}
			}

			//printf("%s\n", buf);

			while (read(fd[0], buf, sizeof(buf)) != 0) {

				//printf("[%s]\n", buf);

				characterCount = characterCount + 1;


				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (entered) {

						entered = 0;
						wordCount = wordCount + 1;

					}

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}

				}
				else {

					entered = 1;

				}

			}

			if (entered) {

				wordCount++;

			}

			// used to count max digits to use...

			int list[3] = { lineCount, wordCount, characterCount };

			int tempNum;

			for (i = 0; i < 3; i++) {


				tempNum = list[i];

				counter = 0;

				while (tempNum != 0) {

					tempNum = tempNum / 10;
					counter++;

				}

				if (counter > maximum) {

					maximum = counter;

				}

			}


			printf("%*d %*d %*d %s\n", maximum, lineCount, maximum, wordCount, maximum, characterCount, path);

			// display all three col

		}
		else if (mode == 1) {

			// printf("1\n");

			if (read(fd[0], buf, sizeof(buf)) != 0) {

				if (buf[0] != ' ' || buf[0] != '\t' || buf[0] != '\n' || buf[0] != '\0') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}
				}

			}

			//printf("%s\n", buf);

			while (read(fd[0], buf, sizeof(buf)) != 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}

				}

			}

			printf("%d %s\n", lineCount, path);

			// lines

		}
		else if (mode == 2) {

			if (read(fd[0], buf, sizeof(buf)) != 0) {

				if (buf[0] != ' ' || buf[0] != '\t' || buf[0] != '\n' || buf[0] != '\0') {

					if (entered && buf[0] != '\n' && buf[0] != '\r') {

						entered = 0;
						wordCount = wordCount + 1;

					}

				}
				else {

					entered = 1;

				}
			}

			while (read(fd[0], buf, sizeof(buf)) != 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (entered) {

						entered = 0;
						wordCount = wordCount + 1;

					}

				}
				else {

					entered = 1;

				}

			}

			if (entered) {

				wordCount++;

			}

			// used to count max digits to use...

			printf("%d %s\n", wordCount, path);

			// words

		}
		else if (mode == 3) {

			if (read(fd[0], buf, sizeof(buf)) != 0) {

				characterCount = characterCount + 1;

			}


			while (read(fd[0], buf, sizeof(buf)) != 0) {

				characterCount = characterCount + 1;

			}

			printf("%d %s\n", characterCount, path);

			// words

		}

	}

}

int main(int argc, char** argv){
	
	//printf("hello112");

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