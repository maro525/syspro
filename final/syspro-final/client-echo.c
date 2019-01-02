#include <stdio.h>
#include <sys/times.h>
#include <resolv.h>
#include <arpa/nameser.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFSIZE 1024

void echo_excute(int sock, char *buf)
{
    int n;

    while(1)
    {
        printf("> ");
        bzero(buf, BUFSIZE);
        fgets(buf, BUFSIZE, stdin);

        n = write(sock, buf, strlen(buf));
        
        if(n < 0)
            perror("write");

        bzero(buf, BUFSIZE);
        
        n = read(sock, buf, BUFSIZE);

        if(n < 0)
            perror("read");

        printf("%s", buf);
    }
}

int main(int argc, char **argv)
{
    int    sockfd, n;
    struct addrinfo hints, *res, *ressave;
    char   *remote;
    char   *port;
    char buf[BUFSIZE];

    if (argc != 3)
    {
        fprintf(stderr, "usage error\n");
        exit(1);
    }

    remote = argv[1];
    port   = argv[2];

    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if( (n = getaddrinfo(remote, port, &hints, &res)) != 0)
        perror("getaddrinfo");

    ressave = res;

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    if( sockfd < 0 )
        perror("socket");

    do {

        if( connect(sockfd, res->ai_addr, res->ai_addrlen) != 0)
            perror("connecting");

        echo_excute(sockfd, buf);

    } while ((res=res->ai_next) != NULL);

    freeaddrinfo(ressave);
    close(sockfd);
    exit(0);
}