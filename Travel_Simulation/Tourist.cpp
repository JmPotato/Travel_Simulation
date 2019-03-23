#include "Tourist.h"

Tourist::Tourist(string start, string end, int strategy) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
}

Tourist::Tourist(string start, string end, int strategy, MyTime t) {
//    id = 0;
    depart = start;
    dest = end;
    type = strategy;
    destTime = t;
}

unsigned short Tourist::getId() {
    return id;
}

string Tourist::getDepart() {
    return depart;
}

string Tourist::getDest() {
    return dest;
}

int Tourist::getType() {
    return type;
}

string Tourist::getLocation() {
    return location;
}

void Tourist::getStrategy() {
    Strategy touristStrategy(type, depart, dest, departTime, destTime);
    touristStrategy.startStrategy();
}
