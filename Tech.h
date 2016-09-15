#ifndef Tech_h
#define Tech_h

#include "Prescription.h"
#include <memory>
#include <cstdio>

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

	inline double getUtilityRate(double latest_event_time) {
		return total_busy_minutes / latest_event_time;
	}

	inline double getBusyMinutes() {
		return total_busy_minutes;
	}
private:
	Prescription prescription;
	Status status;
	int index;

	double busy_time = 0;
	double total_busy_minutes = 0;
};

#endif