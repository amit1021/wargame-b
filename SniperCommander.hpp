#include <iostream>
#include "Soldier.hpp"


class SniperCommander : public Soldier
{

public:
    SniperCommander(int player): Soldier(120,50,player){}
    ~SniperCommander();
};
