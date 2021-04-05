#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

const int PORT = 9999;
const char localhost[10] = "127.0.0.1";
const int MAX_CLIENTS = 5;

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Server socket cannot be created\n");
    // bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Server socket Bind error\n");
    listen(sockfd, MAX_CLIENTS);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0)
        error("Server socket Accept error\n");

    printf("Server listening on PORT %d\n\n", PORT);

    while (1)
    {
        bzero(buffer, 256);
        int flag = read(newsockfd, buffer, 255);
        if (flag < 0)
            error("Read error\n");
        printf("Client: %s\n", buffer);
        bzero(buffer, 256);
        printf("Server: ");
        fgets(buffer, 255, stdin);
        flag = write(newsockfd, buffer, strlen(buffer));
        if (flag < 0)
            error("Write error\n");
        int i = strncmp("Bye", buffer, 3);
        if (i == 0)
            break;
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}