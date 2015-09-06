//
//  Parser.c
//  
//
//  Created by Daniil Popov on 9/4/15.
//
//

#define HOST ("http://www.google.com")

#include "Parser.h"
#include <time.h>

int CanMakeMoney(char * link)
{
    const char * string = GetRequest(link);
    int startindex = 850;
    int endindex;
    int head = 0;
    double lowest;
    double median;
    char buffer[32];
    
    for (short i = 0; i != 2; i += 1)
    {
        while (string[startindex++] != '$');
        endindex = startindex;
        while (string[++endindex] != '\"');
        while (startindex != endindex)
            buffer[head++] = string[startindex++];
        buffer[head++] = '\0';
    }
    lowest = atof(buffer);
    median = atof(&(buffer[strlen(buffer)+1]));
    if ((lowest / median) < 0.85)
        return 1;
    return 0;
}

int main(int argc, char * argv[])
{
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    int response = CanMakeMoney("http://steamcommunity.com/market/priceoverview/?currency=0&appid=730&market_hash_name=StatTrak%E2%84%A2%20P250%20%7C%20Steel%20Disruption%20%28Factory%20New%29");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    if (response)
        printf("yes\n");
    else
        printf("no\n");
    printf("%f s", cpu_time_used);
    
    return 1;
}

