#include <iostream>
#include "Soldier.hpp"

class FootCommander : public Soldier
{

public:
    FootCommander(int player) : Soldier(150,20,player){} 
    ~FootCommander();
};
