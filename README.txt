/*test machine: CSELAB_machine_name
* date: 10/23/21
* name: Michael Johnson, Chase Anderson, Emir Sahbegovic
* x500: Joh18255, And08479, Sahbe001
*/


Purpose of the Program:

	This program has the purpose of implementing a shell for use of multiple commands. Extra features include the use of output rediections into files,
	as well as using pipes to take the output of one program and place it as the input to another program. The feature of piping and redirection in the same
	command is also implemented. This shell will also work to implement system bin commands (e.g. pwd).

How to Compile the Program:

	Once one has correctly navigated to the source file, run the command "make" in a linux system,
	this will utilize gcc to compile and generate an output file.

What Our Program Does:

	Our program will take text input and run the shell commands required of it. Given a command was faulty or a file not provided, the system will provide the
	output error message "Command Error". Aligned with the given purpose, this program will output to the systems standard output or a given file based on the
	provided input. Three embeded commands (cd, ls, wc) not including exit work to change your working directory, list the files in the current 
	working directory, and list input text information respectively.

External Assumptions:

	1. Command input will follow suit of typical linux shell command inputs.

	2. Only a single pipe will be used in command execution.

	3. Command argument count will not exceed 50 entries.

	4. Command arguments themselves will not exceed 100 characters.

	5. As mentioned with TA Wenlong Wang, the character resulting from a printf statement was necessary to prevent a bug
	   that could not be solved by any groupmate.

	6. Files do not need to be created prior to redirecting output, the shell will automatically handle such requests.

Our Team:

	Michael Johnson - Joh18255
	Chase Anderson - And08479
	Emir Sahbegovic - Sahbe001

Our Contributions: 
	
	Michael - ls.c / redirection / commenting
	Chase - wc.c / piping / README
	Emir - cd.c / shell / exit