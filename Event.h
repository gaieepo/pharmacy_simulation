#ifndef Event_h
#define Event_h

#include "Random.h"

// type: 0(arrive), 1(transfer), 2(leave), 3(redo), 4(shift), 5(reschedule), 6(shutdown)
// from: 0-8 from which tech

struct Event {
	double occur_time;
	int type;
	int from;
	int schedule_index;

	Event(
		double occur_time = 0, 
		int type = 0,
		int from = 0,
		int schedule_index = 0): 
	occur_time(occur_time), 
	type(type),
	from(from),
	schedule_index(schedule_index) {}

	friend bool operator<(const Event &, const Event &);
};

#endif