#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    short so;
    int sockfd, portno, n, term_con = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if(argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);

    if(server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server -> h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = portno;

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Error connecting");

    //while(1)
    //{
        so = 256;
        printf("THe value of short is: %d\n",so);
        so = htons(so);
        printf("Client send: %d\n",so);
        n = write(sockfd, &so, sizeof(so));
        if(n < 0)
            error("ERROR writing to socket");
        n = read(sockfd, &so, sizeof(so));
        if (n < 0)
            error("ERROR writing to socket");
        printf("I got %d from server\n", so);
    //}
   
}