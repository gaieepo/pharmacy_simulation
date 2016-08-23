#ifndef System_h
#define System_h

#include "Prescription.h"
#include "Event.h"
#include "Tech.h"
#include <queue>

class System {
public:
	System(
		int total_service_minutes,
		int reg_num,
		int typ_num,
		int pac_num,
		int che_num,
		int pay_num);
	~System();
	void simulate(int simulate_num);

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
private:
	double run();
	void init();
	void end();

	int getIdleTech();
	int getIdleTech(int from);

	void prescArrive();
	void prescTransfer();
	void prescLeave();

	int total_service_minutes;

	int reg_num;
	int typ_num;
	int pac_num;
	int che_num;
	int pay_num;

	Tech* techs;

	std::queue<Prescription> reg_queue;
	std::queue<Prescription> typ_queue;
	std::queue<Prescription> pac_queue;
	std::queue<Prescription> che_queue;
	std::queue<Prescription> pay_queue;

	std::priority_queue<Event> event_queue;

	Event* current_event;

	double result;

	int idCount = 0;
};

#endif