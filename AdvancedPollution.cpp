
/* 
File Name: AdvancedPollution.cpp

*/ 

#include <iostream>
#include "AdvancedPollution.h"

using namespace std;

//This the New Feature for Project 2 that contains the class of AdvancedPollution and AdvancedPollution functionality.
AdvancedPollution::AdvancedPollution(vector<vector<char>>& grid, vector<vector<int>>& population) : regionGrid(grid), dataPopulation(population) 
{
    int rowArrangement = regionGrid.size();

    int columnArrangement = rowArrangement > 0 ? regionGrid[0].size() : 0;

    dataIndustrialPollution = vector<vector<int>>(rowArrangement, vector<int>(columnArrangement, 0));

    dataVehiclePollution = vector<vector<int>>(rowArrangement, vector<int>(columnArrangement, 0));
}

//This is the updatePollutionSources functionality.
void AdvancedPollution::updatePollutionSources() 
{
    pSources.clear();

    //This will reset pollution grids to 0.
    for (size_t i = 0; i < regionGrid.size(); i++) 
    {
        for (size_t j = 0; j < regionGrid[i].size(); j++) 
        {
            dataIndustrialPollution[i][j] = 0;
            dataVehiclePollution[i][j] = 0;
        }
    }

    for (size_t i = 0; i < regionGrid.size(); i++) 
    {
        for (size_t j = 0; j < regionGrid[i].size(); j++) 
        {
            if (regionGrid[i][j] == 'I' && dataPopulation[i][j] >= 1) 
            {
                pSources.emplace_back(i, j);
            }

            if (regionGrid[i][j] == '-') 
            {
                bool adjacentList = false;

                for (int nextP = -1; nextP <= 1; nextP++) 
                {
                    for (int advancePow = -1; advancePow <= 1; advancePow++) 
                    {
                        int temp = i + nextP, prevTemp = j + advancePow;

                        if (temp >= 0 && temp < regionGrid.size() && prevTemp >= 0 && prevTemp < regionGrid[0].size() && 
                            (regionGrid[temp][prevTemp] == 'R' || regionGrid[temp][prevTemp] == 'C') && dataPopulation[temp][prevTemp] >= 1) 
                        {
                            adjacentList = true;
                            break;
                        }
                    }
                    if (adjacentList) break;
                }

                if (adjacentList) 
                {
                    pSources.emplace_back(i, j);
                }
            }
        }
    }
}

//This is the pollutionSpreaded functionality.
void AdvancedPollution::pollutionSpreaded() 
{
    int rowArrangement = regionGrid.size();
    int columnArrangement = rowArrangement > 0 ? regionGrid[0].size() : 0;

    vector<vector<int>> updatedPol(rowArrangement, vector<int>(columnArrangement, 0));
    vector<vector<int>> vehicleRoadPol(rowArrangement, vector<int>(columnArrangement, 0));

    for (const auto& sc : pSources) 
    {
        int i = sc.first, j = sc.second;

        if (regionGrid[i][j] == 'I') 
        {
            updatedPol[i][j] += dataPopulation[i][j]; // Use population value

            for (int pol = j + 1; pol < columnArrangement && (regionGrid[i][pol] == ' ' || regionGrid[i][pol] == 'R' || 
                     regionGrid[i][pol] == 'C' || regionGrid[i][pol] == 'H' || regionGrid[i][pol] == '-' || regionGrid[i][pol] == 'T'); pol++) 
            {
                updatedPol[i][pol] += dataPopulation[i][j];
            }
            for (int pol = j - 1; pol >= 0 && (regionGrid[i][pol] == ' ' || regionGrid[i][pol] == 'R' || 
                     regionGrid[i][pol] == 'C' || regionGrid[i][pol] == 'H' || regionGrid[i][pol] == '-' || regionGrid[i][pol] == 'T'); pol--) 
            {
                updatedPol[i][pol] += dataPopulation[i][j];
            }
        } 
        else if (regionGrid[i][j] == '-') 
        {
            //This will search for the adjacent cell population.
            int sourcePop = 0;
            for (int nextP = -1; nextP <= 1; nextP++) 
            {
                for (int advancePow = -1; advancePow <= 1; advancePow++) 
                {
                    int temp = i + nextP, prevTemp = j + advancePow;
                    if (temp >= 0 && temp < regionGrid.size() && prevTemp >= 0 && prevTemp < regionGrid[0].size() && 
                        (regionGrid[temp][prevTemp] == 'R' || regionGrid[temp][prevTemp] == 'C') && dataPopulation[temp][prevTemp] >= 1) 
                    {
                        sourcePop = max(sourcePop, dataPopulation[temp][prevTemp]);
                    }
                }
            }
            vehicleRoadPol[i][j] += sourcePop;

            for (int pol = j + 1; pol < columnArrangement && (regionGrid[i][pol] == ' ' || regionGrid[i][pol] == 'R' || 
                     regionGrid[i][pol] == 'C' || regionGrid[i][pol] == 'H' || regionGrid[i][pol] == '-' || regionGrid[i][pol] == 'T'); pol++) 
            {
                vehicleRoadPol[i][pol] += sourcePop;
            }
            for (int pol = j - 1; pol >= 0 && (regionGrid[i][pol] == ' ' || regionGrid[i][pol] == 'R' || 
                     regionGrid[i][pol] == 'C' || regionGrid[i][pol] == 'H' || regionGrid[i][pol] == '-' || regionGrid[i][pol] == 'T'); pol--) 
            {
                vehicleRoadPol[i][pol] += sourcePop;
            }
        }
    }

    dataIndustrialPollution = updatedPol;
    dataVehiclePollution = vehicleRoadPol;
}

//This is the functionality to get Data Industrial Pollution.
const vector<vector<int>>& AdvancedPollution::getDataIndustrialPollution() const 
{
    return dataIndustrialPollution;
}

//This is the functionality to get Data Vehcile Pollution.
const vector<vector<int>>& AdvancedPollution::getDataVehiclePollution() const 
{
    return dataVehiclePollution;
}