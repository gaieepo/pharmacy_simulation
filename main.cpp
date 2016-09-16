#include "System.h"
#include <iostream>
#include <cstdlib>
#include <vector>

void optimization(int tech_num, int simulate_num, double total_service_minutes) {
	std::vector<std::vector<int>> combinations;
	double min = 10000;
	System system(total_service_minutes, tech_num);
	for (int reg  = 1; reg != 3; ++reg) {
		for (int pay = 1; pay != 3; ++pay) {
			for (int flx = 0; flx != 5; ++flx) {
				for (int che = 1; che != 5; ++che) {
					int pac = tech_num - reg - pay - flx - che;
					if (pac > 0) {
						system.setTechAllocation(reg, pac, flx, che, pay);
						system.simulate(simulate_num);
						if (system.getAvgStayMinutes() < min) {
							min = system.getAvgStayMinutes();
							combinations.clear();
							std::vector<int> temp{reg, pac, flx, che, pay};
							combinations.push_back(temp);
						} else if (system.getAvgStayMinutes() == min) {
							std::vector<int> temp{reg, pac, flx, che, pay};
							combinations.push_back(temp);
						}
						std::cout << "Tried :" << reg << " " << pac << " " << flx << " " << che << " " << pay << " Use: " << system.getAvgStayMinutes() << std::endl;
					}
				}
			}
		}
	}
	// std::cout << "Min avg stay minutes: " << min << std::endl;
	// std::cout << "--------------------------" << std::endl;
	for (auto v : combinations) {
		for (int i: v) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}
}

void test(int tech_num, int reg, int pac, int flx, int che, int pay, int simulate_num, double total_service_minutes) {
	System system(total_service_minutes, tech_num);
	system.setTechAllocation(reg, pac, flx, che, pay);
	system.simulate(simulate_num);
}

int main() {
	double total_service_minutes = 540;
	
	int tech_num = 11;
	int simulate_num = 10;

	// test(tech_num, 2, 3, 2, 3, 1, simulate_num, total_service_minutes);
	optimization(tech_num, simulate_num, total_service_minutes);
}