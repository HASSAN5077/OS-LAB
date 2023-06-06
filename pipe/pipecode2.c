
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    pid_t pid;
    int fd[2];
    int file = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (pipe(fd) == -1)
    {
        perror("Unable to create pipe");
        return 1;
    }
    if ((pid = fork()) == -1)
    {
        perror("Unable to create child");
        return 1;
    }
    if (pid == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execl("/usr/bin/ls", "ls", NULL);
    }
    dup2(fd[0], STDIN_FILENO);
    dup2(file,STDOUT_FILENO);
    close(fd[1]);
    close(fd[0]);
    execl("/usr/bin/grep", "grep", "x", NULL);
    return 0;
}