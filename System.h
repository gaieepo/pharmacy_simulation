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

	inline void setTechAllocation(int reg, int typ, int pac, int che, int pay) {
		this->reg_num = reg;
		this->typ_num = typ;
		this->pac_num = pac;
		this->che_num = che;
		this->pay_num = pay;
	}

	inline int typIndexBegin() {
		return reg_num;
	}

	inline int pacIndexBegin() {
		return reg_num + typ_num;
	}

	inline int cheIndexBegin() {
		return reg_num + typ_num + pac_num;
	}

	inline int payIndexBegin() {
		return reg_num + typ_num + pac_num + che_num;
	}

	inline int endIndex() {
		return reg_num + typ_num + pac_num + che_num + pay_num;
	}

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

	double run();
	void init();
	void end();

	int getIdleTech();
	int getIdleTech(int from);

	void prescArrive();
	void prescTransfer();
	void prescLeave();

	int total_service_minutes;
	int tech_num;

	int reg_num = 1;
	int typ_num = 1;
	int pac_num = 1;
	int che_num = 1;
	int pay_num = 1;

	Tech* techs = nullptr;

	std::queue<Prescription> reg_queue;
	std::queue<Prescription> typ_queue;
	std::queue<Prescription> pac_queue;
	std::queue<Prescription> che_queue;
	std::queue<Prescription> pay_queue;

	std::priority_queue<Event> event_queue;

	Event* current_event = nullptr;
	std::queue<Prescription> *curr = nullptr;
	std::queue<Prescription> *next = nullptr;

	int total_prescription_num = 0;
	double total_stay_minutes = 0;
	double avg_stay_minutes = 0;

	int idCount = 0;
};

#endif