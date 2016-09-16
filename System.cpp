#include <queue>
#include <cstdio>
#include <memory>
#include "System.h"
#include "Event.h"
#include "Random.h"

bool operator<(const Event &lhs, const Event &rhs) {
	return rhs.occur_time < lhs.occur_time;
}

System::System(int total_service_minutes, int tech_num):
	total_service_minutes(total_service_minutes), tech_num(tech_num) {
		this->techs = new Tech[tech_num];
		for (int i = 0; i != tech_num; ++i)
			techs[i].setIndex(i);
	}

System::~System() {
	delete [] techs;
	delete current_event;
}

void System::simulate(int simulate_num) {
	double sum = 0;
	for (int i = 0; i != simulate_num; ++i) {
		sum += run();
		resetId();
	}

	avg_stay_minutes = sum / simulate_num;

	for (int i = 0; i != endIndex(); ++i) {
		techs[i].calAvgUtilityRate(simulate_num);
		printf("%f ", techs[i].getAvgUtilityRate());
		techs[i].resetUtilityRate();
	}
	printf("\n");
}

double System::getRandomInterval(double time) {
	double ret = 0;
	do {
		ret = Random::getRandom(WEIBULL, 1.71, 121.37) / 60;
	} while (ret < 0.5);
	return ret;
}

PrescType System::getRandomPrescType() {
	double r = Random::getRandom(UNIFORM, 1);
	if (r < 0.5)
		return LONGRX;
	else if (r < 0.9)
		return SHORTRX;
	else
		return ALERT;
}

double System::run() {
	this->init();
	do {
		*current_event = event_queue.top();
		event_queue.pop();
		latest_event_time = current_event->occur_time;
		if (current_event->type == 0) {
			prescArrive();
		} else if (current_event->type == 1) {
			prescTransfer();
		} else if (current_event->type == 2) {
			prescLeave();
		}
	} while (!event_queue.empty());
	this->end();

	return total_stay_minutes / total_prescription_num;
}

void System::init() {
	current_event = new Event;
	event_queue.push(*current_event);
}

void System::end() {
	for (int i = 0; i != endIndex(); ++i) {
		techs[i].setIdle(current_event->occur_time);
		techs[i].calUtilityRate(latest_event_time);
		techs[i].accumulateUtilityRate();
		techs[i].resetBusyMinutes();
	}

	clearQueue(reg_queue);
	clearQueue(pac_queue);
	clearQueue(che_queue);
	clearQueue(pay_queue);

	clearQueue(spe_queue);

	clearQueue(event_queue);
}

int System::getIdleTech() {
	for (int i = 0; i != pacIndexBegin(); ++i) {
		if (techs[i].isIdle()) {
			return i;
		}
	}
	return -1;
}

int System::getIdleTech(int from) {
	if (from >= 0 && from < pacIndexBegin()) {
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
	total_prescription_num++;
	double intertime = getRandomInterval(current_event->occur_time);
	double time = current_event->occur_time + intertime;

	Event next_arrive_event(time);

	if (time < total_service_minutes) {
		event_queue.push(next_arrive_event);
	}

	Prescription* presc = new Prescription(current_event->occur_time, getRandomPrescType());
	presc->id = getId();
	reg_queue.push(*presc);

	int idleIndex = getIdleTech();
	if (idleIndex >= 0) {
		*presc = reg_queue.front();
		reg_queue.pop();
		presc->reg_typ_start = current_event->occur_time;
		presc->reg_typ_end = current_event->occur_time + presc->reg_typ_duration;
		techs[idleIndex].serve(*presc);
		techs[idleIndex].setBusy(current_event->occur_time);

		Event transfer_event(presc->reg_typ_end, 1, idleIndex);
		event_queue.push(transfer_event);
	}

	delete presc;
}

void System::prescTransfer() {
	std::shared_ptr<Prescription> old_presc = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	if (current_event->from >= 0 && current_event->from < pacIndexBegin()) {
		curr = &reg_queue;
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
		if (idleIndex >= pacIndexBegin() && idleIndex < cheIndexBegin()) {
			direct.pac_start = current_event->occur_time;
			direct.pac_end = current_event->occur_time + direct.pac_duration;
			event_time = direct.pac_end;
		} else if (idleIndex >= cheIndexBegin() && idleIndex < payIndexBegin()) {
			direct.che_dis_start = current_event->occur_time;
			direct.che_dis_end = current_event->occur_time + direct.che_dis_duration;
			event_time = direct.che_dis_end;
		} else if (idleIndex >= payIndexBegin() && idleIndex < endIndex()) {
			direct.pay_start = current_event->occur_time;
			direct.pay_end = current_event->occur_time + direct.pay_duration;
			event_time = direct.pay_end;
			isLeaving = true;
		}
		techs[idleIndex].serve(direct);
		techs[idleIndex].setBusy(current_event->occur_time);

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
		if (current_event->from >= 0 && current_event->from < pacIndexBegin()) {
			follow.reg_typ_start = current_event->occur_time;
			follow.reg_typ_end = current_event->occur_time + follow.reg_typ_duration;
			event_time = follow.reg_typ_end;
		} else if (current_event->from >= pacIndexBegin() && current_event->from < cheIndexBegin()) {
			follow.pac_start = current_event->occur_time;
			follow.pac_end = current_event->occur_time + follow.pac_duration;
			event_time = follow.pac_end;
		} else if (current_event->from >= cheIndexBegin() && current_event->from < payIndexBegin()) {
			follow.che_dis_start = current_event->occur_time;
			follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
			event_time = follow.che_dis_end;
		}
		techs[current_event->from].serve(follow);

		Event new_transfer_event(event_time, 1, current_event->from);
		event_queue.push(new_transfer_event);
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
	}
}

void System::prescLeave() {
	std::shared_ptr<Prescription> out = std::make_shared<Prescription>(techs[current_event->from].getPrescription());
	total_stay_minutes += out->pay_end - out->arrive_time;
	
	// printf("No. %d presc type %d: %f(arrive) %d(reg) %d(pac) %d(che) %d(pay) %f(pay_end) Total: %d\n", 
	// 	out->id, 
	// 	(out->pType == LONGRX ? 1 : (out->pType == SHORTRX ? 2 : 3)),
	// 	out->arrive_time, 
	// 	(int) out->reg_typ_duration,
	// 	(int) out->pac_duration,
	// 	(int) out->che_dis_duration,
	// 	(int) out->pay_duration,
	// 	out->pay_end,
	// 	(int) (out->pay_end - out->arrive_time));

	if (pay_queue.size()) {
		Prescription presc = pay_queue.front();
		pay_queue.pop();
		presc.pay_start = current_event->occur_time;
		presc.pay_end = current_event->occur_time + presc.pay_duration;
		techs[current_event->from].serve(presc);

		Event temp_event(presc.pay_end, 2, current_event->from);
		event_queue.push(temp_event);
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
	}
}