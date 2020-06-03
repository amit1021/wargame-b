#include <iostream>
#include "Soldier.hpp"


class SniperCommander : public Soldier
{

public:
    SniperCommander(int player): Soldier(100,50,player){}
    ~SniperCommander();
};
