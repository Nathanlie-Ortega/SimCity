

/* 
File Name: main.cpp

*/

#include <iostream>
#include "Init_Config.h"
#include "City_Simulation.h"

using namespace std;

int main() 
{
    	//A display of the title, my name, and e-mail address
        cout << endl;
        cout << "+------------------------------------------+" << endl;
        cout << "|            Sim City Project              |" << endl;
        cout << "|      Author name: Nathanlie Ortega       |" << endl;
        cout << "|      NathanlieOrtega.Dev@gmail.com       |" << endl;
        cout << "+------------------------------------------+\n" << endl;

    initConfiguration cityConfiguration;    //This is an object that handles configuration for the city.
    citySimulation citySim;                 //This is an object that handles the simulation of the city.

    
    string configFile;                      //This is a string to read the configuration file.

    cout << "\nEnter configuration filename (config1.txt): ";     //This will prompt user to enter the configuration file name which is: config1.txt.
    cin >> configFile;                          //This will read the configuration file.

    cityConfiguration.readConfig(configFile);

    
    citySim.initRegion(cityConfiguration.getRegionFile());     //This will initialize the simulation.
    citySim.initProject2Features();             //This will initialize SimCity features.

    
    cout << "\nInitial Region State:" << endl;       //This is a display that will print the Initial Region State.

    cout << endl;

    citySim.printInitialState();                    //This will print initial region state.

    int timeLimit = 20;                         //This is the time limit assigned as 20.
    int refreshRate = 1;                        //This is the refresh rate assigned as 1.
    citySim.executeCitySim(timeLimit, refreshRate);     //A function that execute the city simulation.
    citySim.analyzeArea();                 //A function to analyze the area.

    cout << endl;

    return 0;
} 
