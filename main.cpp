#include "System.h"
#include <iostream>
#include <cstdlib>
#include <vector>

// void test(int tech_num, int reg, int pac, int flx, int che, int pay, int simulate_num, double total_service_minutes) {
// 	System system(total_service_minutes, tech_num);
// 	system.setTechAllocation(reg, pac, flx, che, pay);
// 	system.simulate(simulate_num);
// 	std::cout << system.getAvgStayMinutes() << std::endl;
// }

int main() {
	double total_service_minutes = 60 * 9;
	
	int tech_num = 11;
	int simulate_num = 100;

	// 	{7, 4},	{7, 4},	{6, 3},	{7, 4},	{4, 3},	{4, 3},	{7, 4},	{7, 4},	{7, 4}
	int break_schedule[9] = {
		11,
		11,
		9,
		11,
		7,
		7,
		11,
		11,
		11,
	};

	// System system(total_service_minutes, 11);
	// system.setTechAllocation(2, 3, 0, 4, 2);
	// system.setReschedule(1, 2, 0, 2, 1, 60);
	// system.setReschedule(2, 3, 0, 4, 2, 120);
	// system.simulate(1);
	// system.clearReschedule();
	// std::cout << "--------------------------" << std::endl;
	// std::cout << "Min avg stay minutes: " << system.getAvgStayMinutes() << std::endl;


	std::vector<std::vector<int>> combinations;
	double min = 10000;
	System system(total_service_minutes, 11);

	for (int reg  = 1; reg != 3; ++reg) {
		for (int pay = 1; pay != 3; ++pay) {
			for (int flxche = 1; flxche != 5; ++flxche) {
				for (int flx = 0; flx <= flxche && flx < 3; ++flx) {
					int che = flxche - flx;
					int pac = 11 - reg - pay - flx - che;
					if (pac > 0) {
						// system.setTechAllocation(reg, pac, flx, che, pay);

						// system.setTechAllocation(1, 8, 1, 0, 1); // 7, 4
						// system.setReschedule(2, 6, 0, 1, 2, 60); // 7, 4
						// system.setReschedule(2, 5, 1, 0, 1, 120); // 6, 3
						// system.setReschedule(2, 6, 0, 1, 2, 180); // 7, 4
						// system.setReschedule(2, 2, 0, 1, 2, 240); // 4, 3
						// system.setReschedule(1, 3, 1, 0, 2, 300); // 4, 3
						// system.setReschedule(2, 4, 2, 1, 2, 360); // 7, 4
						// system.setReschedule(1, 7, 0, 1, 2, 420); // 7, 4
						// system.setReschedule(2, 6, 1, 0, 2, 480); // 7, 4
						// system.setReschedule(reg, pac, flx, che, pay, 60);

						system.setTechAllocation(2, 3, 0, 4, 2);
						system.setReschedule(2, 3, 0, 4, 2, 60);
						system.setReschedule(2, 2, 0, 3, 2, 120);
						system.setReschedule(2, 3, 0, 4, 2, 180);
						system.setReschedule(1, 2, 0, 3, 1, 240);
						system.setReschedule(1, 2, 0, 3, 1, 300);
						system.setReschedule(2, 3, 0, 4, 2, 360);
						system.setReschedule(2, 3, 0, 4, 2, 420);
						system.setReschedule(2, 3, 0, 4, 2, 480);

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
				 		system.clearReschedule();
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