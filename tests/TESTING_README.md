## Test Cases and Coverage

The testing files for Scrabble are run using the following syntax:

1. Generating test output files:

		./scrabble < testname.input > testname.gameout

2. Comparing generated output file to expected output file:
	
		diff -w testname.output testname.gameout

3. Comparing an expected save file to a generated save:

		if [-e testname>.expsave] diff -w -y testname.expsave actual_gamesave
		
#### Test cases covered:
1. Loading in a bad file format;
2. Improper loading in of file generally;
3. Saving of game state;
4. Placing tiles;
5. Generating new game state;
6. Showing credits;
7. Quitting the game;
8. Placing a user's entire hand for a bingo;
9. Placing a tile out of the bounds of the board;
10. Placing a tile not in line with the other placed letters;
11. Passing twice in a row; and
12. General game flow and logic of play for two turns.

#### Individual tests
1. 3player test must be run with:
./scrabble --3player <tests/3player.input >tests/3player.gameout

2. 4player test must be run with:
./scrabble --4player <tests/4player.input >tests/4player.gameout

3. betterInvalidInput test must be run with:
./scrabble --betterInvalidInput <tests/betterInvalidInput.input >tests/betterInvalidInput.gameout

4. dictionary test must be run with:
./scrabble --dictionary <tests/dictionary.input >tests/dictionary.gameout
