
/* 
File Name: AdvancedPollution.h

*/

#ifndef ADVANCED_POLLUTION_H
#define ADVANCED_POLLUTION_H

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

//This the New Feature for Project 2 that contains the class of AdvancedPollution.
class AdvancedPollution 
{
    private:
        vector<vector<char>>& regionGrid;       //A vector to store the grid of the region.

        vector<vector<int>>& dataPopulation;    //A vector to store the data for the population.

        vector<vector<int>> dataIndustrialPollution;    //A vector to store the data for the industrial pollution.

        vector<vector<int>> dataVehiclePollution;   //A vector to store the data for the vehicle pollution.

        vector<pair<int, int>> pSources;        //A vector to store the pollution sources.
    
    public:
        AdvancedPollution(vector<vector<char>>& grid, vector<vector<int>>& population); //This is the AdvancedPollution functionality.

        void updatePollutionSources();  //This is the updatePollutionSources functionality.

        void pollutionSpreaded();   //This is the pollutionSpreaded functionality.

        const vector<vector<int>>& getDataIndustrialPollution() const;  //This is the functionality to get Data Industrial Pollution.

        const vector<vector<int>>& getDataVehiclePollution() const;     //This is the functionality to get Data Vehcile Pollution.

};

#endif