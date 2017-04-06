#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MAX_LENGTH 513

void printError(){
	char message[30] = "An error has occured\n";
	write(STDERR_FILENO, message, strlen(message));
}

void runCd(char* path, int num){
	//printf("cd count %d", num);
	if(num ==1)
	{
		char *dir;
          	dir = getenv("HOME");
          	chdir(dir);
	   
	}
	else if(num==2)
	{
		int ret = chdir(path);
		//printf("\n%d\n%s\n",ret,path);
	}
	else
		printError();
	
}


int ifAmpersand(char* sen)
{
	char* l = NULL;
	l = strchr(sen, '&');
	if(l != NULL)
	{	
		*l = '\0';
		return 1;
	}
	else
		return 0;
}

int ifRed(char* sen)
{
	char* l = NULL;
	l = strchr(sen, '>');
	if(l != NULL)
	{	
		return 1;
	}
	else
		return 0;
}

int ifPython(char* sen)
{
	if(strstr(sen,".py")!= NULL)
		return 1;
	else
		return 0;
}

void runCommand(char* tokens[], int red,char* redpath, int backgnd, int pyth)
{
	if(!(strcmp(tokens[0], "cd"))){
		int count;
		//printf("In cd \n");
		if(tokens[1] != NULL)
			count = 2;
		
		else
			count = 1;
		
		runCd(tokens[1],count);
	}
			
	else if(!(strcmp(tokens[0], "pwd"))){
		//printf("In pwd\n");
		char *buffer = NULL;
		size_t sizeb = 0;
		char *thepath = getcwd(buffer, sizeb);
		sizeb = strlen(thepath);
		int r= write(STDOUT_FILENO, thepath, sizeb);
		printf("\n");
	}

	else if(!(strcmp(tokens[0], "wait"))){
		//printf("In wait\n");
		wait(NULL);
	}
	else if(!(strcmp(tokens[0], "exit"))){
		//printf("In exit\n");
		exit(EXIT_SUCCESS);
	}
	else{
		int rc = fork();
		// child process
	 	if(rc == 0) {
			//printf("%s","i am child\n");
			if(pyth ==1)
			{
				char* argv[3];
				argv[0] = strdup("python");
				argv[1] = strdup(tokens[0]);
				argv[2] = (char*)NULL;
				//printf("%s\n",argv[1]);
				execvp(argv[0],argv);	
			}
			else if(red==1)
			{
				close(STDOUT_FILENO);
				int l = open(redpath, O_CREAT|O_TRUNC|O_RDWR, S_IRWXU);
				//fprintf(stderr,"open = %d\n", l);
				//printf("hi ramla\n");
			}
			int a = 0;
			a = execvp(tokens[0], tokens);	
			if(a==-1)
				printError();

		 
		}

		// parent process
		else if (rc >0) {
			// parent
		 	
			if(!backgnd)
			{
				//printf("In wait\n");
				wait(NULL);
			}
			//printf("%s", "i am parent\n");

		}
	}
	
}

int main(int argc, char *argv[])
{
	if (argc == 1 ){
	
		while(1){
		//printf("mysh> ");
		char* b = strdup("mysh> ");
			//strcat(a,"\n");
		write(STDOUT_FILENO,b,strlen(b));		
		//get input
		char* in = malloc(MAX_LENGTH);
		char* tokens[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
		char delim[2];
		delim[0] = ' ';
		delim[1] = '\n';
		(void) fgets(in,MAX_LENGTH, stdin);
		//printf("%d\n", (int)strlen(in));
		int len = (int) strlen(in);		
		if(len != 1)
		{		
			int backgnd = ifAmpersand(in);
			int red = ifRed(in);
			int redplc = 0;
		
			char * redpath;
			tokens[0] = strtok(in,delim);
			int pyth = ifPython(tokens[0]);
			//printf("%d\n",pyth);
			int i = 0;
		
			//printf("amp = %d\nred = %d\n",backgnd,red);
			while(tokens[i]!=NULL)
		      	{
				i++;
				tokens[i] = strtok(NULL,delim);
			
		
			
		      	}
			int j;
			if(red == 1)
			{
				for(j = 0; j <i; j++)
				{ 
					if(!(strcmp(tokens[j],">")))
						redplc = j;
				}
	
				//printf("redplc = %d\n", redplc);

				redpath = strdup(tokens[redplc+1]);
				//printf("path: %s\n",redpath);
				tokens[redplc] = NULL;
				tokens[redplc +1] = NULL;
	
			}
	
		
			runCommand(tokens,red,redpath,backgnd,pyth);			
		}
	}
}

	else if(argc ==2)
	{
		//printf("%s", "batch mode\n");

		char *batchfile = malloc(MAX_LENGTH);
		char *inp = malloc(MAX_LENGTH);
		batchfile = strdup(argv[1]);
		int fdin = open(batchfile, O_RDONLY);
		if (fdin<0)
			printError();
		int bytes_read = read(fdin, inp, 512);
		char *btokens[50];
		btokens[0] = strtok(inp, "\n");
		int k = 0;
		while(btokens[k] != NULL) {
			k++;
			btokens[k] = strtok(NULL, "\n");
		}
		int l;
		for(l = 0; l<k; l++)
		{
			//printf("mysh> ");
			//char* b = strdup("mysh> ");
			//strcat(a,"\n");
			//write(STDOUT_FILENO,b,strlen(b));
			//printf("%s\n", btokens[l]);
			char* a = strdup(btokens[l]);
			strcat(a,"\n");
			write(STDOUT_FILENO,a,strlen(a));
			char* tokens[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
			char delim[1];
			delim[0] = ' ';

			int backgnd = ifAmpersand(btokens[l]);
			int red = ifRed(btokens[l]);
			int redplc = 0;
			char * redpath;
			int pyth;
			char* abc = strdup(btokens[l]);
			tokens[0] = strtok(abc,delim);
			pyth = ifPython(tokens[0]);
			int i = 0;
		
			//printf("amp = %d\nred = %d\n",backgnd,red);
			while(tokens[i]!=NULL)
		      	{
				i++;
				tokens[i] = strtok(NULL," ");
				//printf(" %s\n",tokens[i]);
		
			
		      	}
			int j;
			if(red == 1)
			{
				for(j = 0; j <i; j++)
				{ 
					if(!(strcmp(tokens[j],">")))
						redplc = j;
				}
	
				//printf("redplc = %d\n", redplc);

				redpath = strdup(tokens[redplc+1]);
				//printf("path: %s\n",redpath);
				tokens[redplc] = NULL;
				tokens[redplc +1] = NULL;
	
			}
	
		
			runCommand(tokens,red,redpath,backgnd,pyth);	
		}		

	}

	else
		printError();

	return 0;
} // main


