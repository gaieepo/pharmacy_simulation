#ifndef Tech_h
#define Tech_h

#include "Prescription.h"
#include <memory>

enum Status {
	SERVICE,
	IDLE,
};

class Tech {
public:
	inline Tech() {
		status = IDLE;
	}

	inline bool isIdle() const {
		return status == IDLE;
	}

	inline void serve(Prescription &prescription) {
		this->prescription = prescription;
	}

	inline void setBusy(double time) {
		if (status == IDLE) {
			busy_time = time;
		}
		status = SERVICE;
	}

	inline void setIdle(double time) {
		if (status == SERVICE) {
			total_busy_minutes += time - busy_time;
			busy_time = 0;
		}
		status = IDLE;
	}

	inline void setIndex(int idx) {
		index = idx;
	}

	inline int getIndex() {
		return index;
	}

	inline Prescription getPrescription() {
		return prescription;
	}

	inline void calUtilityRate(double latest_event_time) {
		utility_rate = total_busy_minutes / latest_event_time;
	}

	inline void accumulateUtilityRate() {
		total_utility_rate += utility_rate;
	}

	inline double getBusyMinutes() {
		return total_busy_minutes;
	}

	inline void resetBusyMinutes() {
		total_busy_minutes = 0;
	}

	inline void resetUtilityRate() {
		utility_rate = 0;
		total_utility_rate = 0;
		avg_utility_rate = 0;
	}

	inline void calAvgUtilityRate(int simulate_num) {
		avg_utility_rate = total_utility_rate / simulate_num;
	}

	inline double getAvgUtilityRate() {
		return avg_utility_rate;
	}
private:
	Prescription prescription;
	Status status;
	int index;

	double busy_time = 0;
	double total_busy_minutes = 0;

	double utility_rate = 0;
	double total_utility_rate = 0;
	double avg_utility_rate = 0;
};

#endif