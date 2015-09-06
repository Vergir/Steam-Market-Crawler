//
//  Parser.c
//  
//
//  Created by Daniil Popov on 9/4/15.
//
//

#define MAKE_MONEY_PERCENT 0.85

#include "Parser.h"
#include <time.h>

enum Games { dota2 = 570, tf2 = 440, csgo = 730 };

int ParsePrices(char * link, double profitPercent)
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
    if ((lowest / median) < profitPercent)
        return 1;
    return 0;
}

char * ParseItemNames(char * link, char delimiter)
{
    short head;
    char * items = calloc(2049, sizeof(char));
    char * string = strstr(strstr(strstr(GetRequest(link), ";\\\">") + 1, ";\\\">") + 1, ";\\\">") + 1;
    int offset = 0;
    
    for (short int i = 0; i != 20; i += 1)
    {
        head = 0;
        string = strstr(string, ";\\\">") + 4;
        while (string[++head] != '<');
        memcpy(items+offset, string, head);
        items[offset+head++] = delimiter;
        offset+= head;
    }
    items[offset] = '\0';
    
    return items;
}

char * ParseItemMarket(char * link, enum Games game, char delimiter)
{
}