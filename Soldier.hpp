
#pragma once
#include <iostream>
#include <bits/stdc++.h>

class Soldier
{
public:
    int pointHealth;
    int pointDamage;
    int player;
    int maxHelth;
    virtual void f() {}
    Soldier(int pointHealth, int pointDamage, int player)
    {
        this->player = player;
        this->pointHealth = pointHealth;
        this->pointDamage = pointDamage;
        maxHelth = pointHealth;
    }
    void setH(int d)
    {
        pointHealth = d;
    }
    static void setNull(std::vector<std::vector<Soldier *>> &board)
    {
        for (size_t i = 0; i < board.size(); i++)
        {
            for (size_t j = 0; j < board.size(); j++)
            {
                if (board[i][j] && board[i][j]->pointHealth <= 0)
                {
                    board[i][j] = nullptr;
                }
            }
        }
    }
};