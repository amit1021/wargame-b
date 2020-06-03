#include <iostream>
#include "Soldier.hpp"
class Paramedic : public Soldier
{

public:
    Paramedic(int player) : Soldier(100, 50, player) {}
    ~Paramedic();
};