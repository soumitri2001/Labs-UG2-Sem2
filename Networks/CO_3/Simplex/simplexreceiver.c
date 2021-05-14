/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-3 Assignment 1 - Simplex Protocol - Receiver
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc simplexreceiver.c -o simplexreceiver
 * EXECUTION: ./simplexreceiver
 ***************************************************************************************/

/* header files */
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#define MAX 1024

typedef unsigned int seq_nr;
typedef struct
{
    char data[MAX];
} packet;

typedef enum
{
    data,
    ack,
    nak
} frame_kind;

typedef struct
{
    frame_kind kind;
    // seq_nr seq;
    // seq_nr ack;
    packet info;
} frame;

typedef enum
{
    frame_arrival,
    time_out
} event_type;

typedef struct
{
    event_type type;
} event;

char buff[4096];
static int i = 1;

void waitForEvent(int sockfd, event_type *event)
{
    memset(buff, 0, sizeof(buff));
    read(sockfd, buff, 4096);
    if (strlen(buff) == 0)
    {
        *event = time_out;
    }
    else
        *event = frame_arrival;
}

void getData(packet *p)
{
    strcpy(p->data, "This is packet #");

    i++;
}

void makeFrame(frame *f, packet p)
{
    f->info = p;
}

void sendFrame(int sockfd, frame *f)
{
    printf("Sending frame...\n");
    write(sockfd, f, sizeof(f));
}

void receiveFrame(frame *f)
{
    printf("Receiving frame...\n");
    frame *fr = (frame *)buff;
    // f->ack = fr->ack;
    f->info = fr->info;
    f->kind = fr->kind;
    // f->seq = fr->seq;
}

void extractData(frame *f, packet *p)
{
    *p = f->info;
}

void deliverData(packet *p)
{
    printf("Received data : %s\n", p->data);
}

void receiver(int sockfd)
{
    frame r;
    packet p;
    event_type event;
    for (int i = 0; i < 3; i++)
    {
        sleep(3);
        waitForEvent(sockfd, &event);
        if (event == frame_arrival)
        {
            receiveFrame(&r);
            extractData(&r, &p);
            deliverData(&p);
        }
    }
}

int main()
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed.");
        exit(EXIT_FAILURE);
    }
    receiver(new_socket);
    close(server_fd);
    return 0;
}