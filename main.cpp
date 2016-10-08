#include "System.h"
#include <iostream>
#include <cstdlib>
#include <vector>

void optimization(int tech_num, int simulate_num, double total_service_minutes, double schedule[][2]) {
	std::vector<std::vector<int>> combinations;
	double min = 10000;
	System system(total_service_minutes, tech_num, schedule);

	// for (int reg = 1; reg <= tech_num; ++reg) {
	// 	for (int pac = 1; pac <= tech_num; ++pac) {
	// 		for (int flxche = 1; flxche <= tech_num; ++flxche) {
	// 			for (int flx = 0; flx <= flxche && flx < 3; ++flx) {
	// 				int che = flxche - flx;
	// 				int pay = tech_num - reg - pac - flx - che;
	// 				if (pay > 0) {
	// 					system.setTechAllocation(reg, pac, flx, che, pay);
	// 					system.simulate(simulate_num);
	// 					if (system.getAvgStayMinutes() < min) {
	// 						min = system.getAvgStayMinutes();
	// 						combinations.clear();
	// 						std::vector<int> temp{reg, pac, flx, che, pay};
	// 						combinations.push_back(temp);
	// 					} else if (system.getAvgStayMinutes() == min) {
	// 						std::vector<int> temp{reg, pac, flx, che, pay};
	// 						combinations.push_back(temp);
	// 					}
	// 					std::cout << "Tried :" << reg << " " << pac << " " << flx << " " << che << " " << pay << " Use: " << system.getAvgStayMinutes() << std::endl;
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	for (int reg  = 1; reg != 3; ++reg) {
		for (int pay = 1; pay != 3; ++pay) {
			for (int flxche = 1; flxche != 5; ++flxche) {
				for (int flx = 0; flx <= flxche && flx < 3; ++flx) {
					int che = flxche - flx;
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

	std::cout << "Min avg stay minutes: " << min << std::endl;
	std::cout << "--------------------------" << std::endl;
	for (auto v : combinations) {
		for (int i: v) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}
}

// void test(int tech_num, int reg, int pac, int flx, int che, int pay, int simulate_num, double total_service_minutes) {
// 	System system(total_service_minutes, tech_num);
// 	system.setTechAllocation(reg, pac, flx, che, pay);
// 	system.simulate(simulate_num);
// 	std::cout << system.getAvgStayMinutes() << std::endl;
// }

int main() {
	double total_service_minutes = 60 * 9;
	
	int tech_num = 11;
	int simulate_num = 30;

	double schedule[tech_num][2] = {
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
		{0, 8},
	};

	// 2 3 2 2 2
	// test(tech_num, 2, 3, 2, 2, 2, simulate_num, total_service_minutes);
	// test(tech_num, 2, 3, 2, 2, 2, simulate_num, total_service_minutes, schedule);
	optimization(tech_num, simulate_num, total_service_minutes, schedule);
}