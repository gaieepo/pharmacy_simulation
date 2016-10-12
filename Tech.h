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
			current_busy_minutes = time - busy_time;
			total_busy_minutes += current_busy_minutes;
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

	inline double getBreakBegin() {
		return break_begin;
	}

	inline void setBreakBegin(double time) {
		break_begin = time;
	}

	inline double getBreakEnd() {
		return break_end;
	}

	inline void setBreakEnd(double time) {
		break_end = time;
	}

	inline bool isBreak(double time) {
		return time >= break_begin && time <= break_end;
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

	inline double getCurrentBusyMinutes() {
		return current_busy_minutes;
	}

	inline double getBusyMinutes() {
		return total_busy_minutes;
	}

	inline void resetCurrentBusyMinutes() {
		current_busy_minutes = 0;
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

	inline void doShift() {
		shift = true;
	}

	inline void undoShift() {
		shift = false;
	}

	inline bool isShift() {
		return shift;
	}

	inline void setNextFinishTime(double time) {
		next_finish_time = time;
	}

	inline double getNextFinishTime() {
		return next_finish_time;
	}
private:
	Prescription prescription;
	
	Status status;

	int index;
	bool shift = false;

	double next_finish_time = 0;

	double break_begin = -1;
	double break_end = -1;

	double busy_time = 0;
	double current_busy_minutes = 0;
	double total_busy_minutes = 0;

	double utility_rate = 0;
	double total_utility_rate = 0;
	double avg_utility_rate = 0;
};

#endif