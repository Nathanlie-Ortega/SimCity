
/* 
File Name: Industrial.cpp

*/

#include "Industrial.h"
#include <iostream>

using namespace std;

void Industrial::updateIndustrial(vector<vector<char>>& regionGrid, vector<vector<int>>& dataPopulation, int& openWorkers, int& goodsAvailable) 
{
    int rowArrangement = regionGrid.size();

    int columnArrangement = regionGrid[0].size();

    vector<vector<int>> growPopulation = dataPopulation;    //This is a population data to track updates this step.

    bool newlyAdjustment = false;

    //This variables will prioritize industrial growth at (0, 0) for this step.
    int industrialRow = 0;

    int industrialColumn = 0;

    //This will execute if the (0,0) industrial cell can grow.
    if (!newlyAdjustment && regionGrid[industrialRow][industrialColumn] == 'I' && dataPopulation[industrialRow][industrialColumn] == 0 && openWorkers >= 2) 
    {
        int powerlineNeighbor = 0;

        //This will check all neighbors around (0, 0) for a powerline.
        for (int nextP = -1; nextP <= 1; nextP++)
        {
            for (int advancePow = -1; advancePow <= 1; advancePow++) 
            {
                int temp = industrialRow + nextP;

                int prevTemp = industrialColumn + advancePow;

                //This will ensure within bounds and check for 'T' powerline.
                if (temp >= 0 && temp < rowArrangement && prevTemp >= 0 && prevTemp < columnArrangement && regionGrid[temp][prevTemp] == 'T') 
                {
                    powerlineNeighbor = 1;

                }

            }
        }

        //This will grow industrial population if adjacent to powerline.
        if (powerlineNeighbor == 1)
        { 
            growPopulation[industrialRow][industrialColumn] = 1;    //This increases population to 1.

            openWorkers -= 2;  //This consumes 2 workers.

            goodsAvailable += 1;    //This produces 1 good.

            newlyAdjustment = true;

        }

    }

    //This scans the region if target didn’t update.
    if (!newlyAdjustment) 
    {
        for (int i = 0; i < rowArrangement && !newlyAdjustment; i++) 
        {

            for (int j = 0; j < columnArrangement && !newlyAdjustment; j++)
            {
                //This checks for unoccupied industrial cell with enough workers.
                if (regionGrid[i][j] == 'I' && dataPopulation[i][j] == 0 && openWorkers >= 2)
                {
                    int powerlineNeighbor = 0;

                    //This checks surrounding cells for powerline.
                    for (int nextP = -1; nextP <= 1; nextP++) 
                    {
                        for (int advancePow = -1; advancePow <= 1; advancePow++) 
                        {
                            int temp = i + nextP;

                            int prevTemp = j + advancePow;

                            if (temp >= 0 && temp < rowArrangement && prevTemp >= 0 && prevTemp < columnArrangement && regionGrid[temp][prevTemp] == 'T') 
                            {
                                powerlineNeighbor = 1;

                            }

                        }
                    }

                    //If powerline is nearby, it will grow.
                    if (powerlineNeighbor == 1)
                    {
                        growPopulation[i][j] = 1;

                        openWorkers -= 2;

                        goodsAvailable += 1;

                        newlyAdjustment = true;

                    }
                }
            }
        }
    }


    if (newlyAdjustment)        //This updates to the main population grid if any growth happened.
    {
        dataPopulation = growPopulation;
    }
}


//This is a function to check and count adjacent cells with a population of at least 1.
int Industrial::checkAdjacency(const vector<vector<int>>& dataPopulation, int arr1, int arr2) 
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
                entry++;        //Indicates the entry neighboring cells with at least 1 industrial.

            }

        }
    }

    return entry;
}
