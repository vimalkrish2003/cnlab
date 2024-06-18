#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8000

typedef struct packet
{
    char data[1024];
} Packet;

typedef struct frame
{
    int frameKind;
    int seqNo;
    int ackNo;
    Packet packet;
}Frame;

int main()
{
    int sockfd,addrSize;
    struct sockaddr_in serverAddr,clientAddr;
    char buffer[1024];
    
    int frameId=0;
    Frame frameReceived,frameSend;
    sockfd=socket(AF_INET,SOCK_DGRAM,0);

    serverAddr.sin_addr.s_addr=INADDR_ANY;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(PORT);

    bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    addrSize=sizeof(clientAddr);

    while(1)
    {
        int frameRecvSize=recvfrom(sockfd,&frameReceived,sizeof(Frame),0,(struct sockaddr*)&clientAddr,&addrSize);
        if(frameRecvSize>0 && frameReceived.frameKind==1 && frameReceived.seqNo==frameId)
        {
            printf("Frame Recieved : %s\n",frameReceived.packet.data);

            frameSend.seqNo=0;
            frameSend.frameKind=0;
            frameSend.ackNo=frameReceived.seqNo+1;
            sendto(sockfd,&frameSend,sizeof(frameSend),0,(struct sockaddr*)&clientAddr,addrSize);
            printf("Ack Send\n");
        }
        else
        {
            printf("Frame not received");
        }
        frameId++;
    }
    close(sockfd);
    return 0;

}
