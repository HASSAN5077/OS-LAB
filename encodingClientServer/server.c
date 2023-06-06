#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t client;
    short so;

    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
   
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = portno;

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd, 5);
    client = sizeof(cli_addr);

    //while(1)
    //{
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &client);
        if(newsockfd < 0)
            error("ERROR on accept");
        //do
        //{
            n = read(newsockfd, &so, sizeof(so));
            printf("I got : %d\n", so);
            so = ntohs(so);
            if(n < 0)
            error("ERROR reading from socket");
            ++so;
            so = htons(so);
            printf("I send: %d\n", so);
            n = write(newsockfd, &so, sizeof(so));
            if(n < 0)
            error("ERROR writing to socket");
        //} while (--c != 'Q');

        close(newsockfd);
    //}
}