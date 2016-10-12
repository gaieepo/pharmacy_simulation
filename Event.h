#ifndef Event_h
#define Event_h

#include "Random.h"

// type: 0(arrive), 1(transfer), 2(leave), 3(redo), 4(shift), 5(reschedule), 6(shutdown)
// from: 0-8 from which tech

struct Event {
	double occur_time;
	int type;
	int from;
	int tech_alloc[5];

	Event(
		double occur_time = 0, 
		int type = 0,
		int from = 0,
		int alloc[5] = nullptr): 
	occur_time(occur_time), 
	type(type),
	from(from) {
		if (alloc) {
			for (int i = 0; i < 5; ++i)
				tech_alloc[i] = alloc[i];
		}
	}

	friend bool operator<(const Event &, const Event &);
};

#endif