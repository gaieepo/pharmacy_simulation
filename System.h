#ifndef System_h
#define System_h

#include "Prescription.h"
#include "Event.h"
#include "Tech.h"
#include <queue>

class System {
public:
	System(int total_service_minutes, int tech_num);
	~System();
	void simulate(int simulate_num);

	void setTechAllocation(int reg, int pac, int flx, int che, int pay);

	inline int pacIndexBegin() {
		return reg_num;
	}

	inline int cheIndexBegin() {
		return reg_num + pac_num;
	}

	inline int flxIndexEnd() {
		return reg_num + pac_num + flx_num;
	}

	inline int payIndexBegin() {
		return reg_num + pac_num + flx_num + che_num;
	}

	inline int endIndex() {
		return reg_num + pac_num + flx_num + che_num + pay_num;
	}

	void setSchedule(double schedule[][2]);

	void reschedule(int reg, int pac, int flx, int che, int pay, double time);

	void clearQueue(std::queue<Prescription> &q) {
		std::queue<Prescription> empty;
		std::swap(q, empty);
	}

	void clearQueue(std::priority_queue<Event> &q) {
		std::priority_queue<Event> empty;
		std::swap(q, empty);
	}

	inline int getId() {
		return idCount++;
	}

	inline void resetId() {
		idCount = 0;
	}

	inline double getAvgStayMinutes() {
		return avg_stay_minutes;
	}
private:
	double getRandomInterval(double time);
	PrescType getRandomPrescType();

	void accumulateAllUtilityRate();
	void resetBusyMinutes();
	void resetUtilityRate();
	void calAvgUtilityRate(int simulate_num);
	void resetAvgUtilityRate();

	void accumulateQueueMinutes();
	void calAvgQueueMinutes(int simulate_num);
	void resetQueueMinutes();
	void resetTotalAvgQueueMinutes();
	void resetAvgQueueMinutes();

	double run();
	void init();
	void end();

	int getIdleTech(double time);
	int getIdleTech(int from, double time);

	void determineCurrAndNext(int from);
	void accumulateBusyMinutes(int from, double time, bool shift);

	void prescArrive();
	void prescTransfer();
	void prescLeave();
	void prescRedo();
	void prescShift();
	void changeSchedule();

	int total_service_minutes;
	int tech_num;

	int reg_num = 1;
	int pac_num = 1;
	int flx_num = 1;
	int che_num = 1;
	int pay_num = 1;

	int reg_alloc = 0;
	int pac_alloc = 0;
	int flx_alloc = 0;
	int che_alloc = 0;
	int pay_alloc = 0;

	double reg_busy_minutes = 0;
	double pac_busy_minutes = 0;
	double shift_busy_minutes = 0;
	double che_busy_minutes = 0;
	double pay_busy_minutes = 0;

	double total_reg_utility_rate = 0;
	double total_pac_utility_rate = 0;
	double total_che_utility_rate = 0;
	double total_pay_utility_rate = 0;

	double avg_reg_utility_rate = 0;
	double avg_pac_utility_rate = 0;
	double avg_che_utility_rate = 0;
	double avg_pay_utility_rate = 0;	

	bool rescheduled = false;
	Tech* techs = nullptr;

	std::queue<Prescription> reg_queue;
	std::queue<Prescription> pac_queue;
	std::queue<Prescription> che_queue;
	std::queue<Prescription> pay_queue;

	std::priority_queue<Event> event_queue;

	Event* current_event = nullptr;
	std::queue<Prescription> *curr = nullptr;
	std::queue<Prescription> *next = nullptr;

	int total_prescription_num = 0;
	int boundary_case = 0;

	double total_stay_minutes = 0;
	double avg_stay_minutes = 0;
	double latest_event_time = 0;

	double reg_queue_minutes = 0;
	double pac_queue_minutes = 0;
	double che_queue_minutes = 0;
	double pay_queue_minutes = 0;

	double total_avg_reg_queue_minutes = 0;
	double total_avg_pac_queue_minutes = 0;
	double total_avg_che_queue_minutes = 0;
	double total_avg_pay_queue_minutes = 0;

	double avg_reg_queue_minutes = 0;
	double avg_pac_queue_minutes = 0;
	double avg_che_queue_minutes = 0;
	double avg_pay_queue_minutes = 0;

	int idCount = 0;
};

#endif