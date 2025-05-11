
/* 
File Name: City_Simulation.h

*/ 

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include "AdvancedPollution.h"
#include "Hospitals.h"

using namespace std;

class citySimulation               //A class that represents the city simulation.
{
private:
    vector<vector<char>> regionGrid;        //A vector to store the grid of the region.
    vector<vector<int>> dataPopulation;     //A vector to store the data for the population.
    vector<vector<int>> dataPollution;      //A vector to store the data for the pollution.
    int openWorkers;       //This is a variable to navigate workers who are available.
    int goodsAvailable;         //This is a variable to navigate goods who are available.

    unique_ptr<AdvancedPollution> advancedPollution;    //This is new features involving advancedPollution for project 2.
    unique_ptr<Hospitals> hospitals;                    //This is new features involving Hospitals for project 2.
    bool project2Initialized;                       //This is new features involving initializing project 2.


public:
    void initRegion(const string &regionFile);      //This is a function to initialize the region grid from the region file.
    void printInitialState() const;                 //This is a function to print the initial state of the region.
    void executeCitySim(int timeLimit, int refreshRate);   //This is a function to run the simulation for time limit and refresh rate.
    void analyzeArea();     //This is a function to analyze the area.

    citySimulation() : openWorkers(0), goodsAvailable(0), advancedPollution(nullptr), hospitals(nullptr), project2Initialized(false) {} //This is City Simulation's with additional sub features.

    void initProject2Features();    //This is new features involving initiating the features in project 2.
}; 

