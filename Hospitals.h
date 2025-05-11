
/* 
File Name: Hospitals.h

*/ 

#ifndef HOSPITALS_H
#define HOSPITALS_H

#include <iostream>
#include <vector>
#include "AdvancedPollution.h"

using namespace std;

//This the New Feature for Project 2 that contains the class of Hospitals.
class Hospitals 
{
    private:
        vector<vector<char>>& regionGrid;   //A vector to store the grid of the region.

        vector<vector<int>>& dataPopulation;     //A vector to store the data for the population.

        const AdvancedPollution& advancedPollution; //A vector to store the data for the advanced pollution.

        vector<vector<int>> illnessRates;       //A vector to store the data for the illness rates.

        vector<pair<int, int>> proxHospitalZones;   //A vector to store the data for the calculated proximity Hospitals.

    
    public:
        //This is Hospitals functionality.
        Hospitals(vector<vector<char>>& grid, vector<vector<int>>& hospitalPopulation, const AdvancedPollution& hospitalPollution);

        void calculateIllnessRates();   //This is calculateIllnessRates functionality.

        void updateHospitalZones();     //This is updateHospitalZones functionality.

        const vector<vector<int>>& getIllnessRates() const; //This is getIllnessRates getter functionality.

};

#endif // HOSPITALS_H