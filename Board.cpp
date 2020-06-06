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
  int i = source.first, j = source.second;
  // source out of bounds
  if ((i >= board.size()) || (j < 0))
  {
    throw runtime_error("source out of bounds");
  }
  switch (direction)
  {
  case MoveDIR::Up:
    if ((i + 1 >= board.size()) || ((board[i + 1][j])))
      throw runtime_error("This place in not free");
    break;
  case MoveDIR::Down:
    if ((i - 1 < 0) || (board[i - 1][j]))
      throw runtime_error("This place in not free");
    break;
  case MoveDIR::Right:
    if ((j + 1 >= board.size()) || (board[i][j + 1]))
      throw runtime_error("This place in not free");
    break;
  case MoveDIR::Left:
    if ((j - 1 < 0) || (board[i][j - 1]))
      throw runtime_error("This place in not free");
    break;
  }
}
void Board::move(uint player_number, std::pair<int, int> source, MoveDIR direction)
{
  availablePlace(source, direction);
  //check if source place is not null
  Soldier *&currSoldier = board[source.first][source.second];
  int i = source.first, j = source.second;
  if (!currSoldier)
  {
    throw runtime_error("There is no player");
  }
  switch (direction)
  {
  case MoveDIR::Up:
    board[i + 1][j] = currSoldier;
    currSoldier = nullptr;
    shoot({i + 1, j}, player_number);
    break;

  case MoveDIR::Down:
    board[i - 1][j] = currSoldier;
    currSoldier = nullptr;
    shoot({i - 1, j}, player_number);

    break;

  case MoveDIR::Right:
    board[i][j + 1] = currSoldier;
    currSoldier = nullptr;
    shoot({i, j + 1}, player_number);

    break;

  case MoveDIR::Left:
    board[i][j - 1] = currSoldier;
    currSoldier = nullptr;
    shoot({i, j - 1}, player_number);
    break;
  }
  return;
}

// returns true if the board contains one or more soldiers of the given player.
bool Board::has_soldiers(int player_number) const
{
  for (size_t i = 0; i < board.size(); i++)
  {
    for (size_t j = 0; j < board[0].size(); j++)
    {
      // check the place is not null and the player in this place has pointHelth
      if (board[i][j] && (board[i][j]->player == player_number))
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
      if (board[i][j] && board[i][j]->player != player )
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
  int i = source.first, j = source.second;
  Soldier *&currSoldier = board[i][j];
  if (typeid(*currSoldier).name() == typeid(Paramedic).name())
  {
    paramedicShoot(source, player);
  }
  else
  {
    Soldier *target = &Board::getTarget(i, j, player);
    if (target)
    {
      target->setH(target->pointHealth - currSoldier->pointDamage);
    }
  }
  // Commanders
  if (typeid(*currSoldier) == typeid(FootCommander))
  {
    shootCommander(source, player, "Foot");
  }
  else if (typeid(*currSoldier) == typeid(SniperCommander))
  {
    shootCommander(source, player, "Sniper");
  }
  else if (typeid(*currSoldier) == typeid(ParamedicCommander))
  {
    shootCommander(source, player, "Paramedic");
  }
  Soldier::setNull(board);
}

void Board ::paramedicShoot(std::pair<int, int> source, int player)
{
  int i = source.first, j = source.second;
  //up
  if (i - 1 >= 0 && board[i - 1][j])
  {
    if (board[i + 1][j]->player == player)
    {
      Soldier *&currSoldier = board[i + 1][j];
      currSoldier->pointHealth = currSoldier->maxHelth;
    }
  }
  //down
  if (source.first - 1 >= 0 && board[i - 1][j])
  {
    if (board[i - 1][j]->player == player)
    {
      Soldier *&currSoldier = board[i + 1][j];
      currSoldier->pointHealth = currSoldier->maxHelth;
    }
  }
  //right
  if (i + 1 < board.size() && board[i][j + 1])
  {
    if (board[i][j + 1]->player == player)
    {
      Soldier *&currSoldier = board[i][j + 1];
      currSoldier->pointHealth = currSoldier->maxHelth;
    }
  }
  if (source.second - 1 >= 0 && board[i][j - 1])
  {
    if (board[i][j - 1]->player == player)
    {
      Soldier *&currSoldier = board[i][j - 1];
      currSoldier->pointHealth = currSoldier->maxHelth;
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
