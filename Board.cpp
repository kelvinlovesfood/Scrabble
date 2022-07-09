#include "Board.h"

/**
 * @brief Construct a new empty Board object.
 */
Board::Board() {
   this->tilebag = new LinkedList();
   for (int i = 0; i < DIM; i++) {
      std::vector<Tile*> row;
      for (int j = 0; j < DIM; j++) {
         row.push_back(nullptr);
      }
      this->board.push_back(row);
   }
}

/**
 * @brief Construct a new Board object with tilebag and board already made.
 */
Board::Board(std::string tilebag, std::vector<std::string> board) {
   this->tilebag = new LinkedList();

   std::vector<std::string> words;
   std::string tilebagCopy = tilebag;
   size_t pos = 0;
   std::string delim = " ";
   while ((pos = tilebagCopy.find(delim)) != std::string::npos) {
      words.push_back(tilebagCopy.substr(0, pos));
      tilebagCopy.erase(0, pos + delim.length());
   }

   for (std::string str : words) {
      Tile* tile = new Tile(str[STRING_CHAR_LOCATION], std::stoi(str.substr(STRING_VALUE_LOCATION, (str.length() - 1))));
      this->tilebag->add(tile);
   }

   for (const auto& line : board) {
      std::string line_copy = line;
      std::vector<std::string> sep;
      std::vector<Tile*> boardTiles;
      size_t pos = 0;
      std::string delim = "|";
      while ((pos = line_copy.find(delim)) != std::string::npos) {
         sep.push_back(line_copy.substr(0, pos));
         line_copy.erase(0, pos + delim.length());
      }
      for (const auto& str : sep) {
         std::string space = " ";
         if (str == space) {
            boardTiles.push_back(nullptr);
         }
         else {
            // Score of the tiles is put to 0 on placed tiles for simplicity
            boardTiles.push_back(new Tile(str[STRING_CHAR_LOCATION], 0));
         }
      }
      this->board.push_back(boardTiles);
   }
}

/**
 * @brief Destroy the Board object
 */
Board::~Board() {
   delete this->tilebag;
   for (int i = 0; i < DIM; i++) {
      for (int j = 0; j < DIM; j++) {
         delete this->board[i][j];
      }
   }
}

/**
 * @brief Reads tiles from ScrabbleTiles.txt and randomizes their order, putting them into tilebag.
 */
void Board::buildBag() {
   //Seed set as 0 for new game diff check to function
   srand(time(0));
   char letter[MAX_ARRAY_SIZE];
   int value[MAX_ARRAY_SIZE];
   int random = 0;
   std::ifstream tiles("ScrabbleTiles.txt");
   for (int i = 0; i < MAX_ARRAY_SIZE; i++) {
      tiles >> letter[i] >> value[i];
   }
   for (int j = 0; j < MAX_ARRAY_SIZE - 1; j++) {
      random = j + (rand() % (MAX_ARRAY_SIZE - j));
      Letter swapLetter = letter[j];
      letter[j] = letter[random];
      letter[random] = swapLetter;
      int swapValue = value[j];
      value[j] = value[random];
      value[random] = swapValue;
   }
   for (int k = 0; k < MAX_ARRAY_SIZE; k++) {
      Tile* tile = new Tile(letter[k], value[k]);
      if ((tile->getLetter()) && tile->getValue() != 0) {
         this->tilebag->add(tile);
      }
      else {
         delete tile;
      }

   }
   tiles.close();
}

void Board::buildDict() {
   std::ifstream file("words");
   while (file) {
      std::string tmp;
      std::getline(file, tmp);
      // Just double checks that something isn't going wrong here
      // Some OSes may add \r to the end of lines
      if (!tmp.empty() && tmp[tmp.size() - 1] == '\r')
         tmp.erase(tmp.size() - 1);

      std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
      this->dict.push_back(tmp);
   }
   file.close();
}

bool Board::wordCheck(std::string input) {
   bool ret = false;
   std::vector<std::string>::iterator it;
   it = std::find (this->dict.begin(), this->dict.end(), input);
   if (it != this->dict.end()) {
      ret = true;
   }
   return ret;
}

/**
 * @brief Make player draw 7 cards, for starting game.
 */
void Board::buildHand(Player* player) {
   for (int i = 0; i < HAND_SIZE; i++) {
      player->draw(tilebag);
   }
}

/**
 * @brief Swaps a tile in the player's hand with one in the tile bag.
 * @return true Tile replaced succesfully.
 * @return false No tiles in tile bag.
 */
bool Board::replaceWithTileFromBag(bool& betterInvalidInput, Player* player, Letter letter) {
   bool ret = true;
   Tile* tileFromBag = this->tilebag->drawTile();

   Tile* tileFromPlayer = player->getHand()->removeLetter(letter);
      player->getHand()->add(tileFromBag);
      this->tilebag->add(tileFromPlayer);

   // // just realised that this is redundant now becase the game will never be in an empty bag state
   // if (tileFromBag == nullptr) {
   //    if (betterInvalidInput) {
   //       std::cout << "There exists no more tiles for replacement" << std::endl;
   //    }
   //    else {
   //       std::cout << "Invalid input" << std::endl;
   //    }
   //    ret = false;
   // }

   // else {
      
   // }
   
   return ret;
}

/**
 * @brief Prints the board.
 */
void Board::print() {
   std::cout << "   ";
   for (int i = 0; i <= 9; ++i) {
      std::cout << "  " << i << " ";
   }
   std::cout << "  10  11  12  13  14";
   std::cout << std::endl;
   std::string line(BOARD_LINE_SIZE, '-');
   std::cout << "   " << line << std::endl;

   for (int row = 0; row < DIM; row++) {
      std::cout << " " << (char)('A' + row) << " |";
      for (int col = 0; col < DIM; col++) {
         if (this->board[row][col] != nullptr) {
            std::cout << " " << board[row][col]->getLetter() << " |";
         }
         else {
            std::cout << "   |";
         }
      }
      std::cout << std::endl;
   }
}

/**
 * @brief Returns a string representing the board.
 */
std::string Board::getBoard() {
   std::string ret = "";
   for (int row = 0; row < DIM; row++) {
      for (int col = 0; col < DIM; col++) {
         if (this->board[row][col] != nullptr) {
            ret += board[row][col]->getLetter();
            ret += "|";
         }
         else {
            ret += " |";
         }
      }
      ret += "\n";
   }

   return ret;
}

/**
 * @brief Pushes tile (letter, row and column) to tileQueue.
 * @param letter Letter of tile.
 * @param coords Coordinate string of tile. E.g. "A12".
 */
void Board::placeBuffer(bool& betterInvalidInput, Letter letter, std::string coords) {
   int row = coords[0] - CHAR_TO_INT_OFFSET; // hash define
   int col = -1;


   // Makes sure that the input is correct
   if (coords.length() > 1) {
      // this uses string to int and substrings to convert from string to int
      col = std::stoi(coords.substr(1, coords.length() - 1));
   }

   // If out of range, do not place the tile.
   if (!(row > DIM || col >= DIM || row < 0 || col <= 0)) {
      std::tuple<Letter, int, int> tile = std::make_tuple(letter, row, col);
      this->tileQueue.push_back(tile);
   }
   else {
      if (betterInvalidInput) {
         std::cout << "The given board location is beyond reality. Please try again..." << std::endl;
         std::cout << "The list of possible board locations are [A, O] -> [0, 15]." << std::endl;
      }
      else {
         std::cout << "Invalid input" << std::endl;
      }
   }
}

/**
 * @brief Places all tiles in tileQueue onto the board, ensuring placement is legal.
 * @return true Legal move.
 * @return false Illegal move.
 */
bool Board::placeAll(bool& betterInvalidInput, bool& dictionary, Player* player) {
   bool ret = false;
   if (this->tileQueue.empty() == false) {
      Letter letter = 'A';
      int row = std::get<1>(this->tileQueue.front());
      int col = std::get<2>(this->tileQueue.front());
      bool hor = true;
      bool ver = true;
      bool legal = true;
      bool isAWord = true;
      bool hasAllTiles = true;
      int countBefore = 0;
      int countAfter = 0;

      // if dictionary is enabled invoke character grid
      // else invoke int grid
      for (int i = 0; i < DIM; i++) {
         for (int j = 0; j < DIM; j++) {
            if (this->board[i][j] != nullptr) {
               this->board2[i][j] = 1;
               countBefore++;
               if (dictionary) {
                  this->board3[i][j] = this->board[i][j]->getLetter();
               }
            }
         }
      }

      // check if all tiles exist on same axis
      for (std::tuple<Letter, int, int> tile : this->tileQueue) {
         if (std::get<1>(tile) != row) {
            hor = false;
         }
         if (std::get<2>(tile) != col) {
            ver = false;
         }
         if (dictionary) {
            this->board3[std::get<1>(tile)][std::get<2>(tile)] = std::get<0>(tile);
         }
         this->board2[std::get<1>(tile)][std::get<2>(tile)] = 1;
         if (!player->getHand()->inList(std::get<0>(tile))) {
            hasAllTiles = false;
         }
      }

      // checks if tiles are placed in a connected line
      // this method is considerate of players using preexisting tiles to build their word
      std::string wordCheck = "";
      if (hor == true) {
         for (int i = col; i <= std::get<2>(this->tileQueue.back()); i++) {
            if (dictionary) {
               wordCheck.push_back(this->board3[row][i]);
            }
            if (this->board2[row][i] == 0) {
               legal = false;
            }
         }
      }
      if (ver == true) {
         for (int i = row; i <= std::get<1>(this->tileQueue.back()); i++) {
            if (dictionary) {
               wordCheck.push_back(this->board3[i][col]);
            }
            if (this->board2[i][col] == 0) {
               legal = false;
            }
         }
      }

      if (dictionary) {
         isAWord = this->wordCheck(wordCheck);
      }

      // debug
      // std::cout << hor << ver << legal << isAWord << hasAllTiles << std::endl;

      // penultimate condition check to determine move legality
      if ((hor == true || ver == true) && legal == true && isAWord == true && hasAllTiles == true) {
         // sweeper modifies countAfter using references
         // sweep takes (7, 7) as argument to enforce first turn must be in centre of board
         this->sweep(BOARD_CENTER, BOARD_CENTER, countAfter);
      }

      // if there exists at least one tile that cannot be reached from (7, 7)
      // it is an invalid move
      if (countBefore + (int)this->tileQueue.size() == countAfter) {
         ret = true;
         if (this->tileQueue.size() == BINGO) {
            std::cout << "BINGO!" << std::endl;
            player->addScore(50);
         }

         // execute tile placement
         while (this->tileQueue.empty() == false) {
            std::tie(letter, row, col) = this->tileQueue.front();
            Tile* tileToPlace = player->getHand()->removeLetter(letter);
            this->board[row][col] = tileToPlace;
            player->addScore(tileToPlace->getValue());
            this->tileQueue.pop_front();
            player->draw(this->tilebag);
         }
      }
      else {
         if (betterInvalidInput) {
            std::cout << "The given tiles and tile locations do not form a legally complete word on the board. Please try again..." << std::endl;
         }
         else {
            std::cout << "Invalid input" << std::endl;
         }
         this->tileQueue.clear();
      }
   }
   else {
      if (betterInvalidInput) {
         std::cout << "No tiles were queued for placement, please try again..." << std::endl;
      }
      else {
         std::cout << "Invalid input" << std::endl;
      }
   }
   return ret;
}

/**
 * @brief Gets string representation of tileBag.
 */
std::string Board::getTiles() {
   return this->tilebag->getNodeStrings();
}

/**
 * @brief Starting from row, col, counts adjacent tiles recursively.
 * @param row Current row.
 * @param col Current col.
 * @param count Value that is incremented when counting.
 */
void Board::sweep(int row, int col, int& count) {
   // precondition sweep is always invoked with row = 7 and col = 7

   // if sweeper is currently standing on a tile
   if (this->board2[row][col] == 1) {
      count++;
   }

   // to prevent backtracking
   this->board2[row][col] = 2;

   // check up
   if (row - 1 >= 0 && this->board2[row - 1][col] == 1) {
      sweep(row - 1, col, count);
   }

   // check right
   if (col + 1 < DIM && this->board2[row][col + 1] == 1) {
      sweep(row, col + 1, count);
   }

   // check down
   if (row + 1 < DIM && this->board2[row + 1][col] == 1) {
      sweep(row + 1, col, count);
   }

   // check left
   if (col - 1 >= 0 && this->board2[row][col - 1] == 1) {
      sweep(row, col - 1, count);
   }
}