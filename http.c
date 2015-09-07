//
//  http.c
//  
//
//  Created by Daniil Popov on 9/4/15.
//
//

#define BUFFER 100000

#include "http.h"

int CheckForEnd(char * last)
{
    for (short i = 0; i != -127; i--)
        if (last[i] == '}')
            return 1;
    return 0;
}

void expand(char *  dst, char * src, long int n)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dst[n+i] = src[i];
        i += 1;
    }
    dst[n+i] = '\0';
}

char* gethost(char ** link)
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

char * GetRequest(char* link)
{
    char* host = gethost(&link);
    int sockfd = 0;
    size_t total;
    int sent;
    int received;
    long int bytes;
    char sendBuffer[BUFFER];
    char recvBuffer[BUFFER];
    char * result = calloc(400000, sizeof(char));
    struct sockaddr_in serv_addr;
    struct hostent * he;
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
    {
        printf("\n Error : Could not create socket \n");
        exit(1);
    }
    
    he = gethostbyname(host);
    if (he == NULL)
    {
        perror("\n Error: Getting Host Failed\n");
        exit(1);
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80);
    memcpy(&serv_addr.sin_addr.s_addr, he->h_addr, he->h_length);
    
    int connectionStatus;
    if ((connectionStatus = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
        perror("\n Error: Connect Failed\n");
        exit(1);
    }

    snprintf(sendBuffer, BUFFER,
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n\r\n"
             ,link, host);
    
    total = strlen(sendBuffer);
    sent = 0;
    do
    {
        bytes = write(sockfd,sendBuffer+sent,total-sent);
        if (bytes < 0)
            perror("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent += bytes;
    }
    while (sent < total);
    
    total = BUFFER-1;
    received = 0;
    do
    {
        memset(recvBuffer, 0, sizeof(recvBuffer));
        bytes = read(sockfd,recvBuffer, total);
        if (bytes < 0)
               perror("ERROR reading response from socket");
        memcpy(result+received, recvBuffer, bytes);
        received += bytes;
        if (strchr(recvBuffer, '}') != NULL)
            break;
    }
    while (1);
    
    close(sockfd);
    
    return result;
}