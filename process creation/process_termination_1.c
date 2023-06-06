#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t pid;
  int status;

  pid = fork(); /*fork a child process*/
  if (pid < 0) { /*error occurred*/
    	fprintf(stderr, "Fork failed");
    	return 1;
  } 
  else if (pid == 0) { /*child process*/
    	execlp("/bin/ls", "ls", NULL);
  } 
  else { /*parent process*/
    	wait(&status); /*parent will wait for the child to complete*/
  if (WIFEXITED(status)) {	/*check if child exited normally*/
      	printf("Child process terminated with exit status: %d\n", WEXITSTATUS(status));
    } 
  else {
      printf("Child process terminated abnormally\n");
    }
  }
  return 0;
}

