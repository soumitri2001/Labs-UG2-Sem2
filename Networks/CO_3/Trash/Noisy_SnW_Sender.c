#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
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
    f->seq = next;
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
    printf("Sending seq: %d\n", fr->seq);
    // printf("Sending info: %s\n", fr->info.data);
    write(sockfd, buff, sizeof(buff));
}

void receiveFrame(frame *f)
{
    printf("Received acknowledgement...\n");
    frame *fr = (frame *)buff;
    f->ack = fr->ack;
    f->info = fr->info;
    f->kind = fr->kind;
    f->seq = fr->seq;
}

int randint()
{
    srand(time(0));
    return rand() % 10 + 1;
}

void extractData(frame *f, packet *p)
{
    *p = f->info;
}

void deliverData(packet *p)
{
    printf("Received data : %s\n", p->data);
}

void sender(int sockfd)
{
    seq_nr next_frame_to_send = 0;
    frame s, r;
    packet buffer;
    event_type event;

    getData(&buffer);
    // strcpy(buffer.data, "hello world");
    // printf("%s\n", buffer.data);
    int i = 0;
    while (i < 3)
    {
        makeFrame(&s, buffer, next_frame_to_send);
        // printf("%ld\n", strlen(s.info.data));
        int x = 1;
        if (x <= 3)
        {
            memset(buff, 0, sizeof(buff));
            write(sockfd, buff, sizeof(buff));
            printf("Sending frame...\n");
        }
        else
        {
            sendFrame(sockfd, &s);
        }
        waitForEvent(sockfd, &event);
        if (event == frame_arrival)
        {
            receiveFrame(&s);
            printf("%d\n", s.ack);

            if (s.ack == next_frame_to_send)
            {
                i++;
                getData(&buffer);
                next_frame_to_send = (next_frame_to_send + 1) % 2;
            }
            else
                printf("Acknowledgement bit did not match...%d %d\n", s.ack, next_frame_to_send);
        }
        else
        {
            printf("Timed out on acknowledgement...resending...\n");
        }
        sleep(5);
    }
}

int main()
{
    int sock = 0, valread;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    sender(sock);
    close(sock);
    return 0;
}