#include <queue>
#include <cstdio>
#include <memory>
#include "System.h"
#include "Event.h"
#include "Random.h"

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
	delete current_event;
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
	do {
		*current_event = event_queue.top();
		event_queue.pop();
		if (current_event->type == 0) {
			prescArrive();
		} else if (current_event->type == 1) {
			prescTransfer();
		} else if (current_event->type == 2) {
			prescLeave();
		}
	} while (!event_queue.empty());
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
	double intertime = 2.0;
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
	std::shared_ptr<Prescription> old_presc = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	if (current_event->from >= 0 && current_event->from < typIndexBegin()) {
		curr = &reg_queue;
		next = &typ_queue;
	} else if (current_event->from >= typIndexBegin() && current_event->from < pacIndexBegin()) {
		curr = &typ_queue;
		next = &pac_queue;
	} else if (current_event->from >= pacIndexBegin() && current_event->from < cheIndexBegin()) {
		curr = &pac_queue;
		next = &che_queue;
	} else if (current_event->from >= cheIndexBegin() && current_event->from < payIndexBegin()) {
		curr = &che_queue;
		next = &pay_queue;
	}
	next->push(*old_presc);

	int idleIndex = getIdleTech(current_event->from);
	if (idleIndex >= 0) {
		Prescription direct(next->front());
		next->pop();
		double event_time = 0;
		bool isLeaving = false;
		if (idleIndex >= typIndexBegin() && idleIndex < pacIndexBegin()) {
			direct.typ_start = current_event->occur_time;
			direct.typ_end = current_event->occur_time + direct.typ_duration;
			event_time = direct.typ_end;
		} else if (idleIndex >= pacIndexBegin() && idleIndex < cheIndexBegin()) {
			direct.pac_start = current_event->occur_time;
			direct.pac_end = current_event->occur_time + direct.pac_duration;
			event_time = direct.pac_end;
		} else if (idleIndex >= cheIndexBegin() && idleIndex < payIndexBegin()) {
			direct.che_dispense_start = current_event->occur_time;
			direct.che_dispense_end = current_event->occur_time + direct.che_dispense_duration;
			event_time = direct.che_dispense_end;
		} else if (idleIndex >= payIndexBegin() && idleIndex < endIndex()) {
			direct.pay_start = current_event->occur_time;
			direct.pay_end = current_event->occur_time + direct.pay_duration;
			event_time = direct.pay_end;
			isLeaving = true;
		}
		techs[idleIndex].serve(direct);
		techs[idleIndex].setBusy();

		if (isLeaving) {
			Event leave_event(event_time, 2, idleIndex);
			event_queue.push(leave_event);
		} else {
			Event transfer_event(event_time, 1, idleIndex);
			event_queue.push(transfer_event);
		}
	}

	if (curr->size()) {
		Prescription follow(curr->front());
		curr->pop();
		double event_time;
		if (current_event->from >= 0 && current_event->from < typIndexBegin()) {
			follow.reg_start = current_event->occur_time;
			follow.reg_end = current_event->occur_time + follow.reg_duration;
			event_time = follow.reg_end;
		} else if (current_event->from >= typIndexBegin() && current_event->from < pacIndexBegin()) {
			follow.typ_start = current_event->occur_time;
			follow.typ_end = current_event->occur_time + follow.typ_duration;
			event_time = follow.typ_end;
		} else if (current_event->from >= pacIndexBegin() && current_event->from < cheIndexBegin()) {
			follow.pac_start = current_event->occur_time;
			follow.pac_end = current_event->occur_time + follow.pac_duration;
			event_time = follow.pac_end;
		} else if (current_event->from >= cheIndexBegin() && current_event->from < payIndexBegin()) {
			follow.che_dispense_start = current_event->occur_time;
			follow.che_dispense_end = current_event->occur_time + follow.che_dispense_duration;
			event_time = follow.che_dispense_end;
		}
		techs[current_event->from].serve(follow);

		Event new_transfer_event(event_time, 1, current_event->from);
		event_queue.push(new_transfer_event);
	} else {
		techs[current_event->from].setIdle();
	}
}

void System::prescLeave() {
	std::shared_ptr<Prescription> out = std::make_shared<Prescription>(techs[current_event->from].getPrescription());
	printf("No. %d presc: %f -> %f. Total: %f\n", out->id, out->arrive_time, out->pay_end, out->pay_end - out->arrive_time);

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