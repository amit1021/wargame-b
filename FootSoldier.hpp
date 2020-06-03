#include <iostream>
#include "Soldier.hpp"
class FootSoldier : public Soldier
{

public:
    FootSoldier(int player) : Soldier(100, 10, player) {}
    ~FootSoldier();
};
