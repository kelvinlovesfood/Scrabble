
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

#define NEW_GAME 1
#define LOAD_GAME 2
#define CREDITS 3
#define QUIT 4

void mainMenu(bool& betterInvalidInput, bool& dictionary, int& playerCount);
int invalidMenu(bool& betterInvalidInput);
void startNewGame(bool& betterInvalidInput, bool& dictionary, int& playerCount);
void loadGame(bool& betterInvalidInput, bool& dictionary, int& playerCount);
void game(bool& betterInvalidInput, bool& dictionary, int& playerCount, Board* board, Player** players, Player* playerTurn);
void credits();
void endGame();

string invalidPlayerName(bool& betterInvalidInput);
string invalidFileName(bool& betterInvalidInput);
std::vector<string> invalidFileSize(bool& betterInvalidInput, int& fileSize);
std::vector<string> invalidFormat(bool& betterInvalidInput, int& playerCount, int& fileSize);

int main(int argc, char** argv) {
   bool betterInvalidInput = false;
   bool dictionary = false;
   int playerCount = 2; 
   for (int i=1; i<argc; ++i) {
      if ((std::string)argv[i] == "--betterInvalidInput") {
         betterInvalidInput = true;
         std::cout << "Better invalid input mode enabled." << std::endl;
      }
      if ((std::string)argv[i] == "--dictionary") {
         dictionary = true;
         std::cout << "Ditionary mode enabled." << std::endl;
      }
      if ((std::string)argv[i] == "--3player") {
         playerCount = 3;
         std::cout << "Three player mode enabled." << std::endl;
      }
      if ((std::string)argv[i] == "--4player") {
         playerCount = 4;
         std::cout << "Four player mode enabled." << std::endl;
      }  
   }

   LinkedList* list = new LinkedList();
   delete list;

   std::cout << "\nWelcome to Scrabble!\n-------------------\n" << std::endl;
   mainMenu(betterInvalidInput, dictionary, playerCount);
   return EXIT_SUCCESS;
}

/**
 * Displays the main menu and takes input, allowing for selection of options.
 */
void mainMenu(bool& betterInvalidInput, bool& dictionary, int& playerCount) {
   std::cout << "Menu\n----\n1. New Game\n2. Load Game\n3. Credits (Show student information)\n4. Quit\n\n> ";
   int input;
   std::cin >> input;
   // Ends game if EOF
   if (std::cin.eof()) {
      input = QUIT;
   }
   if (input > QUIT || input < NEW_GAME) {
      input = invalidMenu(betterInvalidInput);
   }
   // For each of the potential inputs
   if (input == NEW_GAME) {
      startNewGame(betterInvalidInput, dictionary, playerCount);
   }
   else if (input == LOAD_GAME) {
      loadGame(betterInvalidInput, dictionary, playerCount);
   }
   else if (input == CREDITS) {
      credits();
      mainMenu(betterInvalidInput, dictionary, playerCount);
   }
   else if (input == QUIT) {
      endGame();
   }
}

/**
 * Requests menu input from user, and loops until valid value inputted.
 * Returns input as an Integer.
 */
int invalidMenu(bool& betterInvalidInput) {
   int input;
   if (betterInvalidInput) {
      std::cout << "The given input does not match any of the accepted input options [0, 4], please try again..." << std::endl;
   }
   else {
      std::cout << "Invalid input" << std::endl;
   }
   std::cout << std::endl;
   std::cout << "> ";
   std::cin >> input;
   std::cout << std::endl;
   if (std::cin.eof()) {
      input = QUIT;
   }
   else {
      if (input > QUIT || input < NEW_GAME) {
         input = invalidMenu(betterInvalidInput);
      }
   }
   return input;
}

/**
 * Gets input for player names and starts a new game, generating the board.
 * Runs game function with inputted values.
 */
void startNewGame(bool& betterInvalidInput, bool& dictionary, int& playerCount) {
   std::cout << "\nStarting a New Game" << std::endl;
   string playerName = "";
   Player* players[playerCount];
   Board* board = new Board();
   board->buildBag();

   for (int i=1; i<=playerCount && playerName != "end"; ++i) {
      std::cout << "Enter a name for player " << i << " (uppercase characters only)\n> ";
      std::cin >> playerName;
      if (std::cin.eof()) {
         playerName = "end";
         endGame();
      }
      else {
         if (!std::regex_match(playerName, std::regex("^[A-Z]+$"))) {
            playerName = invalidPlayerName(betterInvalidInput);
         }
         Player* playerObject = new Player(playerName);
         players[i-1] = playerObject;
         board->buildHand(playerObject);
      }
   }

   std::cout << "\nLet's Play!" << std::endl;
   std::cout << std::endl;

   game(betterInvalidInput, dictionary, playerCount, board, players, players[0]);
}

/**
 * Requests player input from user, and loops until valid value inputted.
 * Returns input as a string.
 */
string invalidPlayerName(bool& betterInvalidInput) {
   string input;
   if (betterInvalidInput) {
      std::cout << "The given player name is unacceptable. Please try again." << std::endl;
      std::cout << "A name must only use uppercase latin charcacters; no arabic numerals or symbols." << std::endl;
   }
   else {
      std::cout << "Invalid input" << std::endl;
   }
   std::cout << std::endl;
   std::cout << "> ";
   std::cin >> input;
   if (std::cin.eof()) {
      input = "end";
   }
   else {
      std::cout << std::endl;
      if (!std::regex_match(input, std::regex("^[A-Z]+$"))) {
         input = invalidPlayerName(betterInvalidInput);
      }
   }
   return input;

}

/**
 * Takes fileName from cin and loads from the file.
 * Validates file formatting before reading.
 * Runs game function with loaded values.
 */
void loadGame(bool& betterInvalidInput, bool& dictionary, int& playerCount) {
   int fileSize = 17+(playerCount*3);
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
         filename = invalidFileName(betterInvalidInput);
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
         // RETCON file format is now relative to playerCount

         if ((int)lines.size() != (fileSize + 1)) {
            lines = invalidFileSize(betterInvalidInput, fileSize);
         }
         if (!lines.empty()) {
            // Get vector just of the board
            std::vector<string> boardStrings;
            for (int i = (3*playerCount); i < 15+(3*playerCount); ++i) {
               boardStrings.push_back(lines[i]);
            }
            
            bool names = false;
            for (int i=0; i<playerCount; ++i) {
               if (lines[3*i] == lines[16+(3*playerCount)]) {
                  names = true;
               }
            }

            bool nums = true;
            for (int i=0; i<playerCount; ++i) {
               if (!std::isdigit(lines[1+(3*i)][0])) {
                  names = false;
               }
            }

            bool tiles = true;
            for (int i=0; i<playerCount; ++i) {
               if (!lines[2+(3*i)].find('-')) {
                  tiles = false;
               }
            }
            if (!lines[15+(3*playerCount)].find('-')) {
               tiles = false;
            }

            bool boardBool = true;
            for (auto line : boardStrings) {
               boardBool = boardBool && (line.find('|') != std::string::npos);
            }

            if (!(names && nums && tiles && boardBool)) {
               lines = invalidFormat(betterInvalidInput, playerCount, fileSize);
               if (!lines.empty()) {
                  std::vector<string> tmpBoardStrings;
                  for (int i = (3*playerCount); i < 15+(3*playerCount); ++i) {
                     tmpBoardStrings.push_back(lines[i]);
                  }
                  boardStrings = tmpBoardStrings;
               }
            }
            if (!lines.empty()) {
               Player* players[playerCount];
               Player* playerTurn = nullptr;
               for (int i=0; i<playerCount; ++i) {
                  Player* playerObject = new Player(lines[0+(3*i)], std::stoi(lines[1+(3*i)]), lines[2+(3*i)]);
                  players[i] = playerObject;
                  if (playerObject->getName() == lines[16+(3*playerCount)]) {
                     playerTurn = playerObject;
                  }
               }
               Board* board = new Board(lines[15+(3*playerCount)], boardStrings);
               std::cout << "Game loaded successfully" << std::endl;
               game(betterInvalidInput, dictionary, playerCount, board, players, playerTurn);
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
string invalidFileName(bool& betterInvalidInput) {
   string input;
   if (betterInvalidInput) {
      std::cout << "The given filename does not exist. Please try again..." << std::endl;
   }
   else {
      std::cout << "Invalid input" << std::endl;
   }
   std::cout << std::endl;
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
         input = invalidFileName(betterInvalidInput);
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
std::vector<string> invalidFileSize(bool& betterInvalidInput, int& fileSize) {
   string input;
   std::vector<string> lines;
   if (betterInvalidInput) {
      std::cout << "File is not formatted correctly. Please try again..." << std::endl;
   }
   else {
      std::cout << "Invalid input" << std::endl;
   }
   std::cout << std::endl;
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
         input = invalidFileName(betterInvalidInput);
      }

      if (file.is_open()) {
         while (file) {
            string tmp;
            std::getline(file, tmp);
            lines.push_back(tmp);
         }
      }
      file.close();

      if ((int)lines.size() != (fileSize + 1)) {
         lines = invalidFileSize(betterInvalidInput, fileSize);
      }
   }
   return lines;
}

/**
 * Requests new file input from user when file is formatted incorrectly.
 * Loops until file has correct formatting.
 * @return std::vector<string> Vector of lines of correctly formatted file.
 */
std::vector<string> invalidFormat(bool& betterInvalidInput, int& playerCount, int& fileSize) {
   std::vector<string> lines = invalidFileSize(betterInvalidInput, fileSize);

   if (!lines.empty()) {
      // Get vector just of the board
      std::vector<string> boardStrings;
      for (int i = (3*playerCount); i < 15+(3*playerCount); ++i) {
         boardStrings.push_back(lines[i]);
      }
      
      bool names = false;
      for (int i=0; i<playerCount; ++i) {
         if (lines[3*i] == lines[16+(3*playerCount)]) {
            names = true;
         }
      }

      bool nums = true;
      for (int i=0; i<playerCount; ++i) {
         if (!std::isdigit(lines[1+(3*i)][0])) {
            names = false;
         }
      }

      bool tiles = true;
      for (int i=0; i<playerCount; ++i) {
         if (!lines[2+(3*i)].find('-')) {
            tiles = false;
         }
      }
      if (!lines[15+(3*playerCount)].find('-')) {
         tiles = false;
      }

      bool boardBool = true;
      for (auto line : boardStrings) {
         boardBool = boardBool && (line.find('|') != std::string::npos);
      }

      if (!(names && nums && tiles && boardBool)) {
         lines = invalidFormat(betterInvalidInput, playerCount, fileSize);
      }
   }

   return lines;
}

/**
 * Runs a game of scrabble with both players and the inputted board as a starting point.
 * Game ends when tile bag is empty or a player passes their turn twice in a row.
 *
 * @param playerCount Number of players
 * @param players Array of pointers to player objects
 * @param board The game's board object.
 * @param playerTurn Pointer to player whose turn it is.
 */
void game(bool& betterInvalidInput, bool& dictionary, int& playerCount, Board* board, Player** players, Player* playerTurn) {
   string opt;
   string input[4];
   std::stringstream stream;
   bool validMove = false;
   bool terminateGame = false;
   if (dictionary) {
      board->buildDict();
   }

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
       * Players' turn
       * Scores
       * Board
       * Player's hand
       * Options
       */
       // Declares whose turn it is
      playerTurn->declareTurn();

      // Prints scores
      for (int i=0; i<playerCount; ++i) {
         players[i]->printScore();
      }
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
            // RETCON format is now relative to playerCount
            string filename = input[1];
            std::ofstream myfile;

            myfile.open(filename);

            for (int i=0; i<playerCount; ++i) {
               myfile << players[i]->getName() << "\n" << players[i]->getScore() << "\n" << players[i]->getTiles() << "\n";
            }
            myfile << board->getBoard() << board->getTiles() << "\n" << playerTurn->getName();

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
            if (std::regex_match(input[1], std::regex("^[A-Z]+$"))) {
               validMove = board->replaceWithTileFromBag(betterInvalidInput, playerTurn, input[1][0]);
            }
            else {
               if (betterInvalidInput) {
                  std::cout << "The given replace command has typos or carries invalid syntax; please try again..." << std::endl;
                  std::cout << "The syntax should follow this example: \"replace <letter in hand>\"" << std::endl;
               }
               else {
                  std::cout << "Invalid input" << std::endl;
               }
               validMove = false;
            }
         }
         else if (input[0] == "place" && input[2] == "at") {
            // input[3][0]-65
            // input[3][1]-48
            // these are literal int conversions from ascii characters

            board->placeBuffer(betterInvalidInput, input[1][0], input[3]);
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
                        board->placeBuffer(betterInvalidInput, input[1][0], input[3]);
                     }
                     else {
                        if (betterInvalidInput) {
                           std::cout << "The given place command has typos or carries invalid syntax, please try again..." << std::endl;
                           std::cout << "The syntax should follow this example: \"place A at H7\"" << std::endl;
                           std::cout << std::endl;
                        }
                        else {
                           std::cout << "Invalid input" << std::endl;
                        }
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
               validMove = board->placeAll(betterInvalidInput, dictionary, playerTurn);
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
            if (betterInvalidInput) {
               std::cout << "The given input does not match any of the accepted input options, please try again..." << std::endl;
            }
            else {
               std::cout << "Invalid input" << std::endl;
            }
         }
         std::cout << std::endl;
         stream.clear();

         // transfer initiative to other player (relative)
         // player initiative transfer module updated for an arbitrary number of players
         if (validMove == true) {
            bool ret = false;
            for (int i=0; i<playerCount && ret == false; ++i) {
               if (playerTurn == players[playerCount-1]) {
                  playerTurn = players[0];
                  ret = true;
               }
               else if (playerTurn == players[i]) {
                  playerTurn = players[i+1];
                  ret = true;
               }
            }
         }
      }
      // termination case if terminateGame == true
   }

   std::cout << "Game over" << std::endl;
   for (int i=0; i<playerCount; ++i) {
      players[i]->printScore();
      delete players[i];
   }

   delete board;

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
