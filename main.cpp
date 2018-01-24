#include "Grid.cpp"

void pressEnterToContinue(int gen){
	if(gen==0){cin.ignore();}
	printf("\nPress ENTER to continue: ");
	cin.get();
}	

void pressEnterToQuit(int outStyle){
  if(outStyle != 1){cin.ignore();}
  printf("\nPress ENTER to quit: ");
  cin.get();
}

int main(int argc, char * argv[]){
	int mode, outStyle, assignment, rows, columns, gen = 0;
	double popDensity;
	string inputFile;
	
	// Get mode from user
	cout << "\nEnter 0 for Classic Mode, 1 for Doughnut Mode, or 2 for Mirror Mode: ";
	while(cin >> mode){
		if(mode < 0 || mode > 2){cout << "Please enter 0, 1, or 2: ";}
		else{break;}
	}

	// Get outStyle from user
	cout << "\nHow would you like the results to output? Enter 0 for a brief pause between generations, 1 to press 'Enter' between generations, or 2 to output the results to a text file: ";
	while(cin >> outStyle){
		if(outStyle < 0 || outStyle > 2){cout << "Please enter 0, 1, or 2: ";}
		else{break;}
	}

	// Get assignment from user
	cout << "\nWould you like to provide a map file, or choose random assignment? Enter 0 for map or 1 for random: ";
	while(cin >> assignment){
		if(assignment < 0  || assignment > 1){cout << "Please enter 0 or 1: ";}
		else{break;}
	}

	// Declare our Grid
	Grid * board;
	// Instantiate our Grid based on assignment
	if(assignment == 0){
		// Get map file from user
		cout << "\nPlease enter text file name containing rows, columns, and map: ";
		cin >> inputFile; 
		board = new Grid(mode, outStyle,  inputFile);}
	else{
		// Get rows from user
		cout << "\nPlease enter the number of rows: ";
		while(cin >> rows){
			if(rows < 1){cout << "Please enter a number greater than 0: ";}
			else{break;}
		}
		// Get columns from user
		cout << "\nPlease enter the number of columns: ";
		while(cin >> columns){
			if(columns < 1){cout << "Please enter a number greater than 0: ";}
			else{break;}
		}
		// Get popDensity from user
		cout << "\nPlease enter the population density(double between 0 and 1): ";
		while(cin >> popDensity){
			if(popDensity < 0 || popDensity > 1){cout << "Please enter a double between 0 and 1: ";}
			else{break;}
		}
		board = new Grid(mode, outStyle, rows, columns, popDensity);
	}

	bool running = true;


	// Run the simulation	
	while(running){
		// Output based on outStyle
		if(outStyle != 2){
			cout << "\nGen: " << gen << endl;
			board->output(gen);
			if(outStyle == 1){pressEnterToContinue(gen);}
		}
		else if (outStyle == 2){
			board->output(gen, "dillontidgewell.out");
		}
		// Check if there are no living cells
		if(board->isEmpty() || board->isStable){
			// Call destructor to free up memory
			board->~Grid();
			// Prompt user to press enter to quit
			pressEnterToQuit(outStyle);	
			return 0;			
		}
		board->calculateNextGen();
		// Increment gen
		++gen;
	}
}
