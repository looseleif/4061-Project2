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

	// using buf size of 1 allows us to simply scan through each and every character
	// present in the input data

	char buf[1];

	int piped = 0;

	int readReturn = 0;


	size_t Max_Path_Size = 1000; // used for maximum case entries, fault tolerance

	char curDir[Max_Path_Size]; // array that will represent our current directory

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

		}

	}

	// in both sub cases (pipe and nonpipe) if statement is used
	// to check for initial case where first character may or may not 
	// be a valid word character

	// then while loops are used to iterate throughout the rest of contents.
	// error checking included for read

	// comments will be explained in the nonpipe version not the pipe version
	// due to the similarity

	if (piped) {

		// inside of the pipe section, we are using STDIN_FILENO as our fd
		// this is to take in pipe access from what could otherwise be defined

		if (mode == 0) {

			readReturn = read(0, buf, sizeof(buf));

			if (readReturn > 0) {

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

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			while (readReturn > 0) {

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

				readReturn = read(0, buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			if (entered) {

				wordCount++;

			}

			// used to count max digits to use when displaying...

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

			readReturn = read(0, buf, sizeof(buf));

			if (readReturn > 0) {

				if (buf[0] != ' ' || buf[0] != '\t' || buf[0] != '\n' || buf[0] != '\0') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}
				}

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			readReturn = read(0, buf, sizeof(buf));

			while (readReturn > 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}

				}

				readReturn = read(0, buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			printf("%d\n", lineCount);

			// lines

		}
		else if (mode == 2) {

			readReturn = read(0, buf, sizeof(buf));

			if (readReturn > 0) {

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

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			readReturn = read(0, buf, sizeof(buf));

			while (readReturn > 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (entered) {

						entered = 0;
						wordCount = wordCount + 1;

					}

				}
				else {

					entered = 1;

				}

				readReturn = read(0, buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			if (entered) {

				wordCount++;

			}

			printf("%d\n", wordCount);

			// words

		}
		else if (mode == 3) {

			readReturn = read(0, buf, sizeof(buf));
			
			if (readReturn > 0) {

				characterCount = characterCount + 1;

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			readReturn = read(0, buf, sizeof(buf));

			while (readReturn > 0) {

				characterCount = characterCount + 1;

				readReturn = read(0, buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			printf("%d\n", characterCount);

			// character

		}
	}
	else {

	if (mode == 0) {

		// access read return number for use with processing and error checking
					
					// read will fill buf with a single character	
				
		readReturn = read(fd[0], buf, sizeof(buf));

			if (readReturn > 0) {

				// always increment character, given present for first character

				characterCount = characterCount + 1;

				// checking for new word contents given something is present as the first character

				if (buf[0] != '\t' || buf[0] != '\n' || buf[0] != ' ' || buf[0] != '\0') {

					if (entered && buf[0] != '\n' && buf[0] != '\r') {

						entered = 0;
						wordCount = wordCount + 1;

					}

					// checking for new line contents given something is present as the first character

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}
				}
				else {

					// this means that the buf has entered an actual 
					// character signaling the entrance of a word

					entered = 1;

				}
			}

			// error check for failed read

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			readReturn = read(fd[0], buf, sizeof(buf));

			// this loop will read through the file contents until a fail or nothing is read

			while (readReturn > 0) {

				// always increment characterCount

				characterCount = characterCount + 1;

				// special cases for checking valid word

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

					// same idea as before, given none of aforementioed character found, enter a word

					entered = 1;

				}

				readReturn = read(fd[0], buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

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

		// remaining modes employ similar tactics to what is found in mode 1

		else if (mode == 1) {

			readReturn = read(fd[0], buf, sizeof(buf));

			if (readReturn > 0) {

				if (buf[0] != ' ' || buf[0] != '\t' || buf[0] != '\n' || buf[0] != '\0') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}
				}

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			readReturn = read(fd[0], buf, sizeof(buf));

			while (readReturn > 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (buf[0] == '\0' || buf[0] == '\n') {

						lineCount = lineCount + 1;

					}

				}

				readReturn = read(fd[0], buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			printf("%d %s\n", lineCount, path);

			// lines

		}
		else if (mode == 2) {

			readReturn = read(fd[0], buf, sizeof(buf));
			
			if (readReturn > 0) {

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

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			readReturn = read(fd[0], buf, sizeof(buf));

			while (readReturn > 0) {

				if (buf[0] == ' ' || buf[0] == '\t' || buf[0] == '\n' || buf[0] == '\0' || buf[0] == '\r') {

					if (entered) {

						entered = 0;
						wordCount = wordCount + 1;

					}

				}
				else {

					entered = 1;

				}

				readReturn = read(fd[0], buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			if (entered) {

				wordCount++;

			}

			printf("%d %s\n", wordCount, path);

			// words

		}
		else if (mode == 3) {

			readReturn = read(fd[0], buf, sizeof(buf));
			
			if (readReturn > 0) {

				characterCount = characterCount + 1;

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			readReturn = read(fd[0], buf, sizeof(buf));

			while (readReturn > 0) {

				characterCount = characterCount + 1;

				readReturn = read(fd[0], buf, sizeof(buf));

			}

			if (readReturn < 0) {

				fprintf(stderr, "Command Error\n");
				return;

			}

			printf("%d %s\n", characterCount, path);

			// characters

		}

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