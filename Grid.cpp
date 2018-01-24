#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <random>
#include <string>
#include <limits>
#include <algorithm>
#include "Grid.h"

using namespace std;

// Constructor for manual input of dimensions
Grid :: Grid(int mode, int outStyle, int rows, int columns, double popDensity){
	_mode = mode;
	_outStyle = outStyle;
	_rows = rows;
	_columns = columns;
	_popDensity = popDensity;
	bool isStable = false;
	populateMap();
}

// Constructor for providing map file
Grid :: Grid(int mode, int outStyle, string mapFile){
	_mode = mode;
	_outStyle = outStyle;
	_mapFile = mapFile;
	bool isStable = false;
	readMap();	
}

// Destructor
Grid :: ~Grid(){
	// Free up memory used
	for (int i = 0; i < _rows; ++i){
    		delete [] _currGen[i];
  	}
	delete [] _currGen;
}

// Populates array based on dimenions/density given by user
void Grid :: populateMap(){
	// Seed the random generator
	srand((unsigned)time(NULL));

	// Declare our array
	_currGen = new string*[_rows];
  	for (int i = 0; i < _rows; ++i){_currGen[i] = new string[_columns];}

	// Populate the Grid via random generation according to popDensity	
	for(int i = 0; i < _rows; ++i){
		for(int j = 0; j < _columns; j++){
			double chance = ((double)rand()/(double)RAND_MAX);
			if(chance >= _popDensity){_currGen[i][j] = 'X';}
			else{_currGen[i][j] = '-';}
		}
	}
}

// Populates array based on given map file
void Grid :: readMap(){
	ifstream myfile;
	myfile.open(_mapFile);
	int count = 0;
	string line;
	
	// This first loop gets the rows and columns
	while(getline(myfile,line)){
		if(count == 0){_rows = stoi(line);}
		if(count == 1){
			_columns = stoi(line);
			break;
		}
		++count;
	}
	
	// Declare our array
	_currGen = new string*[_rows];
  	for (int i = 0; i < _rows; ++i){_currGen[i] = new string[_columns];}
	int currRow = 0;

	// This second loop populates the Grid based on our map file
	while(getline(myfile,line)){
		for(int i = 0; i < line.size(); ++i){
			_currGen[currRow][i] = line[i];		
		}
		++currRow;
	}
	myfile.close();	
}

// Calculate next generation according to specified mode
void Grid :: calculateNextGen(){	
	string **nextGen;
	nextGen = new string*[_rows];
  	for (int i = 0; i < _rows; ++i){nextGen[i] = new string[_columns];}
		
	for(int i = 0; i < _rows; ++i){
		for(int j = 0; j < _columns; j++){
			int neighbors;
			// Check mode
			if(_mode == 0){neighbors = getNeighborsClassic(i,j);}
			else if(_mode == 1){neighbors = getNeighborsDoughnut(i,j);}
			else if(_mode == 2){neighbors = getNeighborsMirror(i,j);}
			// Determine status of each cell for the next generation based on its number of neighbors
			// 1 or fewer neighbors: dies
			if(neighbors == 0 || neighbors == 1){nextGen[i][j] = '-';}
			// 2 neighbors: stays the same
			else if(neighbors == 2){nextGen[i][j] = _currGen[i][j];}
			// 3 neighbors: grows
			else if(neighbors == 3){nextGen[i][j] = 'X';}
			// 4 or more neighbors: dies
			else{nextGen[i][j] = '-';}
		}
	}

	bool same = true;
	for(int i = 0; i < _rows; ++i){
		for(int j = 0; j < _columns; ++j){
			if(nextGen[i][j] != _currGen[i][j]){same = false;}
		}
	}
	if(same){
		isStable = true;
		cout << "\nThe grid is stable.\n";
	}
	// Update our current gen
	_currGen = nextGen;
}

// Gets number of neighbors for a given cell based on Classic rules
int Grid :: getNeighborsClassic(int r, int c){
	int numNeighbors = 0;
	// Decrement numNeighbors if the given cell is alive(because the loop below counts the given cell itself, which should not count as a neighbor)
	if(_currGen[r][c] == "X"){--numNeighbors;}
	//Checks 8 locations around given cell
	for(int i = r-1; i <= r+1; ++i){
		for(int j = c-1; j <= c+1; ++j){
			// Checks that the location being looked at is valid
			if(i >= 0 && i < _rows && j >= 0 && j < _columns ){
				if(_currGen[i][j] == "X"){
					++numNeighbors;
				}
			}
		}
	}
	return numNeighbors;
}

// Get number of neighbors for a given cell based on Doughnut rules
int Grid :: getNeighborsDoughnut(int r, int c){
	int numNeighbors = 0;
	if(_currGen[r][c] == "X"){--numNeighbors;}
	for(int i = r-1; i <= r+1; ++i){
		for(int j = c-1; j <= c+1; ++j){
			if(i >= 0 && i < _rows && j >= 0 && j < _columns ){
				if(_currGen[i][j] == "X"){
					++numNeighbors;
				}
			}
		}
	}
	// We need to check the edge cases, which are any cells located on the corners or sides
	// Top left corner
	if(r == 0 && c == 0){
		if(_currGen[_rows-1][_columns-1] == "X" ||
			_currGen[r+1][_columns-1] == "X" ||
			_currGen[r][_columns-1] == "X" ||
			_currGen[_rows-1][c] == "X" ||
			_currGen[_rows-1][c+1] == "X"){++numNeighbors;}
	}
	// Upper row
	if(r == 0 && c != 0 && c != _columns-1){
		if(_currGen[_rows-1][c-1] == "X" ||
			_currGen[_rows-1][c] == "X" ||
			_currGen[_rows-1][c+1] == "X"){++numNeighbors;}
	}
	// Top right corner
	if(r == 0 && c == _columns-1){
		if(_currGen[_rows-1][0] == "X" ||
			_currGen[r+1][0] == "X" ||
			_currGen[r][0] == "X" ||
			_currGen[_rows-1][c] == "X" ||
			_currGen[_rows-1][c-1] == "X"){++numNeighbors;}
	}
	// Left column
	if(r != 0 && r != _rows-1 && c == 0){
		if(_currGen[r-1][_columns-1] == "X" ||
			_currGen[r][_columns-1] == "X" ||
			_currGen[r+1][_columns-1] == "X"){++numNeighbors;}
	}
	//Bottom left corner
	if(r == _rows-1 && c == 0){
		if(_currGen[0][_columns-1] == "X" ||
			_currGen[r-1][_columns-1] == "X" ||
			_currGen[r][_columns-1] == "X" ||
			_currGen[0][c] == "X" ||
			_currGen[0][c+1] == "X"){++numNeighbors;}
	}
	//Bottom row
	if(r == _rows-1 && c != 0 && c != _columns-1){
		if(_currGen[0][c-1] == "X" ||
			_currGen[0][c] == "X" ||
			_currGen[0][c+1] == "X"){++numNeighbors;}
	}
	//Bottom right corner
	if(r == _rows-1 && c == _columns-1){
		if(_currGen[0][0] == "X" ||
			_currGen[r-1][0] == "X" ||
			_currGen[r][0] == "X" ||
			_currGen[0][c] == "X" ||
			_currGen[0][c-1] == "X"){++numNeighbors;}
	}
	//Right column
	if(r != 0 && r != _rows-1 && c == _columns-1){
		if(_currGen[r-1][0] == "X" ||
			_currGen[r][0] == "X" ||
			_currGen[r+1][0] == "X"){++numNeighbors;}
	}
	return numNeighbors;

}

// Get number of neighbors for a given cell based on Mirror rules
int Grid :: getNeighborsMirror(int r, int c){
	int numNeighbors = 0;
	if(_currGen[r][c] == "X"){--numNeighbors;}
	for(int i = r-1; i <= r+1; ++i){
		for(int j = c-1; j <= c+1; ++j){
			if(i >= 0 && i < _rows && j >= 0 && j < _columns ){
				if(_currGen[i][j] == "X"){
					++numNeighbors;
				}
			}
		}
	}
	// Top left corner
	if(r == 0 && c == 0){
		if(_currGen[r][c] == "X"){numNeighbors += 3;} 
		if(_currGen[r+1][c] == "X" ||
			_currGen[r][c+1] == "X"){++numNeighbors;}
	}
	// Upper row
	if(r == 0 && c != 0 && c != _columns-1){
		if(_currGen[r+1][c-1] == "X" ||
			_currGen[r+1][c] == "X" ||
			_currGen[r+1][c+1] == "X"){++numNeighbors;}
	}
	// Top right corner
	if(r == 0 && c == _columns-1){
		if(_currGen[r][c] == "X"){numNeighbors += 3;}
		if(_currGen[r+1][c] == "X" ||
			_currGen[r][c-1] == "X"){++numNeighbors;}
	}
	// Left column
	if(r != 0 && r != _rows-1 && c == 0){
		if(_currGen[r-1][c+1] == "X" ||
			_currGen[r][c+1] == "X" ||
			_currGen[r+1][c+1] == "X"){++numNeighbors;}
	}
	//Bottom left corner
	if(r == _rows-1 && c == 0){
		if(_currGen[r][c] == "X"){numNeighbors += 3;}
		if(_currGen[c][r-1] == "X" ||
			_currGen[r][c+1] == "X"){++numNeighbors;}
	}
	//Bottom row
	if(r == _rows-1 && c != 0 && c != _columns-1){
		if(_currGen[r-1][c-1] == "X" ||
			_currGen[r-1][c] == "X" ||
			_currGen[r-1][c+1] == "X"){++numNeighbors;}
	}
	//Bottom right corner
	if(r == _rows-1 && c == _columns-1){
		if(_currGen[r][c] == "X"){numNeighbors += 3;}
		if(_currGen[r][c-1] == "X" ||
			_currGen[r-1][c] == "X"){++numNeighbors;}
	}
	//Right column
	if(r != 0 && r != _rows-1 && c == _columns-1){
		if(_currGen[r-1][c-1] == "X" ||
			_currGen[r][c-1] == "X" ||
			_currGen[r+1][c-1] == "X"){++numNeighbors;}
	}
	
	return numNeighbors;	
}

// Check if our grid is empty
bool Grid :: isEmpty(){
	int xCount = 0;
	for(int i = 0; i < _rows; ++i){
		for(int j = 0; j < _columns; ++j){
			if(_currGen[i][j] == "X"){
				++xCount;
			}
		}
	}
	// If no X's are found in the Grid, it is empty.
	if(xCount > 0){return false;}
	else{return true;}
}

void Grid :: output(int gen){
	for(int i = 0; i < _rows; ++i){
		for(int j = 0; j < _columns; j++){
			cout << _currGen[i][j];
		}
		cout << endl;
	}
	
	// Brief pause between generations
	if(_outStyle == 0){
		sleep(2);
	}
}

// This separate output function makes it easier to output to file(Need to know whether or not to overwrite or append depending on what generation it is) 
void Grid :: output(int gen, string filename){
	ofstream myfile;
	// Open file normally if its the first gen
	if(gen == 0){myfile.open(filename);}
	// Else open in append mode
	else{myfile.open(filename, fstream::app);}
	// Output results to file
	myfile << "Gen " << gen << ":\n";
	for(int i = 0; i < _rows; ++i){
		for(int j = 0; j < _columns; j++){
			myfile << _currGen[i][j];
		}
		myfile << endl;
	}
	myfile.close();
}
