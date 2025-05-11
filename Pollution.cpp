
/* 
File Name: Pollution.cpp

*/


#include "Pollution.h"
#include <iostream>

using namespace std;


//This is a function to spread pollution based on the population in the region.
void Pollution::spreadPollution(vector<vector<int>>& pollutionRegion, const vector<vector<int>>& dataPopulation) 
{
     //This ensures the pollutionRegion grid is the same size as dataPopulation.
    if (pollutionRegion.size() != dataPopulation.size() || pollutionRegion[0].size() != dataPopulation[0].size()) 
    {

        pollutionRegion.resize(dataPopulation.size(), vector<int>(dataPopulation[0].size(), 0));    //This willl resize pollutionRegion and initialize all values to 0.

    }

     //This will loop through every cell in the grid.
    for (int i = 0; i < dataPopulation.size(); i++) 
    {
        for (int j = 0; j < dataPopulation[i].size(); j++) 
        {
             //This means that if the cell has a population, set pollution level equal to population value.
            if (dataPopulation[i][j] > 0)
            {
                pollutionRegion[i][j] = dataPopulation[i][j];  //This is a direct pollution source

            }

            //This means that it will mark this cell polluted if powerline changed and industrial cells below have population.
            if (i == 0 && j == 2 && dataPopulation[1][0] > 0 && dataPopulation[1][1] > 0)
            {
                pollutionRegion[i][j] = 1;  //This will simulate light pollution spread to powerline.

            }
        }
    }
}