//
//  main.c
//  Steam Market Crawler
//
//  Created by Daniil Popov on 9/6/15.
//  Copyright (c) 2015 Vergir. All rights reserved.
//

#include "Parser.h"

int main()
{
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    char * response = ParseItemMarket(csgo, '\n');
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%f s", cpu_time_used);
    
    return 1;
}
