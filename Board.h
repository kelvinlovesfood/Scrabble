
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <deque>
#include <tuple>
#include <algorithm>
#include "Tile.h"
#include "LinkedList.h"
#include "Player.h"

#define DIM 15
#define TEXT_SIZE 235886
#define MAX_ARRAY_SIZE 98
#define BOARD_CENTER DIM/2
#define BINGO 7
#define CHAR_TO_INT_OFFSET 65
#define BOARD_LINE_SIZE 61

class Board {
public:
   Board();
   Board(std::string tilebag, std::vector<std::string> board);
   ~Board();
   void print();
   void buildHand(Player* player);
   void buildBag();
   void buildDict();
   bool replaceWithTileFromBag(bool& betterInvalidInput, Player* player, Letter letter);

   // places tiles in a queue for legality checks, does not modify the board in any way
   void placeBuffer(bool& betterInvalidInput, Letter letter, std::string coords);

   // does the legality check, otherwise empty queue
   bool placeAll(bool& betterInvalidInput, bool& dictionary, Player* player);

   std::string getBoard();
   std::string getTiles();

   // recursive function to determine if all tiles are connected on the board
   // the user is contractually obligated to only call this function with the parameters:
   // (7, 7, 0);
   void sweep(int row, int col, int& count);

   bool wordCheck(std::string input);


private:
   LinkedList* tilebag;
   std::vector<std::vector<Tile*>> board;
   std::deque<std::tuple<Letter, int, int>> tileQueue;
   int board2[15][15] = { };
   char board3[15][15] = { };
   std::vector<std::string> dict;
};

#endif // BOARD_H
