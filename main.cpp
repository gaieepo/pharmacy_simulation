#include "System.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <fstream>

std::string getFileContents (std::ifstream&);            //Gets file contents

double calAvg(std::vector<double> &candTime) {
    auto max = std::max_element(candTime.begin(), candTime.end());
    auto min = std::min_element(candTime.begin(), candTime.end());
    double sum = 0;
    for (auto it = candTime.begin(); it != candTime.end(); ++it) {
        if (it != max && it != min) {
            sum += *it;
        }
    }
    return sum/8;
}

int main() {
	double total_service_minutes = 60 * 9;
	
	int tech_num = 11;

	int simulate_num = 10;

	int break_schedule[9][2] = {
		{11, 4},
		{11, 4},
		{9, 3},
		{11, 4},
		{7, 3},
		{7, 3},
		{11, 4},
		{11, 4},
		{11, 4}
	};

	System system(total_service_minutes, 11);
	system.setTechAllocation(2, 4, 0, 4, 1); // 7, 4
	system.setReschedule(2, 4, 0, 4, 1, 60); // 7, 4
	system.setReschedule(2, 3, 0, 3, 1, 120); // 6, 3
	system.setReschedule(2, 4, 0, 4, 1, 180); // 7, 4
	system.setReschedule(1, 2, 0, 3, 1, 240); // 4, 3
	system.setReschedule(1, 2, 0, 3, 1, 300); // 4, 3
	system.setReschedule(2, 4, 0, 3, 1, 360); // 7, 3
	system.setReschedule(2, 4, 0, 4, 1, 420); // 7, 4
	system.setReschedule(2, 4, 0, 4, 1, 480); // 7, 4

	
    std::ifstream Reader ("banner.txt");             //Open file
    std::string Art = getFileContents (Reader);       //Get file
    std::cout << Art << std::endl;               //Print it to the screen
    Reader.close ();                           //Close file

    bool sent = false;
    std::string input = "";
    while (input != "q") {
    	std::cout << "--------------------------\n";
    	std::cout << "- 1.Base case simulation -\n- 2.Change arrival rate  -\n--------------------------\nEnter choice: ";
    	std::cin >> input;
    	if (input == "1") {
    		system.simulate(simulate_num);
    		std::cout << "Overall average staying time: " << system.getAvgStayMinutes() << std::endl;
    	}
    	else if (input == "2") {
    		if (!sent) {
	    		std::cout << "Input arrival rate: " << std::endl;
	    		std::cout << "The simulation package has sent to NSCC for computation..." << std::endl;
	    		sent = true;
	    	} else {
	    		std::cout << "You are only allowed to submit one simulation package at a time. Please be patient." << std::endl;
	    	}
    	}
    }
    std::cout << "Existing the simulation environment..." << std::endl;
    std::cout << "Thanks for your participation" << std::endl;
}

std::string getFileContents (std::ifstream& File)
{
    std::string Lines = "";        //All lines
    
    if (File)                      //Check if everything is good
    {
	while (File.good ())
	{
	    std::string TempLine;                  //Temp line
	    std::getline (File , TempLine);        //Get temp line
	    TempLine += "\n";                      //Add newline character
	    
	    Lines += TempLine;                     //Add newline
	}
	return Lines;
    }
    else                           //Return error
    {
	return "ERROR File does not exist.";
    }
}