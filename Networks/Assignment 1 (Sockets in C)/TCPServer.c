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
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    char *msg = "Hello from Server !";
    char buffer[256];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Server socket cannot be created\n");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Server socket Bind error\n");

    listen(sockfd, MAX_CLIENTS);
    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &cli_len);
    if (newsockfd < 0)
        error("Server socket Accept error\n");

    printf("****Server listening on PORT %d****\n", PORT);

    int flag = read(newsockfd, buffer, 256);
    if (flag < 0)
        error("Read error\n");
    printf("Client: %s\n", buffer);
    printf("Server: ");
    flag = send(newsockfd, msg, strlen(msg), 0);
    if (flag < 0)
        error("Send error\n");

    close(newsockfd);
    close(sockfd);
    return 0;
}