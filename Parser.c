//
//  Parser.c
//  
//
//  Created by Daniil Popov on 9/4/15.
//
//

#define MAKE_MONEY_PERCENT 0.85
#define ITEMS_PER_PAGE 100
#define MAGIC_STRING ";\\\">"

#include "Parser.h"

char * SafeGetRequest(char * link)
{
    char * result;
    while ((result = GetRequest(link)) == NULL);
    return result;
}

char * SkipMagicString(char * string, int amount, int offset)
{
    char * tmp;
    while (amount-- > 0)
    {
        tmp = strstr(string, MAGIC_STRING);
        if (tmp != NULL)
            string = tmp + offset;
        else
            return string;
    }
    return string;
}

int ParsePrices(char * link, double profitPercent)
{
    const char * string = SafeGetRequest(link);
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
    char * items = calloc(ITEMS_PER_PAGE*100, sizeof(char));
    char * string = SkipMagicString(SafeGetRequest(link), 3, 1);
    int offset = 0;

    for (short int i = 0; i != ITEMS_PER_PAGE; i += 1)
    {
        head = 0;
        string = SkipMagicString(string, 1, 4);
        while (string[++head] != '<');
        memcpy(items+offset, string, head);
        items[offset+head++] = delimiter;
        offset += head;
    }
    items[offset] = '\0';
    
    return items;
}

char * ParseItemMarket(enum Games game, char delimiter)
{
    char * string = malloc(sizeof(char)*200);
    sprintf(string, "http://steamcommunity.com/market/search/render/?query=appid:%d&start=1&count=1&currency=0&l=english&cc=pt", game);
    string = strstr(SafeGetRequest(string), "nt\":")+3;
    unsigned int numOfItems = 0;
    while (*(++string) >= '0')
        numOfItems = numOfItems * 10 + *string - '0';
    printf("%d items\n\n",numOfItems);
    numOfItems -= ITEMS_PER_PAGE;
    int offset = 0;
    char * result = calloc(numOfItems*100, sizeof(char));
    for (int pageNumber = 1; pageNumber < numOfItems; pageNumber += ITEMS_PER_PAGE)
    {
        sprintf(string, "http://steamcommunity.com/market/search/render/?query=appid:%d&start=%d&count=100&currency=0&l=english&cc=pt", game, pageNumber);
        string = ParseItemNames(string, delimiter);
        printf("Items got: %d-%d\n", pageNumber, pageNumber + 100);
        memcpy(result+offset, string, strlen(string));
        offset += strlen(string);
    }
    result[offset] = '\0';
    
    sprintf(string, "ItemList: AppID=%d; Count=%d", game, numOfItems);
    FILE * file = fopen(string, "w");
    fputs(result, file);
    fclose(file);
    
    return result;
}