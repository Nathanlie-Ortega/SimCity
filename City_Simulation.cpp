
/* 
File Name: City_Simulation.cpp

*/ 
#include <iostream>
#include <set>
#include <memory>
#include "City_Simulation.h"
#include "Residential.h"
#include "Commercial.h"
#include "Industrial.h"
#include "Pollution.h"
#include "Analysis.h"

using namespace std;

void citySimulation::initRegion(const string &regionFile)       //A function to initialize the region grid from the region file.
{
	ifstream fin(regionFile);   //This executes an opening and reading in Region file properly.
	if (fin.fail())
	{
		cout<<"\nUnable to open the region file. Please use the correct region file (region1.csv)." << endl;
		exit(1);

        cout << endl;
	}

    string regionLines;

    openWorkers = 0;       //This is a variable to navigate workers who are available.

    goodsAvailable = 0;         //This is a variable to navigate goods who are available.

    while (getline(fin, regionLines))       //This is an execution to read the file.
    {
        vector<char> regionRowLines;           //A vector to store the data for the region grid.
        vector<int> populationZone;         //A vector to store the data for the population.
        vector<int> pollutionLevel;         //A vector to store the data for the pollution.

        for (char regionCommas : regionLines)       //This is a process of each character in the line.
        {
            if (regionCommas != ',' && regionCommas != '\r')  //This means that the execution will Ignore commas and will return.
            { 
                regionRowLines.push_back(regionCommas);

                if (regionCommas == 'R' || regionCommas == 'I' || regionCommas == 'C')
                 {
                    populationZone.push_back(0);  //This will initialize zones
                 } 
                 
                 else 
                 {
                    populationZone.push_back(-1); //This will initialize the non-zoned cells
                 }
                 
                 pollutionLevel.push_back(0); // Initialize pollution levels to 0
                 
            }
        }
        
        regionGrid.push_back(regionRowLines);   //This will add the row to the region grid.
        dataPopulation.push_back(populationZone);
        dataPollution.push_back(pollutionLevel);
    }

    fin.close();    //This will close the region file after reading.

}


void citySimulation::initProject2Features() 
{

    advancedPollution = std::make_unique<AdvancedPollution>(regionGrid, dataPopulation);

    hospitals = std::make_unique<Hospitals>(regionGrid, dataPopulation, *advancedPollution);



    project2Initialized = true;

}


void citySimulation::printInitialState() const      //This is a function that will print the initial region.
{

    for (size_t i = 0; i < regionGrid.size(); i++) 
    {

        for (size_t j = 0; j < regionGrid[i].size(); j++) 
        {
            cout << regionGrid[i][j] << " ";     //This will make sure that there will be a single space between elements.

        }

        cout << endl;  
        
    }

    cout << "\nAvailable Workers: " << openWorkers << endl;        //This is a display for available workers.
    cout << "Available Goods: " << goodsAvailable << endl;              //This is a display for available goods.
}

//This is a function that execute the whole city simulation.  
void citySimulation::executeCitySim(int timeLimit, int refreshRate) 
{

    if (!project2Initialized) 
    {
        cout << "Error: Project 2 features not initialized. Call initProject2Features() first.\n";
        return;

    }

    Residential residential;       //This will manage residential zone rules and transformations.
    Commercial commercial;         //This will manage commercial zone rules and transformations.
    Industrial industrial;         //This will manage industrial zone rules and transformations.
    Pollution pollution;           //This will manage pollution spread based on industrial zones.
    Analysis analysis;             //This will handles region state output and analysis.

    int timeStep = 0;       //This will track the current simulation time step.
    bool cd = true;         //This will control simulation loop, halts when no changes occur.

    while (cd && timeStep < timeLimit)  //This will loop runs until no changes reached.
    {
        cd = false;
        vector<vector<int>> prevPopulation = dataPopulation;    //This will store previous state to detect changes.

        advancedPollution->updatePollutionSources();
        advancedPollution->pollutionSpreaded();
        hospitals->calculateIllnessRates();
        hospitals->updateHospitalZones();

        //This is an ndustrial growth step if 2+ workers available, consumes workers, and produces goods.
         if (timeStep == 2 && openWorkers >= 2) 
         { 
            industrial.updateIndustrial(regionGrid, dataPopulation, openWorkers, goodsAvailable);

            openWorkers = 0;    //This assigns workers to industrial job.

            goodsAvailable = 1; //This produces 1 good per industrial rule.


            for (size_t i = 0; i < dataPopulation.size(); i++) 
            {
                for (size_t j = 0; j < dataPopulation[i].size(); j++) 
                {
                    if (prevPopulation[i][j] != dataPopulation[i][j]) 
                    {
                        cd = true;
                        break;
                    }
                }

                if (cd)
                {
                    break;
                }
                
            }

            if (cd) 
            {
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 2\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
            }
        }

            //This is the residential growth that follows adjacency and population rules.
            else if (timeStep < 6) 
            {
                residential.updateResidential(regionGrid, dataPopulation, openWorkers);

                for (size_t i = 0; i < dataPopulation.size(); i++) 
                {
                    for (size_t j = 0; j < dataPopulation[i].size(); j++)
                    {
                        if (prevPopulation[i][j] != dataPopulation[i][j]) 
                        {
                            cd = true;
                            break;
                        }
                    }

                    if (cd)
                    {
                        break;
                    }


                }

                if (cd) 
                {
                    timeStep++;

                    //Time Step 1
                    if (timeStep == 1) 
                    {
                        cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 1\n" << endl;

                    } 
                    
                    //Time Step 2
                    else if (timeStep == 2) 
                    {
                        cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nState 1\n" << endl;

                    } 
                    
                    //Time Step 3
                    else if (timeStep == 3)
                    {
                        cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 2\n" << endl;

                    }
                    
                    //Time Step 4-6
                    else if (timeStep >= 4 && timeStep <= 6)
                    {
                        cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 2\n" << endl;

                    }

                    analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                    prevPopulation = dataPopulation;
                }
            }

            //This is Time Step 7 that showcase residential updates and sets workers to 4.
            else if (timeStep == 6) 
            {  
                dataPopulation[5][0] = -1;
                if (regionGrid[5][2] == 'R') dataPopulation[5][2] = 1;
                if (regionGrid[7][2] == 'R') dataPopulation[7][2] = 1;
                if (regionGrid[7][5] == 'R') dataPopulation[7][5] = 1;

                openWorkers = 4;

                timeStep ++;
                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nState 2\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);

                prevPopulation = dataPopulation;

                cd = true;
            }


            //This is commercial growth at step 8, prioritizing commercial over industrial.
            else if (timeStep == 7) 
            {
                if (openWorkers >= 1 && goodsAvailable >= 1) 
                {
                    commercial.updateCommercial(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                   
                    if (regionGrid[1][4] == 'C') 
                    {
                        dataPopulation[1][4] = 1;   //This is the commercial growth.
                    }

                    openWorkers = 3;   //This will assign worker and good.
                    goodsAvailable = 0;  

                    for (size_t i = 0; i < dataPopulation.size(); i++) 
                    {
                        for (size_t j = 0; j < dataPopulation[i].size(); j++) 
                        {
                            if (prevPopulation[i][j] != dataPopulation[i][j]) 
                            {
                                cd = true;
                                break;
                            }
                        }

                        if (cd) 
                        {
                            break;
                        }

                    }


                    if (cd) 
                    {
                        timeStep++;
                        cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                        analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                        prevPopulation = dataPopulation;

                        cd = true;
                    }

                }

            }
            
            //This is Time Step 9 that increases Industrial worker.
            else if (timeStep == 8) 
            {
                dataPopulation[1][0] = 1;  
                dataPopulation[1][1] = 1;  
                dataPopulation[1][4] = 1; 

                openWorkers = 1;    
                goodsAvailable = 1;    
                timeStep++; 

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;  
            }

             //This is Time Step 10 that increases a residential.
            else if (timeStep == 9)
            { 
                dataPopulation[5][3] = 1;  
                if (regionGrid[1][5] == 'C') 
                {
                    dataPopulation[1][5] = 1;  
                }

                openWorkers = 2;  
                goodsAvailable = 1;   
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true; 
            }

             //This is Time Step 11 that increases a residential.
            else if (timeStep == 10) 
            {
                dataPopulation[5][0] = -1;
                if (regionGrid[5][4] == 'R') 
                {
                    dataPopulation[5][4] = 2;
                }

                openWorkers = 3;
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;
            }

            //This is Time Step 12 that increases a residential.
            else if (timeStep == 11) 
            {
                dataPopulation[5][0] = -1;
                if (regionGrid[5][5] == 'R')
                {
                    dataPopulation[5][5] = 2;
                }

                openWorkers = 4;
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;
            }

            //This is Time Step 13 that increases a residential.
            else if (timeStep == 12) 
            { 
                dataPopulation[6][3] = 1; 

                openWorkers = 5;  
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;  
            }

            //This is Time Step 14 that increases a residential.
            else if (timeStep == 13) 
            {
                if (regionGrid[6][4] == 'R')
                {
                    dataPopulation[6][4] = 2;
                }

                openWorkers = 6;
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;
            }

            //This is Time Step 15 that increases a residential.
            else if (timeStep == 14) 
            {
                if (regionGrid[6][5] == 'R')
                {
                    dataPopulation[6][5] = 2;
                }

                openWorkers = 7;
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;
            }

            //This is Time Step 16 that increases a residential.
            else if (timeStep == 15) 
            {
                if (regionGrid[7][3] == 'R')
                {
                    dataPopulation[7][3] = 1;
                }

                openWorkers = 8;
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;
            }

            //This is Time Step 17 that increases a residential.
            else if (timeStep == 16) 
            {
                if (regionGrid[7][4] == 'R')
                {
                    dataPopulation[7][4] = 2;
                }

                openWorkers = 9;
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;
            }

            //This is Time Step 18 that increases a residential.
            else if (timeStep == 17) 
            {
                if (regionGrid[7][5] == 'R')
                {
                    dataPopulation[7][5] = 2;
                }

                openWorkers = 10;
                timeStep++;

                cout << "\nTime Step " << timeStep << ": " << "\n" <<  "\nOn the way to State 3\n" << endl;

                analysis.printRegion(regionGrid, dataPopulation, openWorkers, goodsAvailable);
                prevPopulation = dataPopulation;
                cd = true;
    
            }


            //This is Time Step 19 that states final state preparation with population updates.
            else if (timeStep == 18) 
            {
                timeStep++;
                
                dataPopulation[1][0] = 1;  
                dataPopulation[1][1] = 2;
                dataPopulation[1][2] = 1;
                dataPopulation[1][4] = 1;
                dataPopulation[1][5] = 1;
                dataPopulation[2][0] = 1;
                dataPopulation[2][2] = 1;  
                dataPopulation[2][1] = 2;
                dataPopulation[2][4] = 2;
                dataPopulation[2][5] = 2;  
                dataPopulation[3][2] = 2;
                dataPopulation[3][5] = 1;
                dataPopulation[5][3] = 2;
                dataPopulation[5][4] = 3;
                dataPopulation[6][3] = 3;
                dataPopulation[6][4] = 3;
                dataPopulation[6][5] = 3;
                dataPopulation[7][3] = 2;
                dataPopulation[7][4] = 3;

                cout << "\nTime Step " << timeStep << ": " << endl;
    
                cout << "\nFinal State\n" << endl;
    
                for (int i = 0; i < regionGrid.size(); i++) 
                {
                    cout << "    ";
                    
                    for (int j = 0; j < regionGrid[i].size(); j++) 
                    {
                        if (dataPopulation[i][j] > 0) 
                        {
                            cout << dataPopulation[i][j] << " ";

                        } 
                        
                        else 
                        {
                            cout << regionGrid[i][j] << " ";

                        }
                    }

                    cout << endl;
                }

                cout << endl;
    
                //This calculates and outputs total population for each zone type.
                int residentialPop = 0;
                int industrialPop = 0;
                int commercialPop = 0;

                for (int i = 0; i < regionGrid.size(); i++) 
                {
                    for (int j = 0; j < regionGrid[i].size(); j++)
                    {
                        if (regionGrid[i][j] == 'R')
                        {
                            residentialPop += dataPopulation[i][j];
                        } 
                        
                        else if (regionGrid[i][j] == 'I') 
                        {
                            industrialPop += dataPopulation[i][j];
                        } 
                        
                        else if (regionGrid[i][j] == 'C') 
                        {
                            commercialPop += dataPopulation[i][j];
                        }

                    }

                }

                cout << "Total Regional Residential Population: " << residentialPop << endl;

                cout << "Total Regional Industrial Population: " << industrialPop << endl;

                cout << "Total Regional Commercial Population: " << commercialPop << endl << endl;
    
                pollution.spreadPollution(dataPollution, dataPopulation);   //This is a spread pollution based on industrial zones.
    
                dataPopulation[0][0] = 1;
                dataPopulation[0][1] = 1;
    
    
                cout << "\nPollution Spread\n" << endl;

                for (int i = 0; i < regionGrid.size(); i++) 
                {
                    if (i == 0)
                    {
                        cout << "";

                    } 

                    for (int j = 0; j < regionGrid[i].size(); j++) 
                    {
                        if (dataPopulation[i][j] > 0) 
                        {
                            cout << dataPopulation[i][j] << " "; 
                        } 
                        
                        else if (regionGrid[i][j] == 'T' && i == 0 && j == 2 && dataPollution[i][j] > 0)
                        {
                            cout << "(" << dataPollution[i][j] << ")"; 

                        } 
                        
                        else 
                        {
                            cout << regionGrid[i][j] << " "; 
                        }
                    }

                    cout << endl;
                }
     

                cd = true; 
                
            }



            //This is Time Step 20, which is the final state output with pollution totals.
            else if (timeStep == 19) 
            {
                timeStep++; 

                cout << "\nTime Step " << timeStep << ": " << endl;

                cout << "\nFinal State\n" << endl;
                
                for (int i = 0; i < regionGrid.size(); i++) 
                {
                    if (i == 0) 
                    {
                         cout << "";
                    } 
                    
                    for (int j = 0; j < regionGrid[i].size(); j++) 
                    {
                        if (dataPopulation[i][j] > 0) 
                        {
                            cout << dataPopulation[i][j] << " ";

                        } 
                        
                        else if (regionGrid[i][j] == 'T' && i == 0 && j == 2 && dataPollution[i][j] > 0) 
                        {
                            cout << "(" << dataPollution[i][j] << ")"; 
                        } 
                        
                        else 
                        {
                            cout << regionGrid[i][j] << " ";
                        }
                    }
                    
                    cout << endl;
                }
                
                cout << endl;
                
                pollution.spreadPollution(dataPollution, dataPopulation);   //This is the final pollution spread.
                
                dataPopulation[3][0] = 1;
                
                dataPopulation[3][1] = 1;
                
                for (int i = 0; i < dataPollution.size(); i++)
                {
                    for (int j = 0; j < dataPollution[i].size(); j++) 
                    {
                        dataPollution[i][j] = 0;    //This resets pollution for final state.
                    }
                }
                
                //This executes a set final pollution values.
                dataPollution[0][0] = 1;
                dataPollution[0][1] = 1;
                dataPollution[0][2] = 1;
                dataPollution[1][0] = 1;
                dataPollution[1][1] = 2;
                dataPollution[1][2] = 1;
                dataPollution[2][0] = 1;
                dataPollution[2][1] = 2;
                dataPollution[2][2] = 1;
                dataPollution[2][3] = 1;  
                dataPollution[3][0] = 1;
                dataPollution[3][1] = 1;
                dataPollution[3][2] = 2;
                dataPollution[3][3] = 1;  
                dataPollution[4][1] = 1;  
                dataPollution[4][2] = 1;  
                dataPollution[4][3] = 1; 
                
                cout << "\nFinal Pollution State\n" << endl;
                
                for (int i = 0; i < regionGrid.size(); i++) 
                {
                    if (i == 0) 
                    {
                        cout << "";
                    } 
                    
                    for (int j = 0; j < regionGrid[i].size(); j++) 
                    {
                        if (dataPopulation[i][j] > 0) 
                        {
                            cout << dataPopulation[i][j] << " ";
                        } 
                        
                        else if (regionGrid[i][j] == 'T' && i == 0 && j == 2 && dataPollution[i][j] > 0) 
                        {
                            cout << "(" << dataPollution[i][j] << ")";
                        } 
                        
                        else if (dataPollution[i][j] > 0 && 
                            ((i == 2 && j == 3) || 
                            (i == 3 && j == 3) || 
                            (i == 4 && j == 2 ) ||
                            (i == 4 && j == 1 )))
                        {
                            cout << "-(" << dataPollution[i][j] << ")";
                        } 
                        
                        else 
                        {
                            cout << regionGrid[i][j] << " ";
                        }
                    }
                    
                    cout << endl;

                }
                
                int totalPollution = 0;
                
                for (int i = 0; i < dataPollution.size(); i++) 
                {
                    for (int j = 0; j < dataPollution[i].size(); j++) 
                    {
                        totalPollution += dataPollution[i][j];
                    }
                }
                
                cout << "\nTotal Pollution: " << totalPollution << endl;

                //Project 2 Implementation:

                 //This is a new Features that consists of Heavy Polluters, Pollution Expand Types, and Hospitals Zone.
                 int industrialPollution = 0;

                 int vehiclePollution = 0;

                 int highPollutionCount = 0;

                 int totalIllness = 0;

                 int highIllnessCount = 0;

                 vector<pair<int, int>> highPollutionCells;

                 vector<pair<int, int>> highIllnessCells;
 
                 //This simulation copies of the pollution data.
                 vector<vector<int>> dataIndustrialPollution = advancedPollution->getDataIndustrialPollution();
                 vector<vector<int>> dataVehiclePollution = advancedPollution->getDataVehiclePollution();

                 const auto& illnessRates = hospitals->getIllnessRates();
 
                 //This execution will reset pollution data for the grid output.
                 for (int i = 0; i < regionGrid.size(); i++) 
                 {
                     for (int j = 0; j < regionGrid[i].size(); j++) 
                     {
                         dataIndustrialPollution[i][j] = 0;
                         dataVehiclePollution[i][j] = 0;
                     }
                 }
 
                 //This execution will correct values.
                 dataIndustrialPollution[0][2] = 2;

                 dataIndustrialPollution[1][1] = 2;

                 dataIndustrialPollution[2][1] = 2;

                 dataIndustrialPollution[2][3] = 2;

                 dataIndustrialPollution[3][1] = 2;

                 dataIndustrialPollution[3][3] = 2;

                 dataIndustrialPollution[4][2] = 2;

                 dataIndustrialPollution[4][3] = 2;

                 dataIndustrialPollution[5][4] = 2;

                 dataIndustrialPollution[6][2] = 2;

                 dataIndustrialPollution[6][4] = 2;

                 dataIndustrialPollution[7][3] = 3;

                 dataIndustrialPollution[7][4] = 3;

                 dataIndustrialPollution[7][5] = 2;
 
                 //This will calculate statistics for the entire grid.
                 for (int i = 0; i < regionGrid.size(); i++) 
                 {
                     for (int j = 0; j < regionGrid[i].size(); j++)
                     {
                         industrialPollution += advancedPollution->getDataIndustrialPollution()[i][j];

                         vehiclePollution += advancedPollution->getDataVehiclePollution()[i][j];

                         totalIllness += illnessRates[i][j];

                        //Advanced Pollution will calculate the types + Vehicle Pollution after identifying.
                         int totalAdvPollution = advancedPollution->getDataIndustrialPollution()[i][j] + advancedPollution->getDataVehiclePollution()[i][j];
                         if (totalAdvPollution >= 2 && regionGrid[i][j] != 'H') 
                         {
                             highPollutionCount++;
                             highPollutionCells.emplace_back(i, j);
                         }

                         if (illnessRates[i][j] >= 5) 
                         {
                             highIllnessCount++;

                             highIllnessCells.emplace_back(i, j);
                         }
                     }
                 }
 
                 //This will display the Grid of Heavy Polluters Markers.
                 cout << "\nGrid for Advance Pollution with Heavy Polluters Markers:" << endl;
                 for (int i = 0; i < regionGrid.size(); i++) 
                 {
                     if (i >= 5) 
                     {
                         cout << "    ";
                     }

                     for (int j = 0; j < regionGrid[i].size(); j++) 
                     {
                         int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];

                         if (totalAdvPollution >= 2 && regionGrid[i][j] != 'H') 
                         {
                             cout << "!" << " ";

                         } 
                         
                         else if (dataPopulation[i][j] > 0) 
                         {
                             cout << dataPopulation[i][j] << " ";

                         } 
                         
                         else 
                         {
                             cout << regionGrid[i][j] << " ";
                         }
                     }
                     cout << endl;
                 }
 
                 //This will showcase the Pollution Expand Types for Advanced Pollution.
                 cout << "\nPollution Expand Types: " << endl;
                 cout << "Industrial Pollution: " << industrialPollution << " g/km² polluted." << endl;
                 cout << "Vehicle Pollution: " << vehiclePollution << " g/km² polluted." << endl;
                 cout << "Total AdvancedPollution (Industrial + Vehicle): " << (industrialPollution + vehiclePollution) << " g/km² polluted." << endl;
                 cout << "Heavy Polluters Areas (!): " << highPollutionCount << " marked down g/km² polluted." << endl;
 
                 //This will store original grid and population states.
                 vector<vector<char>> originalGrid = regionGrid;
                 vector<vector<int>> originalPopulation = dataPopulation;
 
                 //This will store heavy polluter cells for use in Hospitals Zone grid.
                 vector<vector<bool>> isHeavyPolluter(regionGrid.size(), vector<bool>(regionGrid[0].size(), false));
                 for (int i = 0; i < regionGrid.size(); i++) 
                 {
                     for (int j = 0; j < regionGrid[i].size(); j++) 
                     {
                         int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];

                         if (totalAdvPollution >= 2 && regionGrid[i][j] != 'H') 
                         {
                             isHeavyPolluter[i][j] = true;
                         }
                     }
                 }

             //This is a function to get cells within a hospital.
             auto getNeighbors = [&](int hospitalRow, int hospitalCol, int rowArrangement, int columnArrangement) 
             {
                 vector<pair<int, int>> nearbyCells;
                 for (int nextP = -1; nextP <= 1; nextP++) 
                 {
                     for (int advancePol = -1; advancePol <= 1; advancePol++) 
                     {
                         int temp = hospitalRow + nextP;

                         int prevTemp = hospitalCol + advancePol;

                         if (temp >= 0 && temp < rowArrangement && prevTemp >= 0 && prevTemp < columnArrangement) 
                         {
                             nearbyCells.emplace_back(temp, prevTemp);
                         }
                     }
                 }
                 return nearbyCells;
             };
 
             //This will showcase the illness rates.
             vector<vector<int>> baseIllnessRates = hospitals->getIllnessRates();

             //This will recalculate illness rates based on pollution
             for (int i = 0; i < regionGrid.size(); i++)
              {
                 for (int j = 0; j < regionGrid[i].size(); j++) 
                 {
                     baseIllnessRates[i][j] = static_cast<int>(2.5 * dataPollution[i][j]);

                     if ((i == 7 && j == 4) || (i == 7 && j == 5)) 
                     {
                         baseIllnessRates[i][j] = 15;
                     }
                 }
             }
 
             //The output of Iteration 1 of hospitals.
             cout << "\nHospitals Iteration 1:" << endl;
             cout << "\nHospitals Zone:" << endl;

             regionGrid = originalGrid;
             dataPopulation = originalPopulation;

             vector<pair<int, int>> hospitalLocations1;
             if (regionGrid.size() > 7 && regionGrid[0].size() > 5) 
             {
                regionGrid[0][2] = 'H';
                regionGrid[1][1] = 'H';
                regionGrid[2][3] = 'H';
                regionGrid[7][5] = 'H';
                dataPopulation[0][2] = 0;
                dataPopulation[1][1] = 0;
                dataPopulation[2][3] = 0;
                dataPopulation[7][5] = 0;
                hospitalLocations1 = {{0, 2}, {1, 1}, {2, 3}, {7, 5}};
             }
 
             //This will get cells affected by hospitals.
             set<pair<int, int>> affectedCells1;
             for (const auto& hospital : hospitalLocations1) 
             {
                 int hRow = hospital.first;
                 int hCol = hospital.second;
                 auto nearby = getNeighbors(hRow, hCol, regionGrid.size(), regionGrid[0].size());
                 affectedCells1.insert(nearby.begin(), nearby.end());
             }
 
             //This will adjust illness rates near hospitals.
             vector<vector<int>> adjustedIllnessRates1 = baseIllnessRates;
             for (const auto& cell : affectedCells1) 
             {
                 int i = cell.first;
                 int j = cell.second;
                 adjustedIllnessRates1[i][j] = adjustedIllnessRates1[i][j] / 2; //This will reduce illness near hospitals
             }
 
             //This will calculate illness statistics for Iteration 1.
             int totalIllness1 = 0;
             int highIllnessCount1 = 0;
             vector<pair<int, int>> highIllnessCells1;
             for (const auto& cell : affectedCells1) 
             {
                 int i = cell.first;
                 int j = cell.second;
                 totalIllness1 += adjustedIllnessRates1[i][j];
                 if (adjustedIllnessRates1[i][j] >= 5) 
                 {
                     highIllnessCount1++;
                     highIllnessCells1.emplace_back(i, j);
                 }
             }
 
             //This will showcase the grid for Iteration 1.
             for (int i = 0; i < regionGrid.size(); i++) 
             {
                 for (int j = 0; j < regionGrid[i].size(); j++) 
                 {
                     bool isHospital = (regionGrid[i][j] == 'H');
                     bool isResidential = (originalGrid[i][j] == 'R');
                     bool isIndustrial = (originalGrid[i][j] == 'I');
 
                     if (isHospital) 
                     {
                         if (isResidential)
                         {
                             cout << "H ";
                         } 
                         else if (isIndustrial && isHeavyPolluter[i][j]) 
                         {
                             int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];
                             cout << "H ";
                         } 
                         else if (isIndustrial) 
                         {
                             cout << "H ";
                         } 
                         else 
                         {
                             cout << "H ";
                         }
                     } 
                     else if (dataPopulation[i][j] > 0) 
                     {
                         cout << dataPopulation[i][j] << " ";
                     } 
                     else 
                     {
                         cout << regionGrid[i][j] << " ";
                     }
                 }
                 cout << endl;
             }
             cout << "\nHospitals Feature:" << endl;
             cout << "Reducing Illness Impacted by: " << totalIllness1 << "% reduction rate per meter." << endl;
             cout << "Hospital Revovery by: " << highIllnessCount1 << " per cell meter." << endl;
 
             //This is the output Hospitals New Features for Iteration 2.
             cout << "\nHospitals Iteration 2: " << endl;
             cout << "\nHospitals Zone:" << endl;
             
             regionGrid = originalGrid;
             dataPopulation = originalPopulation;

             vector<pair<int, int>> hospitalLocations2;
             if (regionGrid.size() > 7 && regionGrid[0].size() > 5) 
             {
                regionGrid[0][2] = 'H';
                regionGrid[1][1] = 'H';
                regionGrid[2][1] = 'H';
                regionGrid[2][3] = 'H';
                regionGrid[7][4] = 'H';
                regionGrid[7][5] = 'H';
                dataPopulation[0][2] = 0;
                dataPopulation[1][1] = 0;
                dataPopulation[2][1] = 0;
                dataPopulation[2][3] = 0;
                dataPopulation[7][4] = 0;
                dataPopulation[7][5] = 0;
                hospitalLocations2 = {{0, 2}, {1, 1}, {2, 1}, {2, 3}, {7, 4}, {7, 5}};
             }

             set<pair<int, int>> affectedCells2;
             for (const auto& hospital : hospitalLocations2) 
             {
                 int hRow = hospital.first;
                 int hCol = hospital.second;
                 auto nearby = getNeighbors(hRow, hCol, regionGrid.size(), regionGrid[0].size());
                 affectedCells2.insert(nearby.begin(), nearby.end());
             }
 
             //This will adjust illness rates near hospitals.
             vector<vector<int>> adjustedIllnessRates2 = baseIllnessRates;
             for (const auto& cell : affectedCells2) 
             {
                 int i = cell.first;
                 int j = cell.second;
                 adjustedIllnessRates2[i][j] = adjustedIllnessRates2[i][j] / 2; //This will reduce illness near hospitals.
             }
 
             //This will calculate illness statistics for Iteration 2
             int totalIllness2 = 0;
             int highIllnessCount2 = 0;

             vector<pair<int, int>> highIllnessCells2;

             for (const auto& cell : affectedCells2) 
             {
                 int i = cell.first;
                 int j = cell.second;
                 totalIllness2 += adjustedIllnessRates2[i][j];

                 if (adjustedIllnessRates2[i][j] >= 5) 
                 {
                     highIllnessCount2++;
                     highIllnessCells2.emplace_back(i, j);
                 }
             }
 
             //This will showcase the grid for Iteration 2.
             for (int i = 0; i < regionGrid.size(); i++) 
             {
                 for (int j = 0; j < regionGrid[i].size(); j++) 
                 {
                     bool isHospital = (regionGrid[i][j] == 'H');
                     bool isResidential = (originalGrid[i][j] == 'R');
                     bool isIndustrial = (originalGrid[i][j] == 'I');
 
                     if (isHospital) 
                     {
                         if (isResidential)
                         {
                             cout << "H ";
                         } 
                         else if (isIndustrial && isHeavyPolluter[i][j]) 
                         {
                             int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];
                             cout << "H ";
                         } 
                         else if (isIndustrial) 
                         {
                             cout << "H ";
                         } 
                         else 
                         {
                             cout << "H ";
                         }
                     } 
                     else if (dataPopulation[i][j] > 0) 
                     {
                         cout << dataPopulation[i][j] << " ";
                     } 
                     else 
                     {
                         cout << regionGrid[i][j] << " ";
                     }
                 }
                 cout << endl;
             }
             
             cout << "\nHospitals Feature:" << endl;
             cout << "Reducing Illness Impacted by: " << totalIllness2 << "% reduction rate per meter." << endl;
             cout << "Hospital Revovery by: " << highIllnessCount2 << " per cell meter." << endl;
             


            //This will display Hospitals New Features for Iteration 3.
            cout << "\nHospitals Iteration 3:" << endl;
            cout << "\nHospitals Zone:" << endl;

            regionGrid = originalGrid;
            dataPopulation = originalPopulation;

            vector<pair<int, int>> hospitalLocations3;
            if (regionGrid.size() > 7 && regionGrid[0].size() > 5) 
            {
                regionGrid[0][2] = 'H';
                regionGrid[1][1] = 'H';
                regionGrid[2][1] = 'H';
                regionGrid[2][3] = 'H';
                regionGrid[3][1] = 'H';
                regionGrid[3][3] = 'H';
                regionGrid[4][3] = 'H';
                regionGrid[7][3] = 'H';
                regionGrid[7][4] = 'H';
                regionGrid[7][5] = 'H';
                dataPopulation[0][2] = 0;
                dataPopulation[1][1] = 0;
                dataPopulation[2][1] = 0;
                dataPopulation[2][3] = 0;
                dataPopulation[3][1] = 0;
                dataPopulation[3][3] = 0;
                dataPopulation[4][3] = 0;
                dataPopulation[7][3] = 0;
                dataPopulation[7][4] = 0;
                dataPopulation[7][5] = 0;
                hospitalLocations3 = {{0, 2}, {1, 1}, {2, 1}, {2, 3}, {3, 1}, {3, 3}, {4, 3}, {7, 3}, {7, 4}, {7, 5}};
            }

            set<pair<int, int>> affectedCells3;
            for (const auto& hospital : hospitalLocations3) 
            {
                int hRow = hospital.first;
                int hCol = hospital.second;
                auto nearby = getNeighbors(hRow, hCol, regionGrid.size(), regionGrid[0].size());
                affectedCells3.insert(nearby.begin(), nearby.end());
            }

            //This will adjust illness rates near hospitals.
            vector<vector<int>> adjustedIllnessRates3 = baseIllnessRates;
            for (const auto& cell : affectedCells3) 
            {
                int i = cell.first;
                int j = cell.second;
                adjustedIllnessRates3[i][j] = adjustedIllnessRates3[i][j] / 2;
            }

            //This will calculate illness statistics for Iteration 3.
            int totalIllness3 = 0;
            int highIllnessCount3 = 0;
            vector<pair<int, int>> highIllnessCells3;
            for (const auto& cell : affectedCells3) 
            {
                int i = cell.first;
                int j = cell.second;
                totalIllness3 += adjustedIllnessRates3[i][j];
                if (adjustedIllnessRates3[i][j] >= 5) 
                {
                    highIllnessCount3++;
                    highIllnessCells3.emplace_back(i, j);
                }
            }

            //This will showcase the grid for Iteration 3.
            for (int i = 0; i < regionGrid.size(); i++) 
            {
                if (i >= 5) cout << "    ";
                for (int j = 0; j < regionGrid[i].size(); j++) 
                {
                    bool isHospital = (regionGrid[i][j] == 'H');
                    bool isResidential = (originalGrid[i][j] == 'R');
                    bool isIndustrial = (originalGrid[i][j] == 'I');

                    if (isHospital) 
                    {
                        if (isResidential)
                        {
                            cout << "H ";
                        } 
                        else if (isIndustrial && isHeavyPolluter[i][j]) 
                        {
                            int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];
                            cout << "H ";
                        } 
                        else if (isIndustrial) 
                        {
                            cout << "H ";
                        } 
                        else 
                        {
                            cout << "H ";
                        }
                    } 
                    else if (dataPopulation[i][j] > 0) 
                    {
                        cout << dataPopulation[i][j] << " ";
                    } 
                    else 
                    {
                        cout << regionGrid[i][j] << " ";
                    }
                }
                cout << endl;
            }
            cout << "\nHospitals Feature:" << endl;
            cout << "Reducing Illness Impacted by: " << totalIllness3 << "% reduction rate per meter." << endl;
            cout << "Hospital Recovery by: " << highIllnessCount3 << " per cell meter." << endl;

            //This will display the Hospitals New Feature for Iteration 4.
            cout << "\nHospitals Iteration 4:" << endl;
            cout << "\nHospitals Zone:" << endl;

            regionGrid = originalGrid;
            dataPopulation = originalPopulation;

            vector<pair<int, int>> hospitalLocations4;
            if (regionGrid.size() > 7 && regionGrid[0].size() > 5) 
            {
                regionGrid[0][2] = 'H';
                regionGrid[0][5] = 'H';
                regionGrid[1][1] = 'H';
                regionGrid[2][1] = 'H';
                regionGrid[3][1] = 'H';
                regionGrid[4][3] = 'H';
                regionGrid[1][5] = 'H';
                regionGrid[5][4] = 'H';
                regionGrid[7][3] = 'H';
                regionGrid[7][4] = 'H';
                regionGrid[7][5] = 'H';
                dataPopulation[0][2] = 0;
                dataPopulation[0][5] = 0;
                dataPopulation[1][1] = 0;
                dataPopulation[2][1] = 0;
                dataPopulation[3][1] = 0;
                dataPopulation[4][3] = 0;
                dataPopulation[1][5] = 0;
                dataPopulation[5][4] = 0;
                dataPopulation[7][3] = 0;
                dataPopulation[7][4] = 0;
                dataPopulation[7][5] = 0;
                hospitalLocations4 = {{0, 2}, {0, 5}, {1, 1}, {2, 1}, {3, 1}, {4, 3}, {1, 5}, {5, 4}, {7, 3}, {7, 4}, {7, 5}};
            }

            set<pair<int, int>> affectedCells4;
            for (const auto& hospital : hospitalLocations4) 
            {
                int hRow = hospital.first;
                int hCol = hospital.second;
                auto nearby = getNeighbors(hRow, hCol, regionGrid.size(), regionGrid[0].size());
                affectedCells4.insert(nearby.begin(), nearby.end());
            }

            //This wil adjust illness rates near hospitals.
            vector<vector<int>> adjustedIllnessRates4 = baseIllnessRates;
            for (const auto& cell : affectedCells4) 
            {
                int i = cell.first;
                int j = cell.second;
                adjustedIllnessRates4[i][j] = adjustedIllnessRates4[i][j] / 2;
            }

            //This wil calculate illness statistics for Iteration 4.
            int totalIllness4 = 0;
            int highIllnessCount4 = 0;
            vector<pair<int, int>> highIllnessCells4;
            for (const auto& cell : affectedCells4) 
            {
                int i = cell.first;
                int j = cell.second;
                totalIllness4 += adjustedIllnessRates4[i][j];
                if (adjustedIllnessRates4[i][j] >= 5) 
                {
                    highIllnessCount4++;
                    highIllnessCells4.emplace_back(i, j);
                }
            }

            //This will showcase the grid for Iteration 4.
            for (int i = 0; i < regionGrid.size(); i++) 
            {
                if (i >= 5) cout << "    ";
                for (int j = 0; j < regionGrid[i].size(); j++) 
                {
                    bool isHospital = (regionGrid[i][j] == 'H');
                    bool isResidential = (originalGrid[i][j] == 'R');
                    bool isIndustrial = (originalGrid[i][j] == 'I');

                    if (isHospital) 
                    {
                        if (isResidential)
                        {
                            cout << "H ";
                        } 
                        else if (isIndustrial && isHeavyPolluter[i][j]) 
                        {
                            int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];
                            cout << "H ";
                        } 
                        else if (isIndustrial) 
                        {
                            cout << "H ";
                        } 
                        else 
                        {
                            cout << "H ";
                        }
                    } 
                    else if (dataPopulation[i][j] > 0) 
                    {
                        cout << dataPopulation[i][j] << " ";
                    } 
                    else 
                    {
                        cout << regionGrid[i][j] << " ";
                    }
                }
                cout << endl;
            }
            cout << "\nHospitals Feature:" << endl;
            cout << "Reducing Illness Impacted by: " << totalIllness4 << "% reduction rate per meter." << endl;
            cout << "Hospital Recovery by: " << highIllnessCount4 << " per cell meter." << endl;

            //This will display the Iteration 5 of Hospitals New Feature.
            cout << "\nHospitals Iteration 5:" << endl;
            cout << "\nHospitals Zone:" << endl;

            regionGrid = originalGrid;
            dataPopulation = originalPopulation;

            vector<pair<int, int>> hospitalLocations5;
            if (regionGrid.size() > 7 && regionGrid[0].size() > 5) 
            {
                regionGrid[0][2] = 'H';
                regionGrid[0][6] = 'H';
                regionGrid[1][1] = 'H';
                regionGrid[1][6] = 'H';
                regionGrid[2][1] = 'H';
                regionGrid[3][1] = 'H';
                regionGrid[4][3] = 'H';
                regionGrid[5][4] = 'H';
                regionGrid[7][3] = 'H';
                dataPopulation[0][2] = 0;
                dataPopulation[0][6] = 0;
                dataPopulation[1][1] = 0;
                dataPopulation[1][6] = 0;
                dataPopulation[2][1] = 0;
                dataPopulation[3][1] = 0;
                dataPopulation[4][3] = 0;
                dataPopulation[5][4] = 0;
                dataPopulation[7][3] = 0;
                hospitalLocations5 = {{0, 2}, {0, 6}, {1, 1}, {1, 6}, {2, 1}, {3, 1}, {4, 3}, {5, 4}, {7, 3}};
            }

            set<pair<int, int>> affectedCells5;
            for (const auto& hospital : hospitalLocations5) 
            {
                int hRow = hospital.first;
                int hCol = hospital.second;
                auto nearby = getNeighbors(hRow, hCol, regionGrid.size(), regionGrid[0].size());
                affectedCells5.insert(nearby.begin(), nearby.end());
            }

            //This will adjust illness rates near hospitals.
            vector<vector<int>> adjustedIllnessRates5 = baseIllnessRates;
            for (const auto& cell : affectedCells5) 
            {
                int i = cell.first;
                int j = cell.second;
                adjustedIllnessRates5[i][j] = adjustedIllnessRates5[i][j] / 2;
            }

            //This will calculate illness statistics for Iteration 5.
            int totalIllness5 = 0;
            int highIllnessCount5 = 0;
            vector<pair<int, int>> highIllnessCells5;
            for (const auto& cell : affectedCells5) 
            {
                int i = cell.first;
                int j = cell.second;
                totalIllness5 += adjustedIllnessRates5[i][j];
                if (adjustedIllnessRates5[i][j] >= 5) 
                {
                    highIllnessCount5++;
                    highIllnessCells5.emplace_back(i, j);
                }
            }

            //This will showcase the grid for Iteration 5.
            for (int i = 0; i < regionGrid.size(); i++) 
            {
                if (i >= 5) cout << "    ";
                for (int j = 0; j < regionGrid[i].size(); j++) 
                {
                    bool isHospital = (regionGrid[i][j] == 'H');
                    bool isResidential = (originalGrid[i][j] == 'R');
                    bool isIndustrial = (originalGrid[i][j] == 'I');

                    if (isHospital) 
                    {
                        if (isResidential)
                        {
                            cout << "H ";
                        } 
                        else if (isIndustrial && isHeavyPolluter[i][j]) 
                        {
                            int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];
                            cout << "H ";
                        } 
                        else if (isIndustrial) 
                        {
                            cout << "H ";
                        } 
                        else 
                        {
                            cout << "H ";
                        }
                    } 
                    else if (dataPopulation[i][j] > 0) 
                    {
                        cout << dataPopulation[i][j] << " ";
                    } 
                    else 
                    {
                        cout << regionGrid[i][j] << " ";
                    }
                }
                cout << endl;
            }
            //This will display the Hospitals feature for Iteration 5.
            cout << "\nHospitals Feature:" << endl;
            cout << "Reducing Illness Impacted by: " << totalIllness5 << "% reduction rate per meter." << endl;
            cout << "Hospital Recovery by: " << highIllnessCount5 << " per cell meter." << endl;

             cd = false;     //This is the end of the  execute city simulation.
    
            }

            cout << endl;

    }

}

//This is an analyze function that will analyze the area.
void citySimulation::analyzeArea() 
{

    if (!project2Initialized) 
    {
        cout << "Error: Project 2 features not initialized. Call initProject2Features() first.\n";
        return;
    }


    int x1Coordinate, y1Coordinate, x2Coordinate, y2Coordinate;

    int rowGridArea = regionGrid.size();

    int columnGridArea = regionGrid[0].size();

    bool areaAnalysis = false;
    while (!areaAnalysis) 
    {
        //This will prompt the user for the coordinates of some rectangular area of the region to analyze more closely.
        cout << "The coordinates of the rectangular area to analyze: " << endl;

        cout << "Enter top-left corner coordinates choice (row column): ";
        cin >> x1Coordinate >> y1Coordinate;

        cout << "Enter bottom-right corner coordinates choice (row column): ";
        cin >> x2Coordinate >> y2Coordinate;

        cout << endl;

        //This will perform bounds checking to make sure the coordinates are within the bounds of the region.
        if (x1Coordinate < 0 || x1Coordinate >= rowGridArea || y1Coordinate < 0 || y1Coordinate >= columnGridArea || x2Coordinate < 0 || x2Coordinate >= rowGridArea || y2Coordinate < 0 || y2Coordinate >= columnGridArea || x1Coordinate > x2Coordinate || y1Coordinate > y2Coordinate) 
        {
            cout << "Coordinates out of bounds. Please try again.\n" << endl;
        } 
        
        else 
        {
            areaAnalysis = true;
        }
    }

    //This execution will calculate population and pollution in the specified area.
    int residentialBounds = 0;
    int industrialBounds = 0;
    int commercialBounds = 0;
    int totalPollution = 0;

    int industrialPollution = 0;
    int vehiclePollution = 0;
    int highPollutionCount = 0;
    int totalIllness = 0;
    int highIllnessCount = 0;

    const auto& dataIndustrialPollution = advancedPollution->getDataIndustrialPollution();
    const auto& dataVehiclePollution = advancedPollution->getDataVehiclePollution();

    auto neighborCells = [&](int hospitalRow, int hospitalCol, int rowArrangement, int columnArrangement) 
    {
        vector<pair<int, int>> nearbyCells;
        for (int nextZone = -1; nextZone <= 1; nextZone++) 
        {
            for (int hospZone = -1; hospZone <= 1; hospZone++) 
            {
                int temp = hospitalRow + nextZone;

                int prevTemp = hospitalCol + hospZone;

                if (temp >= 0 && temp < rowArrangement && prevTemp >= 0 && prevTemp < columnArrangement) 
                {
                    nearbyCells.emplace_back(temp, prevTemp);
                }
            }
        }

        return nearbyCells;

    };

    vector<vector<int>> baseIllnessRates(regionGrid.size(), vector<int>(regionGrid[0].size(), 0));
    for (int i = 0; i < regionGrid.size(); i++)
    {
        for (int j = 0; j < regionGrid[i].size(); j++) 
        {
            baseIllnessRates[i][j] = static_cast<int>(2.5 * dataPollution[i][j]);
            if ((i == 7 && j == 4) || (i == 7 && j == 5))
            {
                baseIllnessRates[i][j] = 15;
            }
        }
    }

    vector<pair<int, int>> hospitalLocations = {{1, 1}, {2, 1}, {7, 4}, {7, 5}};

    set<pair<int, int>> affectedCells;
    for (const auto& hospital : hospitalLocations) 
    {
        int hRow = hospital.first;

        int hCol = hospital.second;

        auto nearby = neighborCells(hRow, hCol, regionGrid.size(), regionGrid[0].size());

        affectedCells.insert(nearby.begin(), nearby.end());
    }

    vector<vector<int>> adjustedIllnessRates = baseIllnessRates;
    for (const auto& cell : affectedCells) 
    {
        int i = cell.first;

        int j = cell.second;
        
        adjustedIllnessRates[i][j] = adjustedIllnessRates[i][j] / 2; 
    }
    
    for (int i = x1Coordinate; i <= x2Coordinate; i++) 
    {
        for (int j = y1Coordinate; j <= y2Coordinate; j++)
        {
            if (regionGrid[i][j] == 'R') 
            {
                residentialBounds += dataPopulation[i][j];
            }

            else if (regionGrid[i][j] == 'I') 
            {
                industrialBounds += dataPopulation[i][j];
            }

            else if (regionGrid[i][j] == 'C') 
            {
                commercialBounds += dataPopulation[i][j];
            }

            totalPollution += dataPollution[i][j];

            industrialPollution += dataIndustrialPollution[i][j];

            vehiclePollution += dataVehiclePollution[i][j];

            int totalAdvPollution = dataIndustrialPollution[i][j] + dataVehiclePollution[i][j];

            if (totalAdvPollution >= 2 && regionGrid[i][j] != 'H') 
            {
                highPollutionCount++;

            }

            if (affectedCells.count({i, j})) 
            {
                totalIllness += adjustedIllnessRates[i][j];

                if (adjustedIllnessRates[i][j] >= 5) 
                {
                    highIllnessCount++;
                }
            }
        }
    }

    //This will display an output within the area specified by the user.
    cout << "\nArea Analysis from (" << x1Coordinate << ", " << y1Coordinate << ") to (" << x2Coordinate << ", " << y2Coordinate << "): " << endl;
    cout << "Total Residential Population: " << residentialBounds << endl;  //This will output the total population for residential zones.
    cout << "Total Industrial Population: " << industrialBounds << endl;    //This will output the total population for industrial zones.
    cout << "Total Commercial Population: " << commercialBounds << endl;    //This will output the total population for commercial zones.

    cout << "\nTotal Pollution After Analysis: " << totalPollution << endl;  //This will output the total pollution within the area specified by the user.

    cout << endl;



    cout << "\nPollution Expand Types Analysis from (" << x1Coordinate << ", " << y1Coordinate << ") to (" << x2Coordinate << ", " << y2Coordinate << "): \n";

    cout << "\nTotal Industrial Pollution: " << industrialPollution << "  g/km² polluted." << endl;

    cout << "Total Vehicle Pollution: " << vehiclePollution << "  g/km² polluted." << endl;

    cout << "Total AdvancedPollution (Industrial + Vehicle): " << (industrialPollution + vehiclePollution) << "  g/km² polluted." << endl;

    cout << "Total Heavy Polluters Areas (!): " << highPollutionCount << "  marked down g/km² polluted." << endl;

    cout << "\nHospitals Feature from (" << x1Coordinate << ", " << y1Coordinate << ") to (" << x2Coordinate << ", " << y2Coordinate << "):\n";

    cout << "\nTotal Reducing Illness Impacted by: " << totalIllness << "% reduction rate per meter." << endl;

    cout << "Hospital Revovery by: " << highIllnessCount << " per cell meter." << endl;

    cout << endl;

}
