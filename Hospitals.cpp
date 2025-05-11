
/* 
File Name: Hospitals.cpp

*/ 

#include <iostream>
#include "Hospitals.h"

using namespace std;

//This is Hospitals functionality.
Hospitals::Hospitals(vector<vector<char>>& grid, vector<vector<int>>& hospitalPopulation, const AdvancedPollution& hospitalPollution) : regionGrid(grid), dataPopulation(hospitalPopulation), advancedPollution(hospitalPollution) 
{
    int rowArrangement = regionGrid.size();

    int columnArrangement = rowArrangement > 0 ? regionGrid[0].size() : 0;

    illnessRates = vector<vector<int>>(rowArrangement, vector<int>(columnArrangement, 0));

    for (size_t i = 0; i < rowArrangement; i++) 
    {

        for (size_t j = 0; j < columnArrangement; j++) 
        {

            if (regionGrid[i][j] == 'H') 
            {
                proxHospitalZones.emplace_back(i, j);

            }
        }
    }
}

//This is calculateIllnessRates functionality.
void Hospitals::calculateIllnessRates() 
{
    const auto& dataIndustrialPollution = advancedPollution.getDataIndustrialPollution();

    const auto& dataVehiclePollution = advancedPollution.getDataVehiclePollution();

    for (size_t i = 0; i < regionGrid.size(); i++) 
    {
        for (size_t j = 0; j < regionGrid[i].size(); j++) 
        {
            if (dataPopulation[i][j] >= 1) 
            {
                int overallPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];

                illnessRates[i][j] = overallPollution * dataPopulation[i][j];

            } 
            
            else 
            {
                illnessRates[i][j] = 0;
            }
        }
    }
}

//This is updateHospitalZones functionality.
void Hospitals::updateHospitalZones()
 {
    for (const auto& hospZones : proxHospitalZones) 
    {
        int i = hospZones.first, j = hospZones.second;

        int illnessAlerts = 0;

        for (int nextZone = -2; nextZone <= 2; nextZone++) 
        {
            for (int hospZone = -2; hospZone <= 2; hospZone++) 
            {
                int temp = i + nextZone, prevTemp = j + hospZone;

                if (temp >= 0 && temp < regionGrid.size() && prevTemp >= 0 && prevTemp < regionGrid[0].size()) 
                {
                    illnessAlerts += illnessRates[temp][prevTemp];

                }
            }
        }


        if (illnessAlerts >= 5) 
        {
            if (dataPopulation[i][j] < 5) 
            {
                dataPopulation[i][j]++;

            }
        }
    }
}

//This is getIllnessRates getter functionality.
const vector<vector<int>>& Hospitals::getIllnessRates() const 
{
    return illnessRates;
}