//
//  http.h
//  
//
//  Created by Daniil Popov on 9/4/15.
//
//

#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <ctype.h>


#ifndef ____http__
#define ____http__

char * RequestGet(char * link);
char * TryRequestGet(char * link);

#endif /* defined(____http__) */
