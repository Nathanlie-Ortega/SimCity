
/* 
File Name: Industrial.h

*/

#include <iostream>
#include <vector>

using namespace std;

class Industrial 
{
    public:
        
        //This updates the population of industrial zones based on adjacency rules.
        void updateIndustrial(vector<vector<char>> &regionGrid, vector<vector<int>>& dataPopulation, int& openWorkers, int& goodsAvailable);


        //This will execute checks to the number of adjacent cells.
        int checkAdjacency(const vector<vector<int>>& dataPopulation, int arr1, int arr2);

};