/*
Name:					Stefani Moore
Last Date Modified:		10-14-2016
Assignment:				PA2
*/

//*******************************************************************
//GameOfLife2_csci2312.cpp
//
//Contains GameOfLife2_csci2312 class implentations 
//
//*******************************************************************



#include "GameOfLife2_csci2312.h" //header file

using std::cout;
using std::endl;

namespace csci2312

{
	/****************************************************************
	*
	* CLASS CELL MEMBER FUNCTION IMPLEMENTATIONS
	*
	*****************************************************************/
	//Cell constructor sets face = dead
	Cell::Cell() {
		face = dead; //initializes the cell face
	};

	//Cell constructor can be used to initialize state to cellState
	Cell::Cell(bool cellState) {
		state = cellState; //initializes cell state
	}

	//Cell destructor
	Cell::~Cell()
	{
	};

	//returns state
	bool Cell::getState() const {
		return state;
	}

	//Set state = newState
	void Cell::setState(bool newState) {
		state = newState;
	}

	//face = dead or alive based of the status of getState()
	char Cell::getFace() const {
		if (getState() == 1) //face determined by current cell state
			return alive;
		else
			return dead;
	}

	/****************************************************************
	*
	* CLASS GAMEOFLIFE MEMBER FUNCTION IMPLEMENTATIONS
	*
	*****************************************************************/
	//GameOfLife(size_t myBoardSize) initializes a board of dead cell of size myBoardSize*myBoardSize
	GameOfLife::GameOfLife(size_t myBoardSize) throw (std::bad_alloc) {

		boardSize = myBoardSize; //Initialize boardSize

		//Initialize currentLife board
		currentLife = new CellPtr[boardSize];
		for (size_t i = 0; i < boardSize; i++)
		{
			currentLife[i] = new Cell[boardSize];
		}

		for (size_t row = 0; row < boardSize; row++)
		{
			for (size_t col = 0; col < boardSize; col++)
			{
				currentLife[row][col].setState(0);
			}
		}

		//Initialize fileInput character board
		fileInput = new char*[boardSize];
		for (size_t i = 0; i < boardSize; i++)
		{
			fileInput[i] = new char[boardSize];
		}

		//Initialize nextLife board
		nextLife = new CellPtr[boardSize];
		for (size_t i = 0; i < boardSize; i++)
		{
			nextLife[i] = new Cell[boardSize];
		}

		for (size_t row = 0; row < boardSize; row++)
		{
			for (size_t col = 0; col < boardSize; col++)
			{
				nextLife[row][col].setState(0);
			}
		}

		

		// To test the exception, then remove (Tested and it works)
		// throw (std::bad_alloc());
	}

	//GameOfLife(const GameOfLife & gameToCopy) is a copy constructor
	GameOfLife::GameOfLife(const GameOfLife & gameToCopy) {
		//copy constructor

		//boardSize is not a pointer so we can shallow copy it
		boardSize = gameToCopy.boardSize;

		//currentLife is a pointer so it needs to be deep copied as long as it is not null
		if (gameToCopy.currentLife)
		{
			//Allocate memory for the copy
			currentLife = new CellPtr[gameToCopy.boardSize*gameToCopy.boardSize];

			//create a memory copy of the current life board
			memcpy(currentLife, gameToCopy.currentLife, boardSize*boardSize);
		}
		else
			currentLife = nullptr;

	}

	//Destructor
	GameOfLife::~GameOfLife() {

		for (size_t i = 0; i < boardSize; i++)
		{
			delete[] currentLife[i];
			delete[] nextLife[i];
			delete[] fileInput[i];
		}

		//delete currentLife / nextLife / fileInput
		delete[]currentLife;
		delete[]nextLife;
		delete[]fileInput;

		cout << "\nGame object desctructed \n";
	};

	//returns boardSize
	size_t GameOfLife::getBoardSize() const {

		return boardSize; //returns current boardSize
	}

	//seedBoard(string fileName) seeds the board from a .txt file
	//implements the istream overload operator
	void GameOfLife::seedBoard(string fileName, size_t sizeBoard) throw (FileIOException) {

		//Takes fileName and reads the data from the file
		string dataFile = fileName;
		std::ifstream readData(fileName);

		GameOfLife* game = new GameOfLife(sizeBoard); //allocate memory for a new game board

		if (readData.is_open()) //Error check file opening
		{
			while (!readData.eof()) //Continue reading until eof
			{
				readData >> *game; //read in file data to dereferenced GameOfLife object

				//Uses file characters and sets currentLife board to 1 or 0 based on alive or dead characters
				for (size_t row = 0; row < game->getBoardSize(); row++)
				{
					for (size_t col = 0; col < game->getBoardSize(); col++)
					{
						if (game->fileInput[row][col] == 'o')
						{
							currentLife[row][col].setState(1);
						}
						else
						{
							currentLife[row][col].setState(0);
						}
					}
				}
			}
		}
		else //Unable to open the file
		{
			cout << "Error opening file.\n";
			std::cin.get();
		}

		readData.close(); //close the file
	}

	//seedBoard(size_t seeds) generates a random board with seeds number of live cells determined by the user
	void GameOfLife::seedBoard(size_t seeds) {
		
		size_t row, col;
		for (size_t i = 1; i <= seeds; i++) //Creates a user defined number of random seeds
		{
			row = 1 + rand() % boardSize - 1; //Creates a random row and col locations for random seed
			col = 1 + rand() % boardSize - 1;

			currentLife[row][col].setState(true);
		}
	}

	//seedBoard() allows the user to input a custom board. This was only added to test the implementation of
	//the istream overload operator. It is a bit buggy and requires the exact # of cells that the user put in
	//otherwise it will crash the program
	void GameOfLife::seedBoard() {

		size_t sizeB = 0;

		do
		{
			cout << "How many characters would you like to enter?\n"
				<< "The # of chars. will create a board size \n"
				<< "i.e. input a 4 and it will be a boardsize of 4x4 or 16 char's\n"
				<< "Your board must be smaller than the board size entered previously\n"
				<< "Enter a #: ";

			std::cin >> sizeB; //user determines the amount of cells they would like to enter
		} while (sizeB < 0 || sizeB >= 30); //They will be asked on a loop until a valid number is entered between 0-30

		if (sizeB != 0) //If the user enters 0 the following code will be skipped as it will take in no input
		{
			GameOfLife* game = new GameOfLife(sizeB); //allocate memory for user input board

			cout << "Input " << sizeB*sizeB << " characters of either '-' or 'o'\n"
				<< "Any other character will result in a dead cell\n"
				<<"Inputting more than " << sizeB*sizeB << " will crash the program\n";
			
			std::cin >> *game; //the user inputs the characters, but I could not figure out a way to check how many they are putting in
								//so the exact amount must be put in or there will be a program crash

			//The code below sets the state of currentLife based on the characters input by the user
			for (size_t row = 0; row < game->getBoardSize(); row++)
			{
				for (size_t col = 0; col < game->getBoardSize(); col++)
				{
					if (game->fileInput[row][col] == 'o')
					{
						currentLife[row][col].setState(1);
					}
					else
					{
						currentLife[row][col].setState(0);
					}
				}
			}
		}
	}

	//This is where the game rules are implemented. It displays a main menu and determines which rule set will be
	//used. run() goes step-by-step through each generation 
	void GameOfLife::run(size_t sizeBoard) {

		GameOfLife game = GameOfLife(sizeBoard);
		char choice = '6';
		size_t numOfSeeds = 0;
		int ruleSet = 0;
		bool invalidEntry = false; 
		string fileName = "glider.txt"; //fileName initialized to glider.txt
		std::ifstream testFile;

		//Main menu for user
		std::cout << "\nMAIN MENU: \n"
			<< "1. Load glider example, has to be 30x30 board size\n"
			<< "2. Load oscillator example, has to be 30x30 board size\n"
			<< "3. Generate random see file\n"
			<< "4. Input a file\n"
			<< "5. Enter a custom board\n"
			<< "6. Exit Menu\n";
		std::cin >> choice;

		switch (choice)
		{
		case '1': //Glider example
			game.seedBoard(fileName, game.getBoardSize());
			break;
		case '2': //Oscillator example
			game.seedBoard("oscillator.txt", game.getBoardSize());
			break;
		case '3': //Randomly seeded file
			cout << "Enter the number of seeds to generate: "; //User inputs a number of seeds
			std::cin >> numOfSeeds;
			if (numOfSeeds >= game.getBoardSize()*game.getBoardSize()) //error checks for valid input of <= 900
			{
				cout << "Amount not valid. Must be less than " << game.getBoardSize()*game.getBoardSize() << ".\n";
				invalidEntry = true; //user input invalid
			}
			else
			{
				game.seedBoard(numOfSeeds);
				invalidEntry = false; //user input valid
			}
			break;
		case '4': //User input file
			cout << "Input the file that you would like to use (ex. file.txt): ";
			std::cin >> fileName;
			testFile.open(fileName);
			if (testFile.is_open())
			{
				game.seedBoard(fileName, game.getBoardSize());
				invalidEntry = false; //valid entry input by user
			}
			else
			{
				cout << "Error opening file.\n";
				invalidEntry = true; //invalid entry by user or bad file
			}
			break;
		case '5': //user input board from keyboard
			game.seedBoard();
			break;
		case '6': //exit program
			std::cout << "Thanks for playing.\n";
			break;
		default: //1-6 was not entered
			std::cout << "\nInvalid Entry.\n";
			invalidEntry = true;
			break;
		}

		// Only perform game of life calculations if a valid entry is input
		if ((choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5') && invalidEntry == false)
		{
			cout << "Rule set 1 or 2?: "; //User determines which rule set to use
			std::cin >> ruleSet;
			if (ruleSet == 1) //Standard game rule set
			{
				cout << game << endl;  //display current board

				while (continueGame() == true) //loop while user wants to continue
				{
					game.calculateNextGen(game.currentLife, game.nextLife); //calculates next generation

					std::cout << game << std::endl; //display game

					for (size_t r = 0; r < boardSize; r++)
					{
						for (size_t c = 0; c < boardSize; c++)
						{
							game.currentLife[r][c].setState(game.nextLife[r][c].getState()); //Sets next generation to current generation
						}
					}
				}
			}
			else if (ruleSet == 2) // Day or Night rule set
			{
				cout << game << endl; // display game

				while (continueGame() == true)  //loop while user wants to continue
				{
					game.calculateNextGenXXX(game.currentLife, game.nextLife); //Calculates next generation based on Day & Night rule set

					std::cout << game << std::endl; //display game

					for (size_t r = 0; r < boardSize; r++)
					{
						for (size_t c = 0; c < boardSize; c++)
						{
							game.currentLife[r][c].setState(game.nextLife[r][c].getState()); //Sets next generation to current generation
						}
					}
				}
			}
			else 
				cout << "\nInvalid entry.\n"; //used if invalid random seed amount was entered
		}
	}

	//executeRule110 implements the standard game of life rules to determine the next state
	bool GameOfLife::executeRule110(unsigned int countAlive, bool currentState) {

		if (currentState == true && countAlive < 2)
		{
			return false;
		}
		else if (currentState == true && (countAlive == 2 || countAlive == 3))
		{
			return true;
		}
		else if (currentState == true && countAlive > 3)
		{
			return false;
		}
		else if (currentState == false && countAlive == 3)
		{
			return true;
		}
		else
		{
			return false;
		}

	}

	//executeRuleXXX implements the Day & Night rule set. It is known for its symmetric nature.
	bool GameOfLife::executeRuleXXX(unsigned int countAlive, bool currentState) {

		//meaning that a dead cell becomes live (is born) if it has 3, 6, 7, or 8 live neighbors, and a live cell remains alive (survives) if it has 3, 4, 6, 7, or 8 live neighbors
		if (currentState == false && (countAlive == 3 || countAlive == 6 || countAlive == 7 || countAlive == 8))
		{
			return true;
		}
		else if (currentState == true && (countAlive == 3 || countAlive == 4 || countAlive == 6 || countAlive == 7 || countAlive == 8))
		{
			return true;
		}
		else
		{
			return currentState;
		}
	}

	//calculateNextGen applies executeRule110 and sets the nextLife based on the standard rules
	void GameOfLife::calculateNextGen(CellPtr* current, CellPtr* next) {

		Cell temp;

		for (size_t row = 1; row < boardSize - 1; row++)
		{
			for (size_t col = 1; col < boardSize - 1; col++)
			{
				int liveCount = 0;
				if (current[row - 1][col - 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row - 1][col].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row - 1][col + 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row][col - 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row][col + 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row + 1][col - 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row + 1][col + 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row + 1][col].getFace() == temp.alive)
				{
					liveCount++;
				}

				int num = liveCount;

				bool nextState = executeRule110(num, currentLife[row][col].getState());
				next[row][col].setState(nextState);
			}
		}
	}

	//I added calculateNextGenXXX to apply the day & night rule set so that it can be used on 
	//any seeded board the user chooses in the main menu
	void GameOfLife::calculateNextGenXXX(CellPtr* current, CellPtr* next) {

		Cell temp;

		for (size_t row = 1; row < boardSize - 1; row++)
		{
			for (size_t col = 1; col < boardSize - 1; col++)
			{
				int liveCount = 0;
				if (current[row - 1][col - 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row - 1][col].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row - 1][col + 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row][col - 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row][col + 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row + 1][col - 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row + 1][col + 1].getFace() == temp.alive)
				{
					liveCount++;
				}
				if (current[row + 1][col].getFace() == temp.alive)
				{
					liveCount++;
				}

				int num = liveCount;

				bool nextState = executeRuleXXX(num, currentLife[row][col].getState());
				next[row][col].setState(nextState);
			}
		}
	}

	//This is a second option for game rule implementation. It also displays a main menu, but this run function
	//displays numberOfIterations # of generations in 1 second intervals rather than step-by-step
	void GameOfLife::run(unsigned int numberOfIterations, size_t sizeBoard) {
		GameOfLife game = GameOfLife(sizeBoard);
		char choice = '6';
		size_t numOfSeeds = 0;
		int ruleSet = 0;
		bool invalidEntry = false;
		string fileName = "glider.txt"; //fileName initialized to glider.txt
		std::ifstream testFile;

		std::cout << "\nMAIN MENU: \n"
			<< "1. Load glider example, must use 30x30 board size\n"
			<< "2. Load oscillator example, must use 30x30 board size\n"
			<< "3. Generate random see file\n"
			<< "4. Input a file\n"
			<< "5. Enter a custom board\n"
			<< "6. Exit Menu\n";
		std::cin >> choice;

		switch (choice)
		{
		case '1': //Glider example
			game.seedBoard(fileName, game.getBoardSize());
			break;
		case '2': //Oscillator example
			game.seedBoard("oscillator.txt", game.getBoardSize());
			break;
		case '3': //Randomly seeded file
			cout << "Enter the number of seeds to generate: ";
			std::cin >> numOfSeeds;
			if (numOfSeeds >=  game.getBoardSize()*game.getBoardSize())
			{
				cout << "Amount not valid. Must be less than " << game.getBoardSize()*game.getBoardSize() << ".\n";
				invalidEntry = true; //user input invalid
			}
			else
			{
				game.seedBoard(numOfSeeds);
				invalidEntry = false; //user input valid
			}
			break;
		case '4': //User input file
			cout << "Input the file that you would like to use (ex. file.txt): ";
			std::cin >> fileName;
			testFile.open(fileName);
			if (testFile.is_open())
			{
				game.seedBoard(fileName, game.getBoardSize());
				invalidEntry = false; //valid entry input by user
			}
			else
			{
				cout << "Error opening file.\n";
				invalidEntry = true; //invalid entry by user or bad file
			}
			break;
		case '5': //user input board from keyboard
			game.seedBoard();
			break;
		case '6': //exit program
			std::cout << "Thanks for playing.\n";
			break;
		default: //1-6 was not entered
			std::cout << "\nInvalid Entry.\n";
			invalidEntry = true;
			break;
		}

		// Only perform game of life calculations if a valid entry was input
		if ((choice == '1' || choice == '2' || choice == '3' || choice == '4' || choice == '5') && invalidEntry == false)
		{
			cout << "Rule set 1 or 2?: "; //User determines which rule set to use
			std::cin >> ruleSet;
			if (ruleSet == 1) //standard game set
			{
				std::cout << game << std::endl; //display game

				while (numberOfIterations != 0) //User enters 0 to exit loop
				{
					for (size_t i = 0; i < numberOfIterations; i++) //game loops for user input number of iterations
					{
						game.calculateNextGen(game.currentLife, game.nextLife); //calculate next generation with standard rule set

						std::cout << game << std::endl; //display game

						for (size_t r = 0; r < boardSize; r++)
						{
							for (size_t c = 0; c < boardSize; c++)
							{
								game.currentLife[r][c].setState(game.nextLife[r][c].getState()); //Current state is the previous next state
							}
						}

						std::this_thread::sleep_for(std::chrono::seconds(1)); //Used to pause the program for 1 sec between each generation
					}

					int numTemp = 0;
					cout << "More generations?\n"
						<< "0 - To exit\n"
						<< "Integer Value - To continue\n";
					std::cin >> numTemp; //continue for more generations or exit loop if user enters 0

					if (numTemp >= 0)
						numberOfIterations = numTemp;
					else
					{
						cout << "\nInvalid input.\n"; // if user enters number less than 0, invalid input displayed and loop is exited
						numberOfIterations = 0;
					}
				}
			}
			else if (ruleSet == 2) //day and night rule set
			{
				std::cout << game << std::endl; //display game

				while (numberOfIterations != 0) //loop continues until user enters 0
				{
					for (size_t i = 0; i < numberOfIterations; i++) //Loops for a number of generations
					{
						game.calculateNextGenXXX(game.currentLife, game.nextLife); //calculate next generation based on day & night rule set

						std::cout << game << std::endl; //display game

						for (size_t r = 0; r < boardSize; r++)
						{
							for (size_t c = 0; c < boardSize; c++)
							{
								game.currentLife[r][c].setState(game.nextLife[r][c].getState()); //Current state is the previous next state
							}
						}

						std::this_thread::sleep_for(std::chrono::seconds(1)); //Used to pause the program for 1 sec between each generation
					}

					int numTemp = 0;
					cout << "More generations?\n"
						<< "0 - To exit\n"
						<< "Integer Value - To continue\n"; 
					std::cin >> numTemp; //user can choose to continue or enter 0 to exit

					if (numTemp >= 0) //error checks for valid input by the user
						numberOfIterations = numTemp;
					else //if input is invalid, the loop is exited
					{
						cout << "\nInvalid input.\n";
						numberOfIterations = 0;
					}
				}
			}
			else
			{
				cout << "\nInvalid entry.\n"; //invalid input for menu option
			}
		}

	}

	
	//  Display cell's face 
	ostream& operator << (ostream& out, const Cell& cell) {
		out << (cell.getState() ? cell.alive : cell.dead);
		return out;
	}

	//  Display game's board 
	ostream& operator << (ostream& output, const GameOfLife& board) {

		// Iterate over the board and display cells
		size_t row, col;
		for (row = 0; row < board.boardSize; row++)
		{
			for (col = 0; col < board.boardSize; col++)
			{
				output << board.currentLife[row][col].getFace(); //display boards current face at each position
			}
			output << std::endl;
		}
		output << std::endl;
		return output;
	}

	// Input a board from either a file or keyboard
	istream & operator>>(istream & input, GameOfLife & board)
	{

		for (size_t row = 0; row < board.getBoardSize(); row++)
		{
			for (size_t col = 0; col < board.getBoardSize(); col++)
			{
				//input goes to a dynamic char fileInput member
				input >> board.fileInput[row][col];
				
			}
		}

		return input;
	}
}
