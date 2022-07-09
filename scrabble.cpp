
#include "LinkedList.h"
#include "Board.h"
#include "Player.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <locale>
#include <algorithm>

using std::string;

#define EXIT_SUCCESS    0
#define FILE_LINES      23

#define NEW_GAME 1
#define LOAD_GAME 2
#define CREDITS 3
#define QUIT 4

void mainMenu();
int invalidMenu();

void startNewGame();
string invalidPlayerName();

void loadGame();
string invalidFileName();
std::vector<string> invalidFileSize();
std::vector<string> invalidFormat();

void game(Player* player1, Player* player2, Board* board, Player* playerTurn);

void credits();
void endGame();

int main(void) {
   LinkedList* list = new LinkedList();
   delete list;

   std::cout << "Welcome to Scrabble!\n-------------------\n" << std::endl;

   mainMenu();

   return EXIT_SUCCESS;
}

/**
 * Displays the main menu and takes input, allowing for selection of options.
 */
void mainMenu() {
   std::cout << "Menu\n----\n1. New Game\n2. Load Game\n3. Credits (Show student information)\n4. Quit\n\n> ";

   int input;

   std::cin >> input;

   // Ends game if EOF
   if (std::cin.eof()) {
      input = QUIT;
   }

   if (input > QUIT || input < NEW_GAME) {
      input = invalidMenu();
   }

   // For each of the potential inputs
   if (input == NEW_GAME) {
      startNewGame();
   }
   else if (input == LOAD_GAME) {
      loadGame();
   }
   else if (input == CREDITS) {
      credits();
      mainMenu();
   }
   else if (input == QUIT) {
      endGame();
   }

}

/**
 * Determines if wordToCheck is a valid word within the words file.
 * Returns true if it is found in file, false otherwise.
 * @param wordToCheck The word to be verified.
 */
bool verifyWord(string wordToCheck) {
   bool ret = false;
   //Have word for comparison
   string line;

   //Read in file for checking
   std::ifstream textFile;
   textFile.open("words");
   bool searching = true;

   //Loop through word file to find match
   if (textFile.is_open()) {
      while (searching) {
         std::getline(textFile, line);

         if (line == wordToCheck) {
            //If match found return true, else false
            textFile.close();
            searching = false;
            ret = true;
         }
      }
   }
   textFile.close();
   return ret;
}

/**
 * Requests menu input from user, and loops until valid value inputted.
 * Returns input as an Integer.
 */
int invalidMenu() {
   int input;
   std::cout << "Invalid input" << std::endl;
   std::cout << "> ";
   std::cin >> input;
   std::cout << std::endl;

   if (std::cin.eof()) {
      input = QUIT;
   }
   else {
      if (input > QUIT || input < NEW_GAME) {
         input = invalidMenu();
      }

   }
   return input;
}

/**
 * Gets input for player names and starts a new game, generating the board.
 * Runs game function with inputted values.
 */
void startNewGame() {
   string name1;
   string name2;
   std::cout << "\nStarting a New Game" << std::endl;
   std::cout << "Enter a name for player 1 (uppercase characters only)\n> ";
   std::cin >> name1;

   if (std::cin.eof()) {
      endGame();
   }
   else {
      if (!std::regex_match(name1, std::regex("^[A-Z]+$"))) {
         name1 = invalidPlayerName();
      }

      std::cout << "Enter a name for player 2 (uppercase characters only)\n> ";
      std::cin >> name2;
      if (std::cin.eof() || name1 == "end") {
         endGame();
      }
      else {
         if (!std::regex_match(name2, std::regex("^[A-Z]+$"))) {
            name2 = invalidPlayerName();
         }

         if (name2 != "end") {
            std::cout << "\nLet's Play!" << std::endl;
            std::cout << std::endl;

            Board* board = new Board();
            Player* player1 = new Player(name1);
            Player* player2 = new Player(name2);
            board->buildBag();
            board->buildHand(player1);
            board->buildHand(player2);

            Player* playerTurn = player1;

            game(player1, player2, board, playerTurn);
         }
      }
   }
}

/**
 * Requests player input from user, and loops until valid value inputted.
 * Returns input as a string.
 */
string invalidPlayerName() {
   string input;
   std::cout << "Invalid input" << std::endl;
   std::cout << "> ";
   std::cin >> input;
   if (std::cin.eof()) {
      input = "end";
   }
   else {
      std::cout << std::endl;
      if (!std::regex_match(input, std::regex("^[A-Z]+$"))) {
         input = invalidPlayerName();
      }
   }
   return input;

}

/**
 * Takes fileName from cin and loads from the file.
 * Validates file formatting before reading.
 * Runs game function with loaded values.
 */
void loadGame() {
   // Line count: 23
   std::cout << "Enter the filename from which load a game\n> ";
   string filename;
   std::cin >> filename;
   if (std::cin.eof()) {
      endGame();
   }
   else {
      // Check for file validity (exists and correct format)
      std::ifstream file(filename);
      if (!file) {
         file.close();
         filename = invalidFileName();
         file.open(filename);
      }

      if (filename != "end") {
         std::vector<string> lines;

         if (file.is_open()) {
            while (file) {
               string tmp;
               std::getline(file, tmp);

               // Just double checks that something isn't going wrong here
               // Some OSes may add \r to the end of lines
               if (!tmp.empty() && tmp[tmp.size() - 1] == '\r')
                  tmp.erase(tmp.size() - 1);

               lines.push_back(tmp);
            }
         }


         // check if file is formatted correctly
         /*
          * 23 lines total
          * lines 1, 4, 23 should be player names - can check if 1/4 == 23
          * lines 2, 5 should be numbers - check if making into number works
          * lines 3, 6, 22 should be tiles
          * lines 7-21 (inc) should be the board
          */

         if (lines.size() != FILE_LINES + 1) {
            lines = invalidFileSize();
         }

         if (!lines.empty()) {
            // Get vector just of the board
            std::vector<string> boardStrings;
            for (int i = 6; i < 21; ++i) {
               boardStrings.push_back(lines[i]);
            }

            bool names = (lines[0] == lines[22]) || (lines[3] == lines[22]);

            int nums = (std::isdigit(lines[1][0]))
               && (std::isdigit(lines[4][0]));

            bool tiles = (lines[2].find('-') != std::string::npos)
               && (lines[5].find('-') != std::string::npos)
               && (lines[21].find('-') != std::string::npos);

            bool boardBool = true;
            for (auto line : boardStrings) {
               boardBool = boardBool && (line.find('|') != std::string::npos);
            }

            if (!(names && nums && tiles && boardBool)) {
               lines = invalidFormat();
               if (!lines.empty()) {
                  std::vector<string> tmpBoardStrings;
                  for (int i = 6; i < 21; ++i) {
                     tmpBoardStrings.push_back(lines[i]);
                  }
                  boardStrings = tmpBoardStrings;
               }
            }

            if (!lines.empty()) {

               Player* player1 = new Player(lines[0], std::stoi(lines[1]), lines[2]);

               Player* player2 = new Player(lines[3], std::stoi(lines[4]), lines[5]);

               Board* board = new Board(lines[21], boardStrings);

               std::cout << "Game loaded successfully" << std::endl;

               game(player1, player2, board, player1->getName() == lines[22] ? player1 : player2);
            }
         }
      }
      file.close();
   }
}

/**
 * Requests fileName from user, and loops until file found matching name.
 * Returns input as a string.
 */
string invalidFileName() {
   string input;
   std::cout << "File does not exist." << std::endl;
   std::cout << "> ";
   std::cin >> input;
   if (std::cin.eof()) {
      input = "end";
      endGame();
   }
   else {
      std::cout << std::endl;
      std::ifstream file(input);
      if (!file) {
         input = invalidFileName();
      }
      file.close();
   }
   return input;
}

/**
 * Requests new file input from user when file size is incorrect.
 * Loops until file size is correct.
 * @return std::vector<string> Vector of lines of correctly sized file.
 */
std::vector<string> invalidFileSize() {
   string input;
   std::vector<string> lines;

   std::cout << "File is not formatted correctly." << std::endl;
   std::cout << "> ";
   std::cin >> input;
   std::cout << std::endl;
   if (std::cin.eof()) {
      endGame();
   }
   else {
      // Check for file validity (exists and correct format)
      std::ifstream file(input);
      if (!file) {
         input = invalidFileName();
      }

      if (file.is_open()) {
         while (file) {
            string tmp;
            std::getline(file, tmp);
            lines.push_back(tmp);
         }
      }
      file.close();

      if (lines.size() != FILE_LINES + 1) {
         lines = invalidFileSize();
      }
   }
   return lines;
}

/**
 * Requests new file input from user when file is formatted incorrectly.
 * Loops until file has correct formatting.
 * @return std::vector<string> Vector of lines of correctly formatted file.
 */
std::vector<string> invalidFormat() {
   std::vector<string> lines = invalidFileSize();

   if (!lines.empty()) {
      // Get vector just of the board
      std::vector<string> boardStrings;
      for (int i = 6; i < 21; ++i) {
         boardStrings.push_back(lines[i]);
      }

      bool names = (lines[0] == lines[22]) || (lines[3] == lines[22]);

      std::locale loc;
      bool nums = (isdigit(lines[1], loc)) && (isdigit(lines[4], loc));

      bool tiles = (lines[2].find('-') != std::string::npos) && (lines[5].find('-') != std::string::npos) && (lines[21].find('-') != std::string::npos);

      bool boardBool = true;
      for (auto line : boardStrings) {
         boardBool = boardBool && (line.find('|') != std::string::npos);
      }

      if (!(names && nums && tiles && boardBool)) {
         lines = invalidFormat();
      }
   }

   return lines;
}

/**
 * Runs a game of scrabble with both players and the inputted board as a starting point.
 * Game ends when tile bag is empty or a player passes their turn twice in a row.
 *
 * @param player1 The first player.
 * @param player2 The second player.
 * @param board The game's board object.
 * @param playerTurn Pointer to player whose turn it is.
 */
void game(Player* player1, Player* player2, Board* board, Player* playerTurn) {
   string opt;
   string input[4];
   std::stringstream stream;
   bool validMove;
   bool terminateGame = false;

   // bug fix: prevents skipping of player1 first turn
   std::getline(std::cin, opt);
   while (!std::cin.eof() && terminateGame == false) {
      validMove = false;
      if (board->getTiles() == "") {
         // tile bag is empty, terminate game
         terminateGame = true;
      }

      /*
       * Prints out board state in this format:
       * Player's turn
       * Scores
       * Board
       * Player's hand
       * Options
       */
       // Declares whose turn it is
      playerTurn->declareTurn();

      // Prints scores
      player1->printScore();
      player2->printScore();
      std::cout << std::endl;

      // Prints board
      board->print();
      std::cout << std::endl;

      // Prints out the hand of the current player
      std::cout << "Your hand is" << std::endl;
      playerTurn->printHand();
      std::cout << std::endl << std::endl;

      // Options
      std::cout << "Please choose an option:\nreplace\nplace\nsave\npass\n\n> ";

      // Gets input
      std::getline(std::cin, opt);
      if (!std::cin.eof()) {
         stream.str(opt);

         // input is a string array of size 4
         // where if you type in:
         //    "place A at A0"
         // then 
         //    input[0] = "place"
         //    input[1] = "A"
         //    input[2] = "at"
         //    input[3] = "A0"

         // in input[x][y], the [y] is the character index
         // e.g.
         //    input[0][0] = p
         //    input[0][1] = l
         //    input[0][2] = a
         //    input[0][3] = c
         //    input[0][4] = e

         for (int i = 0; i < 4; i++) {
            stream >> input[i];
         }

         // I moved this up here because the player is able to take action if they so choose afterwards
         if (input[0] == "save") {
            /*
             * FORMAT:
               <player 1 name>
               <player 1 score>
               <player 1 hand>
               <player 2 name>
               <player 2 score>
               <player 2 hand>
               <Board State>
               <tile bag contents>
               <current player name>
             */
            string filename = input[1];
            std::ofstream myfile;

            myfile.open(filename);

            myfile << player1->getName() << "\n" << player1->getScore() << "\n" << player1->getTiles() << "\n"
               << player2->getName() << "\n" << player2->getScore() << "\n" << player2->getTiles() << "\n"
               << board->getBoard() << board->getTiles() << "\n" << playerTurn->getName();

            myfile.close();

            std::cout << "\nGame saved successfully" << std::endl;

            std::cout << "\n> ";
            std::getline(std::cin, opt);
            stream.str(opt);
            for (int i = 0; i < 4; i++) {
               stream >> input[i];
            }
         }

         if (input[0] == "replace") {
            validMove = board->replaceWithTileFromBag(playerTurn, input[1][0]);
         }
         else if (input[0] == "place" && input[2] == "at") {
            // input[3][0]-65
            // input[3][1]-48
            // these are literal int conversions from ascii characters

            board->placeBuffer(input[1][0], input[3]);
            stream.clear();
            bool cont = true;
            while (cont) {
               std::cout << "> ";
               std::getline(std::cin, opt);
               if (!std::cin.eof()) {
                  if (opt != "place Done") {
                     stream.str(opt);
                     for (int i = 0; i < 4; i++) {
                        stream >> input[i];
                     }
                     if (input[0] == "place" && input[2] == "at") {
                        board->placeBuffer(input[1][0], input[3]);
                     }
                     else {
                        std::cout << "Invalid command, try again..." << std::endl;
                     }
                     stream.clear();
                  }
                  else {
                     cont = false;
                  }
               }
               else {
                  cont = false;
                  terminateGame = true;
               }

            }
            if (!terminateGame)
               validMove = board->placeAll(playerTurn);
         }
         else if ((input[0] == "pass") && (playerTurn->getPass() < 1)) {
            std::cout << "Passing turn..." << std::endl;
            playerTurn->incrementPass();
            validMove = true;
         }
         else if ((input[0] == "pass") && (playerTurn->getPass() >= 1)) {
            std::cout << "You cannot pass your turn twice in a row." << std::endl;
            terminateGame = true;
         }
         else {
            std::cout << "Option error, try again..." << std::endl;
         }
         std::cout << std::endl;
         stream.clear();

         // transfer initiative to other player (relative)
         if (validMove == true) {
            playerTurn = (playerTurn == player1) ? player2 : player1;
         }
      }
      // termination case if terminateGame == true
   }

   std::cout << "Game over" << std::endl;
   player1->printScore();
   player2->printScore();
   if (player2->getScore() > player1->getScore()) {
      std::cout << "Player " << player2->getName() << " won!" << std::endl;
   }
   else {
      std::cout << "Player " << player1->getName() << " won!" << std::endl;
   }

   delete board;
   delete player1;
   delete player2;
}

/**
 * Prints credits of game.
 */
void credits() {
   std::cout << "----------------------------------";
   const int memberLen = 4;
   string members[memberLen][2] = { {"Alli Toth", "s3902333"}, {"Carter Facey-Smith", "s3907874"}, {"Antoni Giannakopoulos", "s3895923"}, {"Kelvin Young", "s3899733"} };
   for (int i = 0; i < memberLen; ++i) {
      std::cout << "\nName: " << members[i][0] << "\nStudent ID: " << members[i][1] << "\nEmail: " << members[i][1] << "@student.rmit.edu.au" << std::endl;
   }
   std::cout << "----------------------------------";
   std::cout << "\n\n";
}

/**
 * Says goodbye when game is ended.
 */
void endGame() {
   std::cout << "Goodbye" << std::endl;
}
