
/* 
File Name: Commercial.cpp

*/

#include "Commercial.h"
#include <iostream>

using namespace std;

void Commercial::updateCommercial(vector<vector<char>>& regionGrid, vector<vector<int>>& dataPopulation, int& openWorkers, int& goodsAvailable)
{
    int rowArrangement = regionGrid.size();             //This will get the number of rows arrangement in the grid.

    int columnArrangement = regionGrid[0].size();       //This will get the number of columns arrangement in the grid.

    vector<vector<int>> growPopulation = dataPopulation;

    bool newlyAdjustment = false;

    //This will iterate over each cell in the region.
    for (int i = 0; i < rowArrangement; i++) 
    {

        for (int j = 0; j < columnArrangement; j++)
         {


            //This is a process for commercial zones with at least one available worker and good.
            if (regionGrid[i][j] == 'C' && openWorkers >= 1 && goodsAvailable >= 1) 
            {

                int data = dataPopulation[i][j];

                int adjacency = checkAdjacency(dataPopulation, i, j);

                int powerlineNeighbor = 0;

                //This will check surrounding 8 cells for adjacent powerlines.
                for (int nextP = -1; nextP <= 1; nextP++) 
                {

                    for (int advancePow = -1; advancePow <= 1; advancePow++) 
                    {

                        int temp = i + nextP;

                        int prevTemp = j + advancePow;

                        //This will bounds check and powerline verification.
                        if (temp >= 0 && temp < rowArrangement && prevTemp >= 0 && prevTemp < columnArrangement && regionGrid[temp][prevTemp] == 'T') 
                        {
                            powerlineNeighbor = 1;      //This means that it found a powerline adjacent to the commercial cell.

                        }

                    }
                }

                //This applies ommercial growth rules.
                if ((data == 0 && powerlineNeighbor == 1) || adjacency >= 1) 
                {
                    growPopulation[i][j] = data + 1;        //This means that it increases population by 1.

                    openWorkers--;                          //This assigns one worker to the new job.

                    goodsAvailable--;                       //This assigns one good to the commercial zone.

                    newlyAdjustment = true;                 //This marks change for this timestep.

                    break;                                   //This allows one commercial cell to grow per time step.

                }
            }
        }

        if (newlyAdjustment) 
        {
            break;          //This stops checking other cells if a commercial cell has grown.
        }

    }

    dataPopulation = growPopulation;        //This updates the population data for the region.
}


//This is a function to check and count adjacent cells with a population of at least 1
int Commercial::checkAdjacency(const vector<vector<int>> &dataPopulation, int arr1, int arr2) 
{
    int checkRows = dataPopulation.size();

    int checkColumns = dataPopulation[0].size();

    int entry = 0;


    //This execution will check the surrounding neighbors.
    for (int i = -1; i <= 1; i++)
    {

        for (int j = -1; j <= 1; j++) 
        {
            if (i == 0 && j == 0) 
            {
                continue;
            }

            int newArr1 = arr1 + i;

            int newArr2 = arr2 + j;

            //This will check the boundaries and count adjacent cells with population.
            if (newArr1 >= 0 && newArr1 < checkRows && newArr2 >= 0 && newArr2 < checkColumns && dataPopulation[newArr1][newArr2] >= 1) 
            {
                entry++;    //Indicates the entry neighboring cells with at least 1 commercial.
            }

        }
    }

    return entry;
}