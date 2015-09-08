//
//  http.c
//  
//
//  Created by Daniil Popov on 9/4/15.
//
//

#define BUFFER 32769
#define CHARS_ON_PAGE 400000
#define HTTP_OK 200
#define HTTP_TMR 429
#define TMR_COOLDOWN 300

#include "http.h"

char * GetHost(char ** link)
{
    char host[strlen(*link)];
    int offset = 7;
    int n = 0;
    while ((*link)[offset+n] != '/' && (*link)[n+offset] != '\0')
    {
        host[n] = (*link)[offset+n];
        n += 1;
    }
    char * result = malloc(sizeof(char)*n);
    memcpy(result, host, n);
    result[n] = '\0';
    *link += n + offset;
    return result;
}

void SetUpServerAddress(struct sockaddr_in * serverAddress)
{
    memset(serverAddress, 0, sizeof(*serverAddress));
    serverAddress->sin_family = AF_INET;
    serverAddress->sin_port = htons(80);
}
int SetUpSocket(char * link)
{
    int socketfd = 0;
    char * host = GetHost(&link);
    struct sockaddr_in serv_addr;
    struct hostent * he;
    
    if ((socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        perror("\nError : Could not create socket\n");
    
    if ((he = gethostbyname(host)) == NULL)
        perror("\nError: Could not get Host\n");
    
    SetUpServerAddress(&serv_addr);
    memcpy(&serv_addr.sin_addr.s_addr, he->h_addr, he->h_length);
    
    if ((connect(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
        perror("\nError: Could not connect socket\n");
    
    return socketfd;
    
}
int GetStatusFromResponse(int socketfd)
{
    char buffer[13];
    buffer[12] = '\0';
    
    size_t wow;
    if ((wow = read(socketfd, buffer, 12)) > 0)
        return atoi(&(buffer[9]));
    else
        return -1;
}

char * GetRequest(char * link)
{
    int socket = 0;
    long int bytes;
    size_t packetSize, received = 0;
    char sendBuffer[BUFFER];
    char recvBuffer[BUFFER];
    char * result = calloc(CHARS_ON_PAGE, sizeof(char));
    
    socket = SetUpSocket(link);
    char * host = GetHost(&link);

    snprintf(sendBuffer, BUFFER, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n" ,link, host);
    
    packetSize = strlen(sendBuffer);
    for (long sent = 0, packetSize = strlen(sendBuffer); sent < packetSize; sent += bytes)
        bytes = write(socket, sendBuffer + sent,packetSize - sent);
    
    if ((packetSize = GetStatusFromResponse(socket)) != HTTP_OK)
    {
        if (packetSize == -1)
            perror("\nCould not read a response from server\n");
        else
            printf("\nSomething is wrong...Status code: %d\n", packetSize);
        if (packetSize == HTTP_TMR)
            sleep(TMR_COOLDOWN);
        return NULL;
    }
    
    packetSize = BUFFER - 1;
    
    do
    {
        memset(recvBuffer, 0, packetSize);
        bytes = read(socket,recvBuffer, packetSize);
        if (bytes < 0)
            perror("ERROR reading response from socket");
        memcpy(result+received, recvBuffer, bytes);
        received += bytes;
    }
    while(strchr(recvBuffer, '}') == NULL);
        
    
    close(socket);
    
    return result;
}