#include "System.h"
#include <iostream>
#include <cstdlib>

int main() {
	double total_service_minutes = 300;
	int reg_num = 1;
	int typ_num = 1;
	int pac_num = 3;
	int che_num = 3;
	int pay_num = 1;
	int simulate_num = 10;

	System system(
		total_service_minutes,
		reg_num,
		typ_num,
		pac_num,
		che_num,
		pay_num);
	system.simulate(simulate_num);
}