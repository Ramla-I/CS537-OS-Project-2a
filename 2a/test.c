#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

       #include <sys/stat.h>
       #include <fcntl.h>
int main(int argc, char *argv[])
{
	close(STDOUT_FILENO);
	open("AbC.txt", O_TRUNC|O_RDWR|O_CREAT,S_IRWXU);
	printf("AHHHHHHHHHHH!!!!!!!!!!!!!");
	return 0;
}

