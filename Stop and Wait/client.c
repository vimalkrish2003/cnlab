#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#define PORT 8000

typedef struct packet
{
    char data[1024];
}Packet;

typedef struct frame
{
    int frameKind;
    int seqNo;
    int ackNo;
    Packet packet;
}Frame;

int main()
{
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    int frameId=0;
    Frame frameSend,frameReceived;
    int ackRecv=1;

    sockfd=socket(AF_INET,SOCK_DGRAM,0);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);

    while(1)
    {
        frameSend.frameKind=1;
        frameSend.seqNo=frameId;
        frameSend.ackNo=0;

        printf("Enter Data:");
        scanf("%s",&buffer);
        strcpy(frameSend.packet.data,buffer);
        sendto(sockfd,&frameSend,sizeof(Frame),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
        printf("Frame Send\n");

        int addrSize=sizeof(serverAddr);
        int frameRecvSize=recvfrom(sockfd,&frameReceived,sizeof(frameReceived),0,(struct sockaddr*)&serverAddr,&addrSize);
        if (frameRecvSize>0 && frameReceived.frameKind==0 && frameReceived.ackNo==frameId+1)
        {
            printf("Ack received\n");
            ackRecv=1;
        }
        else
        {
            printf("Ack not received\n");
            ackRecv=0;
        }
        frameId++;
    }
    close(sockfd);
    return 0;

}