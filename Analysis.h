
/* 
File Name: Analysis.h

*/

#include <iostream>
#include <vector>

using namespace std;

class Analysis 
{
    public:
    
        //This updates the print region function.
        void printRegion(const vector<vector<char>> &regionGrid, const vector<vector<int>> &dataPopulation, int goodsAvailable, int availableGoods);
    
};
