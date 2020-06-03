#include <iostream>
#include "Soldier.hpp"
class ParamedicCommander : public Soldier
{

public:
    ParamedicCommander(int player):Soldier(100,50,player){}
    ~ParamedicCommander();
};