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

    char *msg = "Hello from Client !";
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
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("Client connection error\n");

    printf("****Client listening on PORT %d****\n", PORT);

    int flag = send(sockfd, msg, strlen(msg), 0);
    if (flag < 0)
        error("Send error\n");
    flag = read(sockfd, buffer, 256);
    if (flag < 0)
        error("Read error\n");
    printf("Server: %s\n", buffer);

    close(sockfd);
    return 0;
}