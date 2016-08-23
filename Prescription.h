#ifndef Prescription_h
#define Prescription_h

#include "Random.h"

struct Prescription {
	int id;

	double arrive_time;

	double reg_start;
	double reg_duration = 1;
	double reg_end;

	double typ_start;
	double typ_duration = 1;
	double typ_end;

	double pac_start;
	double pac_duration = 5;
	double pac_end;

	double che_dispense_start;
	double che_dispense_duration = 6;
	double che_dispense_end;

	double pay_start;
	double pay_duration = 2;
	double pay_end;

	Prescription(double arrive_time = 0): arrive_time(arrive_time) {}
};

#endif