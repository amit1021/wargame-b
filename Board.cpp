#include <iostream>
#include "Board.hpp"
#include "FootSoldier.hpp"
#include "FootCommander.hpp"
#include "Sniper.hpp"
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

void Board::move(uint player_number, std::pair<int, int> source, MoveDIR direction)
{

  switch (direction)
  {
  case MoveDIR::Up:
    if (!board[source.first + 1][source.second])
    {
      board[source.first + 1][source.second] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      //   board[source.first + 1][source.second]->shoot();
    }

    break;
  case MoveDIR::Down:
    if (!board[source.first - 1][source.second])
    {
      board[source.first - 1][source.second] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      shoot({source.first - 1,source.second},player_number);
    }
    break;

  case MoveDIR::Right:
    if (!board[source.first][source.second + 1])
    {
      board[source.first][source.second + 1] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      //   shoot(board[source.first][source.second + 1]);
    }

    break;
  case MoveDIR::Left:
    if (!board[source.first][source.second - 1])
    {
      board[source.first][source.second - 1] = board[source.first][source.second];
      board[source.first][source.second] = nullptr;
      ///   board[source.first][source.second - 1]->shoot();
    }
    break;
  }

  return;
}

// returns true iff the board contains one or more soldiers of the given player.
bool Board::has_soldiers(uint player_number) const
{
  for (size_t i = 0; i < board.size(); i++)
  {
    for (size_t j = 0; j < board[0].size(); j++)
    {
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
  Soldier* target = &Board::getTarget(source.first, source.second,player);
  target->setH(target->pointHealth - board[source.first][source.second]->pointDamage);
}

// int main()
// {
//   Board b(5, 5);
//   b[{1, 1}] = new FootSoldier(1);
//   b[{1, 2}] = new FootCommander(1);
//   b[{2, 4}] = new Sniper(1);

//   b[{3, 3}] = new FootSoldier(2);
//   b.move(2, {3, 3}, Board::MoveDIR::Down);

//    std::cout << "{2,4}: " << b[{2, 4}]->pointHealth << std::endl;
//   // std::cout << "{1,0}: " << b[{1, 0}]->player << std::endl;
//   // std::cout << "{0,2}: " << b[{0, 2}]->player << std::endl;
//   //std::cout << "PointHelth: " << b.getTarget(3, 3, 2).pointHealth << "  PointDamage: " << b.getTarget(3, 3, 2).pointDamage << std::endl;
// }