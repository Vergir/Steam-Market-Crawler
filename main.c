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
    setbuf(stdout, NULL);
    
    ParseMarketItemNames(csgo, '\n');
    return 1;
}
