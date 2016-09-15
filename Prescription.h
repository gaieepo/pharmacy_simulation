#ifndef Prescription_h
#define Prescription_h

#include "Random.h"

enum PrescType {
	LONGRX,
	SHORTRX,
	ALERT,
};

struct Prescription {
	int id;

	PrescType pType;

	double arrive_time;

	double reg_typ_start;
	double reg_typ_duration = 0;
	double reg_typ_end;

	double pac_start;
	double pac_duration = 0;
	double pac_end;

	double che_dis_start;
	double che_dis_duration = 0;
	double che_dis_end;

	double pay_start;
	double pay_duration = 0;
	double pay_end;

	Prescription(double arrive_time = 0, PrescType type = LONGRX): arrive_time(arrive_time), pType(type) {
		if (pType == LONGRX) {
			do {
				reg_typ_duration = Random::getRandom(LOGNORMAL, 4.75, 0.45) / 60;
			} while (reg_typ_duration < 1.5);

			do {
				pac_duration = Random::getRandom(WEIBULL, 1.30, 318.35) / 60;
			} while (pac_duration < 4);

			do {
				double che_duration = Random::getRandom(LOGNORMAL, 4.91, 0.63) / 60;
				double dis_duration = Random::getRandom(WEIBULL, 1.60, 207.15) / 60;
				che_dis_duration = che_duration + dis_duration;
			} while (che_dis_duration < 4);

			do {
				pay_duration = Random::getRandom(LOGNORMAL, 4.29, 0.39) / 60;
			} while (pay_duration < 0.5);
		} else if (pType == SHORTRX) {
			do {
				reg_typ_duration = Random::getRandom(LOGNORMAL, 4.75, 0.45) / 60;
			} while (reg_typ_duration < 1);

			do {
				pac_duration = Random::getRandom(WEIBULL, 1.81, 108.29) / 60;
			} while (pac_duration < 1);

			do {
				double che_duration = Random::getRandom(LOGNORMAL, 4.78, 0.70) / 60;
				double dis_duration = Random::getRandom(WEIBULL, 1.56, 116.38) / 60;
				che_dis_duration = che_duration + dis_duration;
			} while (che_dis_duration < 2);

			do {
				pay_duration = Random::getRandom(LOGNORMAL, 4.29, 0.39) / 60;
			} while (pay_duration < 0.5);
		} else {
			do {
				reg_typ_duration = Random::getRandom(LOGNORMAL, 4.75, 0.45) / 60;
			} while (reg_typ_duration < 1);

			do {
				pac_duration = Random::getRandom(WEIBULL, 1.81, 108.29) / 60;
			} while (pac_duration < 1);

			do {
				double che_duration = Random::getRandom(NORMAL, 188.33, 143.99) / 60;
				double dis_duration = Random::getRandom(GAMMA, 1.72, 61.33) / 60;
				che_dis_duration = che_duration + dis_duration;
			} while (che_dis_duration < 3);

			do {
				pay_duration = Random::getRandom(LOGNORMAL, 4.29, 0.39) / 60;
			} while (pay_duration < 0.5);
		}
	}
};

#endif