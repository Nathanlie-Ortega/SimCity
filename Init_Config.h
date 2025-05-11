
/* 
File Name: Init_Config.h

*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class initConfiguration             //This is a class that will initialize the configuration.
{
private:

    string region1CSVFile;

    int timeLimit;              //This is a variable that stores the time limit.

    int refreshRate;            //This is a variable that stores the refresh rate.


public:
    void readConfig(const string &filename);        //This is a function to read the configuration file.

    string getRegionFile() const                    //This is a getter getRegionFile function to return to the region file.
    { 
        return region1CSVFile; 
    }

    int getTimeLimit() const    //A getter function to get the Time Limit.
    { 
        return timeLimit; 
    }

    int getRefreshRate() const  //A getter function to get the Refresh Rate.
    { 
        return refreshRate; 
    }
    
};
 