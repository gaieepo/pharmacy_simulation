#ifndef Prescription_h
#define Prescription_h

#include "Random.h"

struct Prescription {
	int id;

	double arrive_time;

	double reg_start;
	double reg_duration = Random::getRandom(LOGNORMAL, 3.9317, 0.84839) / 60;
	double reg_end;

	double typ_start;
	double typ_duration = Random::getRandom(UNIFORM, 2);
	double typ_end;

	double pac_start;
	double pac_duration = 5;
	double pac_end;

	double che_dispense_start;
	double che_dispense_duration = (Random::getRandom(WEIBULL, 1.4423, 199.37) + Random::getRandom(LOGNORMAL, 4.7477, 0.80556)) / 60;
	double che_dispense_end;

	double pay_start;
	double pay_duration = Random::getRandom(EXPONENTIAL, 0.01415) / 60;
	double pay_end;

	Prescription(double arrive_time = 0): arrive_time(arrive_time) {}
};

#endif