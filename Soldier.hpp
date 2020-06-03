#pragma once
#include <iostream>
#include <bits/stdc++.h>

class Soldier
{
public:
    int pointHealth;
    int pointDamage;
    int player;

    Soldier(int pointHealth, int pointDamage, int player)
    {
        this->player = player;
        this->pointHealth = pointHealth;
        this->pointDamage = pointDamage;
    }
    void setH(int d)
    {
        pointHealth = d;
    }
};