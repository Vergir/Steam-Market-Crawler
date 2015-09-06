//
//  main.c
//  Steam Market Crawler
//
//  Created by Daniil Popov on 9/6/15.
//  Copyright (c) 2015 Vergir. All rights reserved.
//

#include <time.h>
#include "Parser.h"

int main()
{
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    char * response = ParseItemNames("http://steamcommunity.com/market/search/render/?query=appid:570&start=1&count=20&currency=0&l=english&cc=pt", '\n');
    for (short i = 0; i != 20; i += 1)
        printf("%s", response);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f s", cpu_time_used);
    
    return 1;
}
