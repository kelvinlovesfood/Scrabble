
#include "LinkedList.h"
#include "Board.h"
#include "Player.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <algorithm>
#include <vector>

using std::string;

// i hope this file isnt gonna be submitted...

void wordListToVect() {
   string line;
   int pos = 0;
   string textArray[2];
   std::vector<string> wordVect(textArray, textArray + 10);

   //Open file
   std::ifstream textFile;
   textFile.open("words");

   //Add each word encounteredd into the text array
   if (textFile.is_open()) {
      while (!textFile.eof()) {
         getline(textFile, line);
         wordVect.push_back(line);
         std::cout << wordVect.at(pos) << std::endl;
         break;
         pos++;
      }
      textFile.close();
   }
   else std::cout << "Can't open text file." << std::endl;

   //Returns a pointer to the text array
   //return textArray;
}

// FUNCTIONAL - TESTING COMPLETE
// string* wordListToArray(){
//    string line;
//    int pos = 0;
//    string* textArray = new string[TEXT_SIZE];

//    //Open file
//    std::ifstream textFile;
//    textFile.open("words");

//    //Add each word encounteredd into the text array
//    if (textFile.is_open())
//    {
//       while (!textFile.eof()){
//          getline(textFile,line);
//          textArray[pos] = line;
//          //Printing to test array intake
//          //std::cout << textArray[pos] << std::endl;
//          pos++;
//       }
//     textFile.close();
//    }
//    else std::cout << "Can't open text file." << std::endl;

//    //Returns a pointer to the text array
//    return textArray;
// }

int main() {

   // a pointer to an int.
   //wordListToVect();
   //string* p = wordListToArray();
   return 0;
}
