
/* 
File Name: Residential.h

*/

#include <iostream>
#include <vector>

using namespace std;

class Residential 
{
    public:

        //This updates the population of residential zones based on adjacency rules
        void updateResidential(vector<vector<char>> &regionGrid, vector<vector<int>>& dataPopulation, int& openR);

    private:

         //This will execute checks to the number of adjacent cells
        int checkAdjacency(const vector<vector<int>> &dataPopulation, int arr1, int arr2);

};
