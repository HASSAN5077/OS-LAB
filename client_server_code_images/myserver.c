#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>
#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, newsockfd, portno;
    char c;
    socklen_t cleint;

    struct sockaddr_in serv_addr, cli_addr;
    int n;
    if(argc < 2){
        fprintf(stderr, "Error no port no provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error while opening socket..");
    }

bzero((char*) &serv_addr,sizeof(serv_addr));
    portno =atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr;
    serv_addr.sin_port = portno;
    if(bind(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0){
        error("Error while binding");
    }

    listen(sockfd,5);
    cleint = sizeof(cli_addr);

    while(1){
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client);
        if(newsockfd < 0){
            error("error while accepting connection");
        }

        while(--c != 'Q'){
            n = read(newsockfd, &c, 1);
            if(n < 0)
            error("ERROR reading from socket");
            printf("I got : %c\n", c);
            ++c;
            n = write(newsockfd, &c, 1);
            if(n < 0)
            error("ERROR writing to socket");
        }
        close(newsockfd);
    }
}