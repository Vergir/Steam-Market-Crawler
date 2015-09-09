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
#define SOMEWEHERE_BEFORE_PRICE 850
#define DELAY_BETWEEN_REQUESTS 1
#define PRINT_NOW fflush(stdout);

#include "Parser.h"

void SaveToFile(void * string, char * filename)
{
    
    FILE * file = fopen(filename, "w");
    fputs(string, file);
    fclose(file);
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
    const char * string = TryRequestGet(link);
    int startindex = SOMEWEHERE_BEFORE_PRICE;
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

int FindEndOfName(char * string)
{
    int result = 0;
    while (string[++result] != '<')
        if (isascii(string[result] == 0))
            return -1;
    return result;
}

char * ParseItemNames(char * link, char delimiter)
{
    short endOfName;
    char * items = calloc(ITEMS_PER_PAGE*100, sizeof(char));
    char * string = SkipMagicString(TryRequestGet(link), 3, 1);
    int offset = 0;

    for (short int i = 0; i != ITEMS_PER_PAGE; i += 1)
    {
        string = SkipMagicString(string, 1, 4);
        if ((endOfName = FindEndOfName(string)) < 0)
            continue;
        memcpy(items+offset, string, endOfName);
        items[offset+endOfName++] = delimiter;
        offset += endOfName;
    }
    items[offset] = '\0';
    
    return items;
}

char * ParseMarketItemNames(enum Games game, char delimiter)
{
    char * string = malloc(sizeof(char)*200);
    unsigned int numOfItems = 0;
    int offset = 0;
    
    sprintf(string, "http://steamcommunity.com/market/search/render/?query=appid:%d&start=1&count=1&currency=0&l=english&cc=pt", game);
    string = strstr(TryRequestGet(string), "nt\":")+3;
    while (*(++string) >= '0')
        numOfItems = numOfItems * 10 + *string - '0';
    numOfItems -= ITEMS_PER_PAGE;
    printf("%d to Parse\n", numOfItems);
    printf("Parsed: ");
    char * result = calloc(numOfItems*100, sizeof(char));
    for (int pageNumber = 1; pageNumber < numOfItems; pageNumber += ITEMS_PER_PAGE)
    {
        sprintf(string, "http://steamcommunity.com/market/search/render/?query=appid:%d&start=%d&count=100&currency=0&l=english&cc=pt", game, pageNumber);
        string = ParseItemNames(string, delimiter);
        printf("%d-%d...", pageNumber, pageNumber+99);PRINT_NOW
        memcpy(result+offset, string, strlen(string));
        offset += strlen(string);
        sleep(DELAY_BETWEEN_REQUESTS);
    }
    result[offset] = '\0';
    
    sprintf(string, "ItemNames_%d.txt", game);
    SaveToFile(result, string);
    
    return result;
}