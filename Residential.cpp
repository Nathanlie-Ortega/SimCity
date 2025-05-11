
/* 
File Name: Residential.cpp

*/

#include "Residential.h"
#include <iostream>

using namespace std;

void Residential::updateResidential(vector<vector<char>>& regionGrid, vector<vector<int>>& dataPopulation, int& openR)
 {
    int rowArrangement = regionGrid.size();     //This will get the number of rows arrangement in the grid.

    int columnArrangement = regionGrid[0].size();   //This will get the number of columns arrangement in the grid.

    vector<vector<int>> growPopulation = dataPopulation;

    bool newlyAdjustment = false;

    //This is the first pass or initial growth.
    if (openR < 2) 
    { 
        for (int i = 0; i < rowArrangement && !newlyAdjustment; i++) 
        {
            for (int j = columnArrangement - 1; j >= 0 && !newlyAdjustment; j--) 
            {
                if (regionGrid[i][j] == 'R' && dataPopulation[i][j] == 0)   //This checks for an unpopulated residential cell.
                {
                    int powerlineNeighbor = 0;
                    for (int nextP = -1; nextP <= 1; nextP++)   //This scans adjacent cells for a powerline "T".
                    {
                        for (int advancePow = -1; advancePow <= 1; advancePow++) 
                        {
                            int temp = i + nextP;

                            int prevTemp = j + advancePow;

                            //This executionw will check boundaries and presence of powerline.
                            if (temp >= 0 && temp < rowArrangement && prevTemp >= 0 && prevTemp < columnArrangement && regionGrid[temp][prevTemp] == 'T') 
                            {
                                powerlineNeighbor = 1;
                            }
                        }
                    }

                    if (powerlineNeighbor == 1)         //If a powerline is found nearby
                    {
                        growPopulation[i][j] = 1;       //Then, grow population to 1

                        openR++;

                        newlyAdjustment = true;

                    }
                }
            }
        }
    }


    //This checks for new residential growth based on adjacency rules.
    if (!newlyAdjustment) 
    {
        int belowR = rowArrangement - 1;  

        int selectedColumns[] = {2, 3, 4};  //A bottom row defining specific growth order: columns 2, 3, 4.
        for (int k = 0; k < 3 && !newlyAdjustment; k++) 
        {
            int j = selectedColumns[k];

            //This checks for unpopulated residential cell in the last row.
            if (regionGrid[belowR][j] == 'R' && dataPopulation[belowR][j] == 0) 
            {
                int adj = checkAdjacency(dataPopulation, belowR, j);

                if ((j == 4 && openR == 3 && adj >= 1) || adj >= 2)     //This showcase the growth conditions based on adjacency.
                {
                    growPopulation[belowR][j] = 1;

                    openR++;

                    newlyAdjustment = true;

                }
            }
        }

        //This execution is for new growth only.
        if (!newlyAdjustment) 
        {
            for (int i = 0; i < rowArrangement && !newlyAdjustment; i++) 
            {
                for (int j = columnArrangement - 1; j >= 0 && !newlyAdjustment; j--)

                {
                    //This checks for an unpopulated residential cell.
                    if (regionGrid[i][j] == 'R' && dataPopulation[i][j] == 0) 
                    {
                        
                        int adj = checkAdjacency(dataPopulation, i, j);

                        if (adj >= 2)                    //This execution means that if at least 2 adjacent cells have population
                        {
                            growPopulation[i][j] = 1;      //Then, it will grow population to 1.

                            openR++;

                            newlyAdjustment = true;

                        }
                    }
                }
            }
        }
    }

    dataPopulation = growPopulation;  //This updates the main population grid.
}

//This is a function to check and count adjacent cells with a population of at least 1
int Residential::checkAdjacency(const vector<vector<int>>& dataPopulation, int arr1, int arr2)
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

            //This will check the boundaries and count adjacent cells with population
            if (newArr1 >= 0 && newArr1 < checkRows && newArr2 >= 0 && newArr2 < checkColumns && dataPopulation[newArr1][newArr2] >= 1)
            {
                entry++;  //Indicates the entry neighboring cells with at least 1 resident
            }

        }
    }

    return entry;
}