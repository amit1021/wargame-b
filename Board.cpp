#include <iostream>
#include "Board.hpp"
#include "FootSoldier.hpp"
#include "FootCommander.hpp"
#include "Sniper.hpp"
#include "Paramedic.hpp"
#include "SniperCommander.hpp"
#include "ParamedicCommander.hpp"

// #include <cstdlib>

using namespace std;
using namespace WarGame;

Soldier *&Board ::operator[](std::pair<int, int> location)
{
  return board[location.first][location.second];
}

// operator for reading which soldiers are on the game-board.
Soldier *Board ::operator[](std::pair<int, int> location) const
{
  return board[location.first][location.second];
}
// check if in the player can move to the new place
void Board::availablePlace(std::pair<int, int> source, MoveDIR direction)
{
  // source out of bounds
  if ((source.first >= board.size()) || (source.first < 0) || (source.second >= board.size()) || (source.second < 0))
  {
    throw runtime_error("source out of bounds");
  }
  switch (direction)
  {
  case MoveDIR::Up:
    if ((source.first + 1 >= board.size()) || (board[source.first + 1][source.second] && board[source.first + 1][source.second]->pointHealth > 0))
      throw runtime_error("This place in not free");
    break;
  case MoveDIR::Down:
    if ((source.first - 1 < 0) || (board[source.first - 1][source.second] && board[source.first - 1][source.second]->pointHealth > 0))
      throw runtime_error("This place in not free");
    break;
  case MoveDIR::Right:
    if ((source.second + 1 >= board.size()) || (board[source.first][source.second + 1] && board[source.first][source.second + 1]->pointHealth > 0))
      throw runtime_error("This place in not free");
    break;
  case MoveDIR::Left:
    if ((source.second - 1 < 0) || (board[source.first][source.second - 1] && board[source.first][source.second - 1]->pointHealth > 0))
      throw runtime_error("This place in not free");
    break;
  }
}
void Board::move(uint player_number, std::pair<int, int> source, MoveDIR direction)
{
  availablePlace(source, direction);
  //check if source place is not null
  if (!board[source.first][source.second])
  {
    throw runtime_error("There is no player");
  }
  switch (direction)
  {
  case MoveDIR::Up:
    if ((!board[source.first + 1][source.second]))
    {
      board[source.first + 1][source.second] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      shoot({source.first + 1, source.second}, player_number);
    }
    break;
  case MoveDIR::Down:
    if ((!board[source.first - 1][source.second]))
    {
      board[source.first - 1][source.second] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      shoot({source.first - 1, source.second}, player_number);
    }
    break;

  case MoveDIR::Right:

    if (!board[source.first][source.second + 1])
    {
      board[source.first][source.second + 1] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      shoot({source.first, source.second + 1}, player_number);
    }
    break;

  case MoveDIR::Left:
    if ((!board[source.first][source.second - 1]))
    {
      board[source.first][source.second - 1] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      shoot({source.first, source.second - 1}, player_number);
    }
    break;
  }
  return;
}

// returns true if the board contains one or more soldiers of the given player.
bool Board::has_soldiers(uint player_number) const
{
  for (size_t i = 0; i < board.size(); i++)
  {
    for (size_t j = 0; j < board[0].size(); j++)
    {
      // check the place is not null and the player in this place has pointHelth
      if (board[i][j] && board[i][j]->pointHealth > 0 && (board[i][j]->player == player_number))
      {
        return true;
      }
    }
  }
  return false;
}

Soldier &Board ::getTarget(int i1, int j1, int player)
{
  int min = INT_MAX;
  int diff = 0;
  std::pair<int, int> target;
  for (size_t i = 0; i < board.size(); i++)
  {
    for (size_t j = 0; j < board[0].size(); j++)
    {
      if (board[i][j] && board[i][j]->player != player)
      {
        int diffI = i - i1;
        diffI = abs(diffI);
        int diffJ = j - j1;
        diffJ = abs(diffJ);
        diff = diffI + diffJ;
        if (diff < min && diff != 0)
        {
          min = diff;
          target = {i, j};
        }
      }
    }
  }
  return *board[target.first][target.second];
}

void Board ::shoot(std::pair<int, int> source, int player)
{
  if (typeid(*board[source.first][source.second]).name() == typeid(Paramedic).name())
  {
    paramedicShoot(source, player);
  }
  else
  {
    Soldier *target = &Board::getTarget(source.first, source.second, player);
    target->setH(target->pointHealth - board[source.first][source.second]->pointDamage);
  }
  // Commanders
  if (typeid(*board[source.first][source.second]) == typeid(FootCommander))
  {
    shootCommander(source, player, "Foot");
  }
  else if (typeid(*board[source.first][source.second]) == typeid(SniperCommander))
  {
    shootCommander(source, player, "Sniper");
  }
  else if (typeid(*board[source.first][source.second]) == typeid(ParamedicCommander))
  {
    shootCommander(source, player, "Paramedic");
  }
}

void Board ::paramedicShoot(std::pair<int, int> source, int player)
{
  //up
  if (source.first + 1 < board.size() && board[source.first + 1][source.second])
  {
    if (board[source.first + 1][source.second]->player = player && board[source.first + 1][source.second]->pointHealth > 0)
    {
      board[source.first + 1][source.second]->pointHealth = board[source.first + 1][source.second]->maxHelth;
    }
  }
  //down
  if (source.first - 1 >= 0 && board[source.first - 1][source.second])
  {
    if (board[source.first - 1][source.second]->player = player && board[source.first - 1][source.second]->pointHealth > 0)
    {
      board[source.first - 1][source.second]->pointHealth = board[source.first - 1][source.second]->maxHelth;
    }
  }
  //right
  if (source.second + 1 < board.size() && board[source.first][source.second + 1])
  {
    if (board[source.first][source.second + 1]->player = player && board[source.first][source.second + 1]->pointHealth > 0)
    {
      board[source.first][source.second + 1]->pointHealth = board[source.first][source.second + 1]->maxHelth;
    }
  }
  if (source.second - 1 >= 0 && board[source.first][source.second - 1])
  {
    if (board[source.first][source.second - 1]->player = player && board[source.first][source.second - 1]->pointHealth > 0)
    {
      board[source.first][source.second - 1]->pointHealth = board[source.first][source.second - 1]->maxHelth;
    }
  }
}
void Board ::shootCommander(std::pair<int, int> source, int player, std::string type)
{
  for (size_t i = 0; i < board.size(); i++)
  {
    for (size_t j = 0; j < board.size(); j++)
    {
      if (board[i][j] != nullptr)
      {
        if (type == "Foot" && player == board[i][j]->player)
        {
          if (typeid(*board[i][j]) == typeid(FootSoldier))
          {
            shoot({i, j}, player);
          }
        }
        else if (type == "Sniper" && player == board[i][j]->player)
        {
          if (typeid(*board[i][j]) == typeid(Sniper))
          {
            shoot({i, j}, player);
          }
        }
        else if (type == "Paramedic" && player == board[i][j]->player)
        {
          if (typeid(*board[i][j]) == typeid(Paramedic))
          {
            paramedicShoot({i, j}, player);
          }
        }
      }
    }
  }
}

// int main()
// {
//   WarGame::Board board(3, 3);
//   board[{0, 1}] = new FootSoldier(1);   //soldier 1 - 100ssssss
//   board[{0, 2}] = new FootSoldier(1);   //soldier 1 - 100
//   board[{0, 0}] = new FootCommander(1); //soldier 1 - 100
//   board[{2, 0}] = new Sniper(2);        //soldier 1 - 100
//   board[{2, 1}] = new Sniper(2);        //soldier 1 - 100
//   board[{2, 2}] = new SniperCommander(2);

//    board.move(1, {0, 0}, WarGame::Board::MoveDIR::Up); //sodier 1 - 80

//   //   board[{0, 2}] = new Paramedic(1);   //soldier 1 - 100
//   //   board[{2, 0}] = new FootSoldier(2); //soldier 1 - 100
//   //   board[{2, 2}] = new Paramedic(2);   //soldier 1 - 100
//   //  board.move(1, {8, 2}, WarGame::Board::MoveDIR::Down); //sodier 1 - 80
//   //   board.move(2, {2, 2}, WarGame::Board::MoveDIR::Left); //sodier 1 - 80
//   //   board.move(2, {2, 0}, WarGame::Board::MoveDIR::Down); //sodier 1 - 80
//   //   board.move(1, {0, 2}, WarGame::Board::MoveDIR::Up);   //sodier 1 - 80
// }