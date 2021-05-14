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
    seq_nr seq;
    seq_nr ack;
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
    printf("%s\n", buff);
    if (strlen(buff) == 0)
    {
        *event = time_out;
    }
    else
        *event = frame_arrival;
}

void getData(packet *p)
{
    char num[50];
    strcpy(num, "This is packet #");
    sprintf(num + 16, "%d", i);
    strcpy(p->data, num);

    i++;
}

void makeFrame(frame *f, packet p, seq_nr next)
{
    f->info = p;
    f->ack = next;
    printf("### %d ###\n", f->ack);
    //f->kind = ack;
}

void sendFrame(int sockfd, frame *f)
{
    memset(buff, 0, sizeof(buff));
    frame *fr = (frame *)buff;
    fr->kind = f->kind;
    fr->seq = f->seq;
    fr->ack = f->ack;
    fr->info = f->info;
    //printf("%s\n",fr->info.data);
    printf("Sending frame...\n");
    write(sockfd, buff, sizeof(buff));
}

void receiveFrame(frame *f)
{
    printf("Receiving frame...\n");
    frame *fr = (frame *)buff;
    f->ack = fr->ack;
    f->info = fr->info;
    f->kind = fr->kind;
    f->seq = fr->seq;
    // printf("Received seq: %d\n", f->seq);
    // printf("Received info: %s\n", f->info.data);
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
    seq_nr frame_expected = 0;
    frame r, s;
    event_type event;

    int i = 0;
    while (i < 3)
    {
        waitForEvent(sockfd, &event);
        if (event == frame_arrival)
        {
            receiveFrame(&r);
            if (r.seq == frame_expected)
            {
                i++;
                deliverData(&r.info);
                frame_expected = (frame_expected + 1) % 2;
                printf("now frame_expected = %d\n", frame_expected);
            }
            else
                printf("Bits didnt match: %d %d\n", frame_expected, r.seq);
            packet dummy;
            //printf("### %d ###\n", (seq_nr)(1-frame_expected));
            makeFrame(&s, dummy, 1 - frame_expected);
            sendFrame(sockfd, &s);
        }
        else
        {
            printf("Frame was not received...\n");
            memset(buff, 0, sizeof(buff));
            write(sockfd, buff, sizeof(buff));
        }
        sleep(5);
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
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("Accept failed.");
        exit(EXIT_FAILURE);
    }
    receiver(new_socket);
    close(server_fd);
    return 0;
}