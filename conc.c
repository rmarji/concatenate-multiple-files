/*
~~~~~~~~~!!!IMPORTANT!!!~~~~~~~~~~~~
Please Read the Read_me File First
~~~~~~~~~!!!IMPORTANT!!!~~~~~~~~~~~~
*/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[])
{
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if (strcmp(argv[1], "--help") == 0)
		usageErr("%s old-file new-file\n", argv[0]);

	int i=0;
	for (i = 2 ; i < argc; i++) // this for loop will go through the input files and copy there contents into argv[1]
	{
		if (argv[i] == argv[1]) // this checks if the input file is different from the out
			errExit("one of the input file is the same as the output");
			
		/* Open input and output files */
		inputFd = open(argv[i], O_RDONLY); // this opens the input file specified in the termenal.
	
		if (inputFd == -1)
			errExit("opening file %s", argv[1]);
		
		openFlags = O_CREAT | O_WRONLY | O_APPEND; // added append flag so the buffer data is added not overwritten

		filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; /* rw-rw-rw- */

		outputFd = open(argv[1], openFlags, filePerms);
		
		while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
			if (write(outputFd, buf, numRead) != numRead)
				fatal("couldn't write whole buffer");

		if (numRead == -1)
			errExit("read");

		if (close(inputFd) == -1)
			errExit("close input");

		if (close(outputFd) == -1)
			errExit("close output");

		
	}
	
	exit(EXIT_SUCCESS);


}
