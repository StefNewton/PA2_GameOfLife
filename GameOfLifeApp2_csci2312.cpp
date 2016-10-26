
/*
Name:					Stefani Moore
Last Date Modified:		10-14-2016
Assignment:				PA2
*/

//*******************************************************************
//GameOfLifeApp2_csci2312.cpp
//
//Contains main and Game of Life implentations 
//
//*******************************************************************


// *********************************************************************
//  This program demostrates the Game Of Life 
//  
// *********************************************************************


#include "GameOfLife2_csci2312.h" //header file

using std::cout;
using std::endl;
using std::cin;
using namespace csci2312;


// ======================
//     main function
// ======================
int main()

{
	
	////////// STUDENT TASK - go on with your menus etc
	int numberOfIterations; // to determine # of game iterations
	char cont = 'n'; 

	gameDescription(); //game and rule descriptions

	cout << "Would you like to play? (y/n)\n"; //Used to continue the game
	cin >> cont;

	while (cont == 'y' || cont == 'Y') //the game will continue looping until the user enters somethiing besides y or Y
	{
		size_t userBoardSize = 0;
		while(userBoardSize < 1) //User must enter a board size greater than 0
		{
			cout << "\nWhat board size would you like to use?: ";
			cin >> userBoardSize;
		}

		GameOfLife* myGame;

		// Moved try-catch block into the while loop so that the user can adjust the board size without
		//completely exiting the program

		// Construct the game anticipating memory allocation issues
		try {

			myGame = new GameOfLife(userBoardSize);
		}
		catch (std::bad_alloc) {
			cout << "Failed to allocate memory for a new game" << endl;

			cin.get();
			return(1);
		};

		std::cout << "\nHow many generations would you like to run?\n"
			<< "0 - for step-by-step \n"
			<< "Integer Value - for number of generations\n";
		cin >> numberOfIterations;

		//number of iterations used to determine which run member function is used
		if (numberOfIterations == 0)
			myGame->run(userBoardSize); //Step-by-step run function with user defined board size
		else if (numberOfIterations > 0)
		{
			myGame->run(numberOfIterations, userBoardSize); //run function with user defined generations and board size
		}
		else if (numberOfIterations < 0)
			std::cout << "Number must be greater than 0.\n";
		else
			std::cout << "Must be an integer value.\n";

		cout << "Play again? (y/n): ";
		cin >> cont;

		delete myGame;  // invokes GameOfLife destructor 
	}
    ///////// END TASK


	return 0;
}

//Used when user inputs '0' for number of iterations
//Goes step-by-step through each generation
bool continueGame()
{
	char cont;
	std::cout << "Continue? (y/n)\n";
	std::cin >> cont;

	if (cont == 'y' || cont == 'Y')
		return true;
	else
		return false;
}


//Provides the user with a full game description and goes over the two different rule sets
void gameDescription()
{

	std::cout << "\t\tJOHN CONWAY'S GAME OF LIFE\n"
		<< "This program attempts to replicate John Conway's Game of Life.\n\n"
		<< "\nGAME RULES 1:\n"
		<< "1. Alive cell with < 2 alive neighbours dies (under-population)\n"
		<< "2. Alive cell with 2 or 3 alive neighbours lives\n"
		<< "3. Alive cell with > 3 alive neighbours dies (over-population)\n"
		<< "4. Dead cell with 3 live neighbours becomes a live cell (reproduction)\n"
		<< "\nGAME RULES 2 - DAY & NIGHT:\n"
		<< "1. Dead cell is born if it has 3, 6, 7, or 8 live neighbors\n"
		<< "2. Alive cell stays alive if it has 3, 4, 6, 7, or 8 live neighbors\n"
		<< "3. For all other situations, cell state remains the same\n"
		<< "**The oscillator file best shows the symmetric nature of the Day & Night rules**\n\n";
}