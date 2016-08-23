#include <queue>
#include <cstdio>
#include "System.h"
#include "Event.h"

bool operator<(const Event &lhs, const Event &rhs) {
	return rhs.occur_time < lhs.occur_time;
}

System::System(
	int total_service_minutes,
	int reg_num,
	int typ_num,
	int pac_num,
	int che_num,
	int pay_num):
	total_service_minutes(total_service_minutes),
	reg_num(reg_num),
	typ_num(typ_num),
	pac_num(pac_num),
	che_num(che_num),
	pay_num(pay_num) {
		int total_tech_num = endIndex();
		this->techs = new Tech[total_tech_num];
	}

System::~System() {
	delete [] techs;
}

void System::simulate(int simulate_num) {
	double sum = 0;
	for (int i = 0; i != simulate_num; ++i) {
		sum += run();
		printf("%f\n", sum);
	}
	result = sum;
}

double System::run() {
	this->init();
	while (!event_queue.empty()) {
		if (current_event->type == 0) {
			printf("arrive\n");
			prescArrive();
		} else if (current_event->type == 1) {
			printf("transfer\n");
			prescTransfer();
		} else if (current_event->type == 2) {
			printf("leave\n");
			prescLeave();
		}
		*current_event = event_queue.top();
		event_queue.pop();
	}
	this->end();
	return 1.0;
}

void System::init() {
	current_event = new Event;
	event_queue.push(*current_event);
}

void System::end() {
	for (int i = 0; i != endIndex(); ++i) {
		techs[i].setIdle();
	}

	clearQueue(reg_queue);
	clearQueue(typ_queue);
	clearQueue(pac_queue);
	clearQueue(che_queue);
	clearQueue(pay_queue);

	clearQueue(event_queue);
}

int System::getIdleTech() {
	for (int i = 0; i != typIndexBegin(); ++i) {
		if (techs[i].isIdle()) {
			return i;
		}
	}
	return -1;
}

int System::getIdleTech(int from) {
	if (from >= 0 && from < typIndexBegin()) {
		for (int i = typIndexBegin(); i != pacIndexBegin(); ++i) {
			if (techs[i].isIdle()) {
				return i;
			}
		}
	} else if (from >= typIndexBegin() && from < pacIndexBegin()) {
		for (int i = pacIndexBegin(); i != cheIndexBegin(); ++i) {
			if (techs[i].isIdle()) {
				return i;
			}
		}
	} else if (from >= pacIndexBegin() && from < cheIndexBegin()) {
		for (int i = cheIndexBegin(); i != payIndexBegin(); ++i) {
			if (techs[i].isIdle()) {
				return i;
			}
		}
	} else if (from >= cheIndexBegin() && from < payIndexBegin()) {
		for (int i = payIndexBegin(); i != endIndex(); ++i) {
			if (techs[i].isIdle()) {
				return i;
			}
		}
	}
	return -1;
}

void System::prescArrive() {
	double intertime = 10.0;
	int time = current_event->occur_time + intertime;

	Event next_arrive_event(time);

	if (time < total_service_minutes) {
		event_queue.push(next_arrive_event);
	}

	Prescription* presc = new Prescription(current_event->occur_time);
	presc->id = getId();
	reg_queue.push(*presc);

	int idleIndex = getIdleTech();
	if (idleIndex >= 0) {
		*presc = reg_queue.front();
		reg_queue.pop();
		presc->reg_start = current_event->occur_time;
		presc->reg_end = current_event->occur_time + presc->reg_duration;
		techs[idleIndex].serve(*presc);
		techs[idleIndex].setBusy();

		Event transfer_event(presc->reg_end, 1, idleIndex);
		event_queue.push(transfer_event);
	}

	delete presc;
}

void System::prescTransfer() {
	Prescription* old_presc;
	old_presc = techs[current_event->from].getPrescription();
	std::queue<Prescription> &curr = reg_queue;
	std::queue<Prescription> &next = typ_queue;
	if (current_event->from >= typIndexBegin() && current_event->from < pacIndexBegin()) {
		curr = typ_queue;
		next = pac_queue;
	} else if (current_event->from >= pacIndexBegin() && current_event->from < cheIndexBegin()) {
		curr = pac_queue;
		next = che_queue;
	} else if (current_event->from >= cheIndexBegin() && current_event->from < payIndexBegin()) {
		curr = che_queue;
		next = pay_queue;
	}
	next.push(*old_presc);

	int idleIndex = getIdleTech(current_event->from);
	if (idleIndex >= 0) {
		*old_presc = next.front();
		next.pop();
		double event_time;
		bool isLeaving = false;
		if (current_event->from >= typIndexBegin() && current_event->from < pacIndexBegin()) {
			old_presc->typ_start = current_event->occur_time;
			old_presc->typ_end = current_event->occur_time + old_presc->typ_duration;
			event_time = old_presc->typ_end;
		} else if (current_event->from >= pacIndexBegin() && current_event->from < cheIndexBegin()) {
			old_presc->pac_start = current_event->occur_time;
			old_presc->pac_end = current_event->occur_time + old_presc->pac_duration;
			event_time = old_presc->pac_end;
		} else if (current_event->from >= cheIndexBegin() && current_event->from < payIndexBegin()) {
			old_presc->che_dispense_start = current_event->occur_time;
			old_presc->che_dispense_end = current_event->occur_time + old_presc->che_dispense_duration;
			event_time = old_presc->che_dispense_end;
		} else if (current_event->from >= payIndexBegin() && current_event->from < endIndex()) {
			old_presc->pay_start = current_event->occur_time;
			old_presc->pay_end = current_event->occur_time + old_presc->pay_duration;
			event_time = old_presc->pay_end;
			isLeaving = true;
		}
		techs[idleIndex].serve(*old_presc);
		techs[idleIndex].setBusy();

		if (isLeaving) {
			Event leave_event(event_time, 2, idleIndex);
			event_queue.push(leave_event);
		} else {
			Event transfer_event(event_time, 1, idleIndex);
			event_queue.push(transfer_event);
		}
	}

	if (curr.size()) {
		*old_presc = curr.front();
		curr.pop();
		double event_time;
		if (current_event->from >= 0 && current_event->from < typIndexBegin()) {
			old_presc->reg_start = current_event->occur_time;
			old_presc->reg_end = current_event->occur_time + old_presc->reg_duration;
			event_time = old_presc->reg_end;
		} else if (current_event->from >= typIndexBegin() && current_event->from < pacIndexBegin()) {
			old_presc->typ_start = current_event->occur_time;
			old_presc->typ_end = current_event->occur_time + old_presc->typ_duration;
			event_time = old_presc->typ_end;
		} else if (current_event->from >= pacIndexBegin() && current_event->from < cheIndexBegin()) {
			old_presc->pac_start = current_event->occur_time;
			old_presc->pac_end = current_event->occur_time + old_presc->pac_duration;
			event_time = old_presc->pac_end;
		} else if (current_event->from >= cheIndexBegin() && current_event->from < payIndexBegin()) {
			old_presc->che_dispense_start = current_event->occur_time;
			old_presc->che_dispense_end = current_event->occur_time + old_presc->che_dispense_duration;
			event_time = old_presc->che_dispense_end;
		}
		techs[current_event->from].serve(*old_presc);

		Event new_transfer_event(event_time, 1, current_event->from);
		event_queue.push(new_transfer_event);
	} else {
		techs[current_event->from].setIdle();
	}
}

void System::prescLeave() {
	Prescription* out = techs[current_event->from].getPrescription();
	printf("No. %d presc: %f -> %f\n", out->id, out->arrive_time, out->pay_end);
	delete out;

	if (pay_queue.size()) {
		Prescription presc = pay_queue.front();
		pay_queue.pop();
		presc.pay_start = current_event->occur_time;
		presc.pay_end = current_event->occur_time + presc.pay_duration;
		techs[current_event->from].serve(presc);

		Event temp_event(presc.pay_end, 2, current_event->from);
		event_queue.push(temp_event);
	} else {
		techs[current_event->from].setIdle();
	}
}