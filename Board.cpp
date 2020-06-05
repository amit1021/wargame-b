#include <iostream>
#include "Board.hpp"
#include "FootSoldier.hpp"
#include "FootCommander.hpp"
#include "Sniper.hpp"
#include "Paramedic.hpp"
#include "SniperCommander.hpp"
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
  if (typeid(*board[source.first][source.second]).name() == typeid(FootCommander).name())
  {
    CommanderShoot(player, "Foot");
  }
  else if (typeid(*board[source.first][source.second]).name() == typeid(SniperCommander).name())
  {
    CommanderShoot(player, "Sniper");
  }
  else
  {
    CommanderShoot(player, "Paramedic");
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

void Board ::CommanderShoot(int player, string type)
{
  for (size_t i = 0; i < board.size(); i++)
  {
    for (size_t j = 0; j < board.size(); j++)
    {
      if (board[i][j] && type == "Foot")
      {
        // if (typeid(*board[source.first][source.second]).name() == typeid(Paramedic).name())

        if ((typeid(*board[i][j]).name() == typeid(FootSoldier).name()))
        {
          if (board[i][j]->player = player)
          {
            shoot({i, j}, player);
          }
        }
      }
      if (board[i][j] && type == "Sniper")
      {
        if (typeid(*board[i][j]).name() == typeid(Sniper).name())
        {
          if (board[i][j]->player = player)
          {
            shoot({i, j}, player);
          }
        }
      }
      else
      {
        if (board[i][j] && typeid(*board[i][j]).name() == typeid(Paramedic).name())
        {
          if (board[i][j]->player = player)
          {
            paramedicShoot({i, j}, player);
          }
        }
      }
    }
  }
}

//example
// if(NewType* v = dynamic_cast<NewType*>(old)) {
//    // old was safely casted to NewType
//    v->doSomething();
// }

// int main()
// {
//   WarGame::Board board(8, 8);
//   board[{0, 1}] = new FootSoldier(1); //soldier 1 - 100

//   board[{7, 1}] = new FootSoldier(2); //soldier 2 - 100

//   board.move(1, {0, 1}, WarGame::Board::MoveDIR::Up); //soldier 2 - 90

//   //CHECK_THROWS(board.move(1, {0, 1}, WarGame::Board::MoveDIR::Up)); //no soldier thehre
//   board.move(1, {1, 1}, WarGame::Board::MoveDIR::Up);               //soldier 2 - 80
//   CHECK(board.has_soldiers(2));
//   CHECK(board.has_soldiers(1));

//   CHECK_THROWS(board.move(1, {7, 1}, WarGame::Board::MoveDIR::Up)); //  cant go up !!
//   CHECK_THROWS(board.move(2, {7, 1}, WarGame::Board::MoveDIR::Up));
//   board.move(2, {7, 1}, WarGame::Board::MoveDIR::Down); //soldier 1 - 90
//   CHECK(board.has_soldiers(2));
//   CHECK(board.has_soldiers(1));
//   board.move(2, {6, 1}, WarGame::Board::MoveDIR::Down); //sodier 1 - 80
//   CHECK(board.has_soldiers(2));
//   CHECK(board.has_soldiers(1));
//   board.move(2, {5, 1}, WarGame::Board::MoveDIR::Down); //soldier 1 - 70
//   CHECK(board.has_soldiers(2));
//   CHECK(board.has_soldiers(1));

//   board.move(1, {2, 1}, WarGame::Board::MoveDIR::Up); //soldier 2 - 70
//   CHECK(board.has_soldiers(2));
//   CHECK(board.has_soldiers(1));

//   CHECK_THROWS(board.move(1, {3, 1}, WarGame::Board::MoveDIR::Up)); //there is another soldier in the destenation
// }

// int main()
// {
//   WarGame::Board board(3, 3);
//   board[{0, 1}] = new FootSoldier(1); //soldier 1 - 100
//   board[{0, 2}] = new Paramedic(1);   //soldier 1 - 100
//   board[{2, 0}] = new FootSoldier(2); //soldier 1 - 100
//   board[{2, 2}] = new Paramedic(2);   //soldier 1 - 100
//   board.move(1, {0, 1}, WarGame::Board::MoveDIR::Up);   //sodier 1 - 80
//   board.move(2, {2, 2}, WarGame::Board::MoveDIR::Left); //sodier 1 - 80
//   board.move(2, {2, 0}, WarGame::Board::MoveDIR::Down); //sodier 1 - 80
//   board.move(1, {0, 2}, WarGame::Board::MoveDIR::Up);   //sodier 1 - 80

// }

int main()
{
  WarGame::Board board(8, 8);
  //CHECK(!board.has_soldiers(1));
  board[{0, 1}] = new FootSoldier(1);   //player 1 footSoldier1 {0,1}
                                        // CHECK_THROWS((board[{0,1}] = new FootSoldier(1))); //there is already soldier there
  board[{0, 0}] = new FootCommander(1); //player 1 commanderSoldier {0,0}
  board[{0, 2}] = new FootSoldier(1);   //player 1 footSoldier2 {0,2}
  //CHECK(board.has_soldiers(1));         //there is no need to write this any time

  //CHECK(!board.has_soldiers(2));
  board[{7, 0}] = new FootCommander(2); //player 2 commanderSoldier {7,0}
  board[{7, 1}] = new FootSoldier(2);   //player 2 footSoldier1 {7,1}
  board[{7, 2}] = new FootSoldier(2);   //player 2 footSoldier2 {7,2}

  //CHECK(board.has_soldiers(2));

  board.move(1, {0, 1}, WarGame::Board::MoveDIR::Up); //player 2 footsoldier1 - 90
  //CHECK(board.has_soldiers(2));
  //CHECK(board.has_soldiers(1));
  //CHECK_THROWS(board.move(1, {0, 1}, WarGame::Board::MoveDIR::Up)); //no soldier there
  board.move(1, {1, 1}, WarGame::Board::MoveDIR::Up);               //player 2 footsoldier1 - 80
  //CHECK(board.has_soldiers(2));
  //CHECK(board.has_soldiers(1));

  //CHECK_THROWS(board.move(2, {7, 0}, WarGame::Board::MoveDIR::Left)); // cant move left
  board.move(2, {7, 0}, WarGame::Board::MoveDIR::Down);               //player 1 commanderSoldier - 130, player 1 footsoldier1 - 70,   player 1 footsoldier2 - 90,
  //CHECK(board.has_soldiers(2));
  //CHECK(board.has_soldiers(1));

  board.move(1, {0, 0}, WarGame::Board::MoveDIR::Up); //player 2 commanderSoldier - 130, player 2 footsoldier1 - 70,   player 2 footsoldier2 - 90,
  //CHECK(board.has_soldiers(2));
  //CHECK(board.has_soldiers(1));
  board.move(1, {1, 0}, WarGame::Board::MoveDIR::Down);
  //CHECK(board.has_soldiers(2));
  //CHECK(board.has_soldiers(1));
  board.move(1, {0, 0}, WarGame::Board::MoveDIR::Up);
  //CHECK(board.has_soldiers(2));
  //CHECK(board.has_soldiers(1));
  board.move(1, {1, 0}, WarGame::Board::MoveDIR::Down);
  // failed
  cout << board.has_soldiers(2) << endl;
}