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
	bool redoed = false;
	double efficiency = 1;

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

	inline double getRegWait() {
		return reg_typ_start - arrive_time;
	}

	inline double getPacWait() {
		return pac_start - reg_typ_end;
	}

	inline double getCheWait() {
		return che_dis_start - pac_end;
	}

	inline double getPayWait() {
		return pay_start - che_dis_end;
	}

	Prescription(double arrive_time = 0, PrescType type = LONGRX): arrive_time(arrive_time), pType(type) {
		// double medisave = (Random::getRandom(UNIFORM, 100) < 2) ? 10 : 0;
		double medisave = 0;
		// double clarify = (Random::getRandom(UNIFORM, 100) < 2) ? Random::getRandom(UNIFORM, 10, 15) : 0;
		double clarify = 0;
		double check_variance = 0.0;
		double check_mean = 0;
		
		if ((arrive_time >= 60 && arrive_time <= 180) || (arrive_time >= 360 && arrive_time <= 480)) {
			efficiency = 0.70 ;
		} else {
			efficiency = 1.2 ;
		}

		if (pType == LONGRX) {
			do {
				reg_typ_duration = Random::getRandom(LOGNORMAL, check_mean + 4.5, 1.3 + check_variance) / 60;
			} while (reg_typ_duration <0.2|| reg_typ_duration > 8);
			reg_typ_duration += medisave;

			do {
				pac_duration = Random::getRandom(LOGNORMAL, check_mean + 5.5, 1.4 + check_variance) / 60;
			} while (pac_duration <0.2|| pac_duration > 8);

			do {
				double che_duration = Random::getRandom(LOGNORMAL, check_mean + 4.8, 1.6 + check_variance) / 60;
				double dis_duration = Random::getRandom(LOGNORMAL, check_mean + 5.0, 1.6 + check_variance) / 60;
				che_dis_duration = che_duration + dis_duration;
			} while (che_dis_duration <0.2|| che_dis_duration > 8);

			do {
				pay_duration = Random::getRandom(LOGNORMAL, check_mean + 4.1, 1.2 + check_variance) / 60;
			} while (pay_duration <0.2|| pay_duration > 8);
		} else if (pType == SHORTRX) {
			do {
				reg_typ_duration = Random::getRandom(LOGNORMAL, check_mean + 4.5, 1.3 + check_variance) / 60;
			} while (reg_typ_duration <0.2|| reg_typ_duration > 8);
			reg_typ_duration += medisave;

			do {
				pac_duration = Random::getRandom(LOGNORMAL, check_mean + 4.8, 1.5 + check_variance) / 60;
			} while (pac_duration <0.2|| pac_duration > 8);

			do {
				double che_duration = Random::getRandom(LOGNORMAL, check_mean + 4.5, 1.6 + check_variance) / 60;
				double dis_duration = Random::getRandom(LOGNORMAL, check_mean + 5.0, 1.6 + check_variance) / 60;
				che_dis_duration = che_duration + dis_duration;
			} while (che_dis_duration <0.2|| che_dis_duration > 8);

			do {
				pay_duration = Random::getRandom(LOGNORMAL, check_mean + 4.1, 1.6 + check_variance) / 60;
			} while (pay_duration <0.2|| pay_duration > 8);
		} else {
			do {
				reg_typ_duration = Random::getRandom(LOGNORMAL, check_mean + 4.5, 1.3 + check_variance) / 60;
			} while (reg_typ_duration <0.2|| reg_typ_duration > 8);
			reg_typ_duration += medisave;

			do {
				pac_duration = Random::getRandom(LOGNORMAL, check_mean + 4.8, 1.5 + check_variance) / 60;
			} while (pac_duration <0.2|| pac_duration > 8);

			do {
				double che_duration = Random::getRandom(LOGNORMAL, check_mean + 4.8, 1.6 + check_variance) / 60;
				double dis_duration = Random::getRandom(LOGNORMAL, check_mean + 5.0, 1.6 + check_variance) / 60;
				che_dis_duration = che_duration + dis_duration;
			} while (che_dis_duration <0.2|| che_dis_duration > 10);

			do {
				pay_duration = Random::getRandom(LOGNORMAL, check_mean + 4.1, 1.6 + check_variance) / 60;
			} while (pay_duration <0.2|| pay_duration > 10);
		}

		reg_typ_duration = reg_typ_duration * efficiency;
		pac_duration = pac_duration * efficiency;
		che_dis_duration = che_dis_duration * efficiency;
		pay_duration = pay_duration * efficiency;
	}
};

#endif