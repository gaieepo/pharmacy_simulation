#include "System.h"
#include <iostream>
#include <cstdlib>
#include <vector>

void optimization(int tech_num, int simulate_num, double total_service_minutes) {
	std::vector<std::vector<int>> combinations;
	double min = 10000;
	System system(total_service_minutes, tech_num);
	for (int i = 1; i != 3; ++i) {
		for (int j = 1; j != 3; ++j) {
			for (int k = 1; k != tech_num - 3; ++k) {
				for (int l = 1; l != 5; ++l) {
					for (int m = 1; m != 3; ++m) {
						if (i + j + k + l + m == tech_num) {
							system.setTechAllocation(i, j, k, l, tech_num - i - j - k - l);
							system.simulate(simulate_num);
							if (system.getAvgStayMinutes() < min) {
								min = system.getAvgStayMinutes();
								combinations.clear();
								std::vector<int> temp{i, j, k, l, tech_num - i - j - k - l};
								combinations.push_back(temp);
							} else if (system.getAvgStayMinutes() == min) {
								std::vector<int> temp{i, j, k, l, tech_num - i - j - k - l};
								combinations.push_back(temp);
							}
							std::cout << "Tried :" << i << " " << j << " " << k << " " << l << " " << (tech_num - i - j - k - l) << " Use: " << system.getAvgStayMinutes() << std::endl;
						}
					}
				}
			}
		}
	}
	std::cout << "Min avg stay minutes: " << min << std::endl;
	std::cout << "--------------------------" << std::endl;
	for (auto v : combinations) {
		for (int i : v) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}
}

int main() {
	double total_service_minutes = 540;
	
	int tech_num = 11;
	int simulate_num = 10;

	optimization(tech_num, simulate_num, total_service_minutes);
}