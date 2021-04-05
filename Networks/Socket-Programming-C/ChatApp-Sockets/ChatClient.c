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
    exit(1);
}

const int PORT = 9999;
const char localhost[10] = "127.0.0.1";

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Client socket cannot be created\n");
    server = gethostbyname(localhost);
    if (server == NULL)
    {
        fprintf(stderr, "No such host found\n");
        exit(0);
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(PORT);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Client connection error\n");

    printf("Client listening on PORT %d\n\n", PORT);

    while (1)
    {
        bzero(buffer, 256);
        printf("Client: ");
        fgets(buffer, 255, stdin);
        int flag = write(sockfd, buffer, strlen(buffer));
        if (flag < 0)
            error("Write error\n");
        bzero(buffer, 256);
        flag = read(sockfd, buffer, 255);
        if (flag < 0)
            error("Read error\n");
        printf("Server: %s\n", buffer);
        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }
    close(sockfd);
    return 0;
}