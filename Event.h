#ifndef Event_h
#define Event_h

// type: 0(arrive), 1(transfer), 2(leave)
// from: 0-8 from which tech

struct Event {
	double occur_time;
	int type;
	int from;
	Event(
		double occur_time = 0, 
		int type = 0,
		int from = 0): 
	occur_time(occur_time), 
	type(type),
	from(from) {}

	friend bool operator<(const Event &, const Event &);
};

#endif