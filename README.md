# SimCity: README file by Nathanlie Ortega 

**Author**: Nathanlie Ortega


**Email**: NathanlieOrtega.Dev@gmail.com


# Short Brief Description:

This SimCity Final Submission is utilized in C++ using files, initializes and simulates a city layout. It reads configuration and region files into a grid, provides initial region-state output, and executes a full city simulation over multiple time steps, managing residential, commercial, industrial, pollution, and analysis transformations with final population and pollution outputs. The new features are implemented within the Advanced Pollution and Hospitals features.


# Files and Structure:

- `Init_Config.h` - States the initConfiguration class is reliable for reading config1.txt.

- `Init_Config.cpp` - Executes the configuration file, pulling region file name, time limit, and refresh rate.

- `City_Simulation.h` - States the citySimulation class is reliable for reading region1.csv with function declarations for simulation and area analysis.

- `City_Simulation.cpp` - Executes the whole city simulation steps along with the grid storage and initial region state output. Alongside, the new features will be displayed within Advanced Pollution and Hospitals.

- `Residential.h` - Declares the Residential class for residential zone growth rules and transformations.

- `Residential.cpp` - Implements residential zone population updates based on adjacency and powerline rules.

- `Commercial.h` - Declares the Commercial class for commercial zone growth rules and transformations.

- `Commercial.cpp` - Implements commercial zone population updates using workers and goods.

- `Industrial.h` - Declares the Industrial class for industrial zone growth rules and transformations.

- `Industrial.cpp` - Implements industrial zone population updates, worker consumption, and goods production.

- `Pollution.h` - Declares the Pollution class for pollution spread function.

- `Pollution.cpp` - Implements pollution spread function from industrial zones to adjacent cells.

- `Analysis.h` - Declares the Analysis class for region state output.

- `Analysis.cpp` - Implements region state printing.

- `main.cpp` - Manages user exchange, reads filenames, directs the initialization operation, runs the simulation as well as new features, and analysis area prompt.

- `config1.txt` - Configuration file.

- `region1.csv` - Region file representing the initial city layout.

SimCity New Features:

- `AdvancedPollution.h` - Declares the AdvancedPollution class for enhanced pollution mechanics, including vehicle pollution and heavy polluter identification.

- `AdvancedPollution.cpp` - Implements advanced pollution features, calculating vehicle pollution and identifying heavy polluters with pollution levels ≥ 5.

- `Hospitals.h` - Declares the Hospitals class for hospital zone mechanics, including illness rate calculations and hospital placement effects.

- `Hospitals.cpp` - Implements hospital features, managing hospital zones, reducing illness rates in nearby cells, and calculating recovery statistics across multiple iterations.


### How to Compile and Run:

1.) Open a Windows Powershell Terminal, Run as Administrator and navigate to the SimCity folder using:

cd SimCity


2.) Compile the program using:


g++ *.cpp       


OR         


Alternatively, you can use:  g++ main.cpp Init_Config.cpp City_Simulation.cpp Residential.cpp Commercial.cpp Industrial.cpp Pollution.cpp Analysis.cpp AdvancedPollution.cpp Hospitals.cpp


3.) Run the program using:

./a.exe


## What the output will do:
During execution, the program prompts the user to enter the configuration filename (`config1.txt`). It reads `config1.txt`, initializes the region using `region1.csv`, prints the initial region state, and then runs the simulation for 20 time steps with a refresh rate of 1. The output includes time step updates, region states, available workers and goods, final population totals for residential, commercial, and industrial zones, and the final pollution state with total pollution.


## What the FIRST Half output for SimCity will show:


```````````````````````````````````````````````````````
Enter configuration filename (config1.txt): config1.txt

Initial Region State:

    T # T T T   
I I I - C C T   
I I I - C C T P 
I I I - C C T   
- - - - - - # - 
    - R R R T   
    - R R R     
    - R R R     

Available Workers: 0
Available Goods: 0


Time Step 1:

On the way to State 1

    T # T T T
I I I - C C T
I I I - C C T P
I I I - C C T
- - - - - - # -
    - R R 1 T
    - R R R
    - R R R

Available Workers: 1
Available Goods: 0



Time Step 2:

State 1

    T # T T T
I I I - C C T
I I I - C C T P
I I I - C C T
- - - - - - # -
    - R R 1 T
    - R R 1
    - R R R

Available Workers: 2
Available Goods: 0



Time Step 3:

On the way to State 2

    T # T T T
I 1 I - C C T
I I I - C C T P
I I I - C C T
- - - - - - # -
    - R R 1 T
    - R R 1
    - R R R

Available Workers: 0
Available Goods: 1


(And So On....)


```````````````````````````````````````````````````````


## What the SECOND Half output for SimCity will show:

Grid for Advance Pollution with Heavy Polluters Markers:
1 1 ! # T T T   
1 ! 1 - 1 1 T   
1 ! 1 ! 2 2 T P 
1 ! 2 ! C 1 T   
- - ! ! - - # - 
        - 2 ! 2 T   
        ! 3 ! 3     
        - ! ! !     

Pollution Expand Types: 
Industrial Pollution: 29 g/km² polluted.
Vehicle Pollution: 166 g/km² polluted.
Total AdvancedPollution (Industrial + Vehicle): 195 g/km² polluted.
Heavy Polluters Areas (!): 47 marked down g/km² polluted.


Hospitals Iteration 1:

Hospitals Zone:
1 1 H # T T T   
1 H 1 - 1 1 T   
1 2 1 H 2 2 T P 
1 1 2 - C 1 T   
- - - - - - # - 
    - 2 3 2 T   
    - 3 3 3     
    - 2 3 H     

Hospitals Feature:
Reducing Illness Impacted by: 29% reduction rate per meter.
Hospital Revovery by: 2 per cell meter.

Hospitals Iteration 2: 

Hospitals Zone:
1 1 H # T T T   
1 H 1 - 1 1 T   
1 H 1 H 2 2 T P 
1 1 2 - C 1 T   
- - - - - - # - 
    - 2 3 2 T   
    - 3 3 3     
    - 2 H H     

Hospitals Feature:
Reducing Illness Impacted by: 31% reduction rate per meter.
Hospital Revovery by: 2 per cell meter.

Hospitals Iteration 3:

Hospitals Zone:
1 1 H # T T T   
1 H 1 - 1 1 T   
1 H 1 H 2 2 T P 
1 H 2 H C 1 T   
- - - H - - # - 
        - 2 3 2 T   
        - 3 3 3     
        - H H H     

Hospitals Feature:
Reducing Illness Impacted by: 34% reduction rate per meter.
Hospital Recovery by: 2 per cell meter.

Hospitals Iteration 4:

Hospitals Zone:
1 1 H # T H T   
1 H 1 - 1 H T   
1 H 1 - 2 2 T P 
1 H 2 - C 1 T   
- - - H - - # - 
        - 2 H 2 T   
        - 3 3 3     
        - H H H     

Hospitals Feature:
Reducing Illness Impacted by: 33% reduction rate per meter.
Hospital Recovery by: 2 per cell meter.

Hospitals Iteration 5:

Hospitals Zone:
1 1 H # T T H   
1 H 1 - 1 1 H   
1 H 1 - 2 2 T P 
1 H 2 - C 1 T   
- - - H - - # - 
        - 2 H 2 T   
        - 3 3 3     
        - H 3 2     

Hospitals Feature:
Reducing Illness Impacted by: 26% reduction rate per meter.
Hospital Recovery by: 1 per cell meter.


## Other Notes:
- Make sure that config1.txt and region1.csv files are in the same directory as the executable.
- If a file is missing, an error message will be displayed.
