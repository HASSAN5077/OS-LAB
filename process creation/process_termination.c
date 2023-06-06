#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
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
	perror("execlp");  /*print error message if execlp fails*/
    	exit(1);  /*exit with non-zero status to indicate an error*/
}

else{	/*parent process*/
	int status;
    	wait(&status);  /*parent will wait for the child to complete and get the exit status*/
    	if (WIFEXITED(status)) {    /*check if child exited normally*/
        	printf("child completed with exit status %d\n", WEXITSTATUS(status));
    	} 
    	else if (WIFSIGNALED(status)) {    /*check if child terminated due to a signal*/
        	printf("child terminated by signal %d\n", WTERMSIG(status));
    }
}
return 0;
}
