/*
#include<stdio.h>
#include<string.h>
int main(){
FILE *rd;
char buffer[45];
sprintf(buffer,"Hello world");
rd=popen("wc -c","w");
fwrite(buffer,sizeof(char),strlen(buffer),rd);
pclose(rd);
}
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main()
{
    FILE *rd;
    char buffer[45];
    rd = popen("ls", "r");
    fread(buffer, sizeof(char), 20, rd);
    write(1, buffer, 20);
    printf("\n");
    pclose(rd);
}
