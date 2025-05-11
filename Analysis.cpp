
/* 
File Name: Analysis.cpp

*/

#include "Analysis.h"
#include <iostream>

using namespace std;

//This is the function to print the current state of the region grid along with population data, open workers, and goods.
void Analysis::printRegion(const vector<vector<char>>& regionGrid, const vector<vector<int>>& dataPopulation, int openWorkers, int goodsAvailable)
{
    //This loops through each row of the grid.
    for (size_t i = 0; i < regionGrid.size(); i++) 
    {
        //This loops through each column of the current row.
        for (size_t j = 0; j < regionGrid[i].size(); j++) 
        {
            //It will print the population value if this cell has population.
            if (dataPopulation[i][j] > 0) 
            {
                cout << dataPopulation[i][j];
            } 
            
            else        // Otherwise, print the zone character in either 'R', 'C', 'I', 'T'.
            {
                cout << regionGrid[i][j];
            }
            
            //This will add a space between cells.
            if (j < regionGrid[i].size() - 1) 
            {
                cout << " ";
            }
        }
        
        cout << endl;
    }
    
    cout << "\nAvailable Workers: " << openWorkers << endl;  //A diusplay output for the count of available workers.

    cout << "Available Goods: " << goodsAvailable << endl;   //A display output the count of available goods.
    
    cout << endl;

}
