/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-2 Assignment 1 - STREAM SOCKETS - SERVER
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc TCPServer.c -o TCPServer
 * EXECUTION: ./TCPServer
 ***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_LEN 1024

const int PORT = 9999;
const int MAX_CLIENTS = 5;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    char buffer[MAX_LEN];
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("Server socket cannot be created\n");

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
        printf("[Waiting for client's message]\n");

        bzero(buffer, MAX_LEN);

        int flag = read(newsockfd, buffer, MAX_LEN);
        if (flag < 0)
            error("Read error\n");

        printf("Client: %s\n", buffer);

        if (strncmp("EXIT", buffer, 4) == 0)
            break;

        bzero(buffer, MAX_LEN);
        printf("Server (enter 'EXIT' to exit): ");
        fgets(buffer, MAX_LEN, stdin);

        flag = write(newsockfd, buffer, strlen(buffer));
        if (flag < 0)
            error("Write error\n");

        if (strncmp("EXIT", buffer, 4) == 0)
            break;
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}