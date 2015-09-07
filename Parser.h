//
//  Parser.h
//  
//
//  Created by Daniil Popov on 9/4/15.
//
//

#ifndef ____Parser__
#define ____Parser__

#include "http.h"
#include <time.h>

enum Games { dota2 = 570, tf2 = 440, csgo = 730 };

int ParsePrices(char * link, double profitPercent);
char * ParseItemNames(char * link, char delimiter);
char * ParseItemMarket(enum Games game, char delimiter);

#endif /* defined(____Parser__) */
