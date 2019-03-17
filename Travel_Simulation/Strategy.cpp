#include "Strategy.h"

Strategy::Strategy(int t, string d1, string d2, Time T)
{
    type = t;
    depart = d1;
    dest = d2;
    expectedTime = T;
}

void Strategy::startStrategy()
{
    switch (type) {
        case 1:
            cheapestStrategy();
            break;
        case 2:
            fastestStrategy();
            break;
        case 3:
            timeLimitStrategy();
            break;
    }
}
