
/* 
File Name: Init_Config.cpp

*/ 

#include "Init_Config.h"
using namespace std;

void initConfiguration::readConfig(const string &filename)
 {
    ifstream fin(filename);     //This will execute an opening and reading in Config file properly.
    if (fin.fail()) 
    {
        cout << "\nUnable to open the configuration file. Please use the correct configuration file (config1.txt). " << endl;
        exit(1);

        cout << endl;
    }

    string reglayoutFile;

    fin >> reglayoutFile >> region1CSVFile;      //This will read the region layout of region1.csv.

    fin >> reglayoutFile >> timeLimit;       //This will read the time Limit: 20.

    fin >> reglayoutFile >> refreshRate;     //This will read the refresh rate: 1.


    size_t onlyContain = region1CSVFile.find(":");  //This execution ensures regionFile only contains the actual file.

    if (onlyContain != string::npos) 
    {
        region1CSVFile = region1CSVFile.substr(onlyContain + 1);  //This execution will extracts the file.

    }

   
    region1CSVFile.erase(0, region1CSVFile.find_first_not_of(" "));  //This execution will cut any leading spaces shown.


    fin.close();         //This closes the configuration file after reading.
}
