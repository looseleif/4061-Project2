/*test machine: CSELAB_machine_name
* date: 10/04/21
* name: Michael Johnson, Chase Anderson, Emir Sahbegovic
* x500: Joh18255, And08479, Sahbe001
*/


Purpose of the Program:

	This program is capable of executing multiple processes either in sequential DFS order, or 
	in parallel using a given input file denoting [Number of Command Nodes, Each Nodes Command, and 
	the Source-->Destination Edge Descriptors]

How to Compile the Program:

	Once one has correctly navigated to the source file, run the command "make" in a linux system,
	this will utilize gcc to compile and generate an output file.

What Our Program Does:

	Our program will output the given commands and the process's ID and process's parent ID to a seperate output file. These commands will 
	be sorted by the order determined by depth first search. Our program will even 
	print the process id next to each executed command in the output file. 

External Assumptions:

	1. Commands will follow system call file path and file label standards. (e.g. /bin/ls -l)

	2. User will explicitly use "-p" as the parallel flag, this can be before 
	   or after the input text filename. 

	3. The input text file path will be explicity defined from its nearest common directory.

		(e.g. ./depGraph -p ../Testcases/in.txt OR ./depGraph ../Testcases/in.txt -p)

Our Team:

	Michael Johnson - Joh18255
	Chase Anderson - And08479
	Emir Sahbegovic - Sahbe001

Our Contributions: 
	
	Michael - DFS Algorithm & README
	Chase - Input File Decomposition & Process Graph
	Emir - File Generation, Error Handling, AddEdge