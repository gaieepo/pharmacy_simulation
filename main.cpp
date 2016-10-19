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
	int simulate_num = 3;

	// 	{7, 4},	{7, 4},	{6, 3},	{7, 4},	{4, 3},	{4, 3},	{7, 4},	{7, 4},	{7, 4}
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

	// System system(total_service_minutes, 11);
	// system.setTechAllocation(2, 3, 0, 4, 2);
	// system.setReschedule(1, 2, 0, 2, 1, 60);
	// system.setReschedule(2, 3, 0, 4, 2, 120);
	// system.simulate(1);
	// system.clearReschedule();
	// std::cout << "--------------------------" << std::endl;
	// std::cout << "Min avg stay minutes: " << system.getAvgStayMinutes() << std::endl;

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

	// std::vector<std::vector<int>> hour1;
	// std::vector<std::vector<int>> hour2;
	// std::vector<std::vector<int>> hour3;
	// std::vector<std::vector<int>> hour4;
	// std::vector<std::vector<int>> hour5;
	// std::vector<std::vector<int>> hour6;
	// std::vector<std::vector<int>> hour7;
	// std::vector<std::vector<int>> hour8;
	// std::vector<std::vector<int>> hour9;

	// for (int i = 0; i < 9; ++i) {
	// 	for (int reg  = 1; reg != 3; ++reg) {
	// 		for (int pay = 1; pay != 3; ++pay) {
	// 			for (int flxche = 1; flxche != break_schedule[i][1] + 1; ++flxche) {
	// 				for (int flx = 0; flx <= flxche && flx < 3; ++flx) {
	// 					int che = flxche - flx;
	// 					int pac = break_schedule[i][0] - reg - pay - flx - che;
	// 					if (pac > 0) {
	// 						std::vector<int> temp{reg, pac, flx, che, pay};
	// 						switch (i) {
	// 							case 0:
	// 								hour1.push_back(temp);
	// 								break;
	// 							case 1:
	// 								hour2.push_back(temp);
	// 								break;
	// 							case 2:
	// 								hour3.push_back(temp);
	// 								break;
	// 							case 3:
	// 								hour4.push_back(temp);
	// 								break;
	// 							case 4:
	// 								hour5.push_back(temp);
	// 								break;
	// 							case 5:
	// 								hour6.push_back(temp);
	// 								break;
	// 							case 6:
	// 								hour7.push_back(temp);
	// 								break;
	// 							case 7:
	// 								hour8.push_back(temp);
	// 								break;
	// 							case 8:
	// 								hour9.push_back(temp);
	// 								break;
	// 							default:
	// 								break;
	// 						}
	// 					}
	// 				}
	// 			}
	// 		}
	// 	}
	// }

	System system(total_service_minutes, 11);
	system.setTechAllocation(2, 4, 0, 4, 1);
	system.setReschedule(2, 4, 0, 4, 1, 60); // 7, 4
	system.setReschedule(2, 3, 0, 3, 1, 120); // 6, 3
	system.setReschedule(2, 4, 0, 4, 1, 180); // 7, 4
	system.setReschedule(1, 2, 0, 3, 1, 240); // 4, 3
	system.setReschedule(1, 2, 0, 3, 1, 300); // 4, 3
	system.setReschedule(2, 4, 0, 3, 1, 360); // 7, 3
	system.setReschedule(2, 4, 0, 4, 1, 420); // 7, 4
	system.setReschedule(2, 4, 0, 4, 1, 480); // 7, 4
	system.simulate(simulate_num);
	printf("%f\n", system.getAvgStayMinutes());
	system.clearReschedule();



	// 44, 32, 29
	// std::vector<std::vector<int>> combinations;
	// double min = 10000;
	// System system(total_service_minutes, 11);
	// for (auto a = hour1.begin(); a != hour1.end(); ++a) 
	// 	for (auto b = hour2.begin(); b != hour2.end(); ++b)
	// 		for (auto c = hour3.begin(); c != hour3.end(); ++c)
	// 			for (auto d = hour4.begin(); d != hour4.end(); ++d)
	// 				for (auto e = hour5.begin(); e != hour5.end(); ++e)
	// 					for (auto f = hour6.begin(); f != hour6.end(); ++f)
	// 						for (auto g = hour7.begin(); g != hour7.end(); ++g)
	// 							for (auto h = hour8.begin(); h != hour8.end(); ++h)
	// 								for (auto i = hour9.begin(); i != hour9.end(); ++i) {
	// 									system.setTechAllocation((*a)[0], (*a)[1], (*a)[2], (*a)[3], (*a)[4]);
	// 									system.setReschedule((*b)[0], (*b)[1], (*b)[2], (*b)[3], (*b)[4], 60);
	// 									system.setReschedule((*c)[0], (*c)[1], (*c)[2], (*c)[3], (*c)[4], 120);
	// 									system.setReschedule((*d)[0], (*d)[1], (*d)[2], (*d)[3], (*d)[4], 180);
	// 									system.setReschedule((*e)[0], (*e)[1], (*e)[2], (*e)[3], (*e)[4], 240);
	// 									system.setReschedule((*f)[0], (*f)[1], (*f)[2], (*f)[3], (*f)[4], 300);
	// 									system.setReschedule((*g)[0], (*g)[1], (*g)[2], (*g)[3], (*g)[4], 360);
	// 									system.setReschedule((*h)[0], (*h)[1], (*h)[2], (*h)[3], (*h)[4], 420);
	// 									system.setReschedule((*i)[0], (*i)[1], (*i)[2], (*i)[3], (*i)[4], 480);
	// 									system.simulate(simulate_num);

	// 									if (system.getAvgStayMinutes() < min) {
	// 										min = system.getAvgStayMinutes();
	// 										combinations.clear();
	// 										combinations.push_back(*a);
	// 										combinations.push_back(*b);
	// 										combinations.push_back(*c);
	// 										combinations.push_back(*d);
	// 										combinations.push_back(*e);
	// 										combinations.push_back(*f);
	// 										combinations.push_back(*g);
	// 										combinations.push_back(*h);
	// 										combinations.push_back(*i);
	// 									}
	// 							 		system.clearReschedule();

	// 							 		printf("---------\n");
	// 									printf("Tried:\n");
	// 									printf("1 - %d %d %d %d %d\n", (*a)[0], (*a)[1], (*a)[2], (*a)[3], (*a)[4]);
	// 									printf("2 - %d %d %d %d %d\n", (*b)[0], (*b)[1], (*b)[2], (*b)[3], (*b)[4]);
	// 									printf("3 - %d %d %d %d %d\n", (*c)[0], (*c)[1], (*c)[2], (*c)[3], (*c)[4]);
	// 									printf("4 - %d %d %d %d %d\n", (*d)[0], (*d)[1], (*d)[2], (*d)[3], (*d)[4]);
	// 									printf("5 - %d %d %d %d %d\n", (*e)[0], (*e)[1], (*e)[2], (*e)[3], (*e)[4]);
	// 									printf("6 - %d %d %d %d %d\n", (*f)[0], (*f)[1], (*f)[2], (*f)[3], (*f)[4]);
	// 									printf("7 - %d %d %d %d %d\n", (*g)[0], (*g)[1], (*g)[2], (*g)[3], (*g)[4]);
	// 									printf("8 - %d %d %d %d %d\n", (*h)[0], (*h)[1], (*h)[2], (*h)[3], (*h)[4]);
	// 									printf("9 - %d %d %d %d %d\n", (*i)[0], (*i)[1], (*i)[2], (*i)[3], (*i)[4]);
	// 									printf("Avg - %f\n", system.getAvgStayMinutes());
	// 									printf("Best:\n");
	// 									printf("1 - %d %d %d %d %d\n", combinations[0][0], combinations[0][1], combinations[0][2], combinations[0][3], combinations[0][4]);
	// 									printf("2 - %d %d %d %d %d\n", combinations[1][0], combinations[1][1], combinations[1][2], combinations[1][3], combinations[1][4]);
	// 									printf("3 - %d %d %d %d %d\n", combinations[2][0], combinations[2][1], combinations[2][2], combinations[2][3], combinations[2][4]);
	// 									printf("4 - %d %d %d %d %d\n", combinations[3][0], combinations[3][1], combinations[3][2], combinations[3][3], combinations[3][4]);
	// 									printf("5 - %d %d %d %d %d\n", combinations[4][0], combinations[4][1], combinations[4][2], combinations[4][3], combinations[4][4]);
	// 									printf("6 - %d %d %d %d %d\n", combinations[5][0], combinations[5][1], combinations[5][2], combinations[5][3], combinations[5][4]);
	// 									printf("7 - %d %d %d %d %d\n", combinations[6][0], combinations[6][1], combinations[6][2], combinations[6][3], combinations[6][4]);
	// 									printf("8 - %d %d %d %d %d\n", combinations[7][0], combinations[7][1], combinations[7][2], combinations[7][3], combinations[7][4]);
	// 									printf("9 - %d %d %d %d %d\n", combinations[8][0], combinations[8][1], combinations[8][2], combinations[8][3], combinations[8][4]);
	// 									printf("Min - %f\n", min);
	// 									printf("---------\n");
	// 								}


// std::cout << "Tried :" << reg << " " << pac << " " << flx << " " << che << " " << pay << " Use: " << system.getAvgStayMinutes() << std::endl;
	// std::cout << "Min avg stay minutes: " << min << std::endl;
	// std::cout << "--------------------------" << std::endl;
	// for (auto v : combinations) {
	// 	for (int i: v) {
	// 		std::cout << i << " ";
	// 	}
	// 	std::cout << std::endl;
	// }
}