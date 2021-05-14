/****************************************************************************************
 * NAME: SOUMITRI CHATTOPADHYAY
 * ROLL: 001911001083
 * SUBJECT: COMPUTER NETWORKS LABORATORY (UG-2, SEM-2)
 * ASSIGNMENT: CO-3 Assignment 2(b) - Noisy Stop-n-Wait Protocol - Sender
 ****************************************************************************************/

/****************************************************************************************
 * COMPILATION: gcc noisystopnwaitSender.c -o noisystopnwaitSender
 * EXECUTION: ./noisystopnwaitSender
 ***************************************************************************************/

/* header files */
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
static int packetNumber = 1;

void waitForEvent(int sockfd, event_type *event)
{
    memset(buff, 0, sizeof(buff));
    read(sockfd, buff, 4096);
    if (strlen(buff) == 0)
        *event = time_out;
    else
        *event = frame_arrival;
}

void getData(packet *p)
{
    char num[50];
    strcpy(num, "This is packet #");
    sprintf(num + 16, "%d", packetNumber);
    strcpy(p->data, num);

    packetNumber++;
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

void extractData(frame *f, packet *p)
{
    *p = f->info;
}

void deliverData(packet *p)
{
    printf("Received data : %s\n", p->data);
}

int randint()
{
    srand(time(0));
    return rand() % 5 + 1;
}

void sender(int sockfd)
{
    frame s, ak;
    packet p;
    event_type event;

    for (int i = 0; i < 3; i++)
    {
        int x = randint(); // random number for noise

        if (x <= 3)
        {
            getData(&p);
            makeFrame(&s, p, packetNumber);
            sendFrame(sockfd, &s);
        }
        else
        {
            makeFrame(&s, p, packetNumber);
            sendFrame(sockfd, &s);
        }
        //sender waits for the acknowledgement!
        waitForEvent(sockfd, &event);
        if (event == frame_arrival)
            receiveFrame(&ak);
        else
        {
            packetNumber--;
            printf("Resending packet...\n");
            getData(&p);
            makeFrame(&s, p, packetNumber);
            sendFrame(sockfd, &s);
        }
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
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&serv_addr.sin_zero, 0, 8);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    sender(sock);
    close(sock);
    return 0;
}