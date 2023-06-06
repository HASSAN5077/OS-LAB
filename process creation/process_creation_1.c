#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
int main()
{
pid_t pid;
pid=fork();  /*fork a child process*/
if(pid<0){	/*error occurred*/
	fprintf(stderr,"Fork failed");
	return 1;
}

else if(pid==0){	/*child process*/
	execlp("/bin/ls","ls",NULL);
}

else{	/*parent process*/
	/*wait(NULL);*/		/*parent will wait for the child to complete*/
	printf("child completed");
}
return 0;
}
