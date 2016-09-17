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

void System::accumulateAllUtilityRate() {
	total_reg_utility_rate += reg_busy_minutes / (latest_event_time * reg_num);
	total_pac_utility_rate += (pac_busy_minutes + shift_busy_minutes) / (latest_event_time * pac_num + shift_busy_minutes);
	total_che_utility_rate += (che_busy_minutes - shift_busy_minutes) / (latest_event_time * (che_num + flx_num) - shift_busy_minutes);
	total_pay_utility_rate += pay_busy_minutes / (latest_event_time * pay_num);
}

void System::resetBusyMinutes() {
	reg_busy_minutes = 0;
	pac_busy_minutes = 0;
	shift_busy_minutes = 0;
	che_busy_minutes = 0;
	pay_busy_minutes = 0;
}

void System::resetUtilityRate() {
	total_reg_utility_rate = 0;
	total_pac_utility_rate = 0;
	total_che_utility_rate = 0;
	total_pay_utility_rate = 0;
}

void System::calAvgUtilityRate(int simulate_num) {
	avg_reg_utility_rate = total_reg_utility_rate / simulate_num;
	avg_pac_utility_rate = total_pac_utility_rate / simulate_num;
	avg_che_utility_rate = total_che_utility_rate / simulate_num;
	avg_pay_utility_rate = total_pay_utility_rate / simulate_num;
}

void System::resetAvgUtilityRate() {
	avg_reg_utility_rate = 0;
	avg_pac_utility_rate = 0;
	avg_che_utility_rate = 0;
	avg_pay_utility_rate = 0;
}

void System::accumulateQueueMinutes() {
	total_avg_reg_queue_minutes += reg_queue_minutes / total_prescription_num;
	total_avg_pac_queue_minutes += pac_queue_minutes / total_prescription_num;
	total_avg_che_queue_minutes += che_queue_minutes / total_prescription_num;
	total_avg_pay_queue_minutes += pay_queue_minutes / total_prescription_num;
}

void System::calAvgQueueMinutes(int simulate_num) {
	avg_reg_queue_minutes = total_avg_reg_queue_minutes / simulate_num;
	avg_pac_queue_minutes = total_avg_pac_queue_minutes / simulate_num;
	avg_che_queue_minutes = total_avg_che_queue_minutes / simulate_num;
	avg_pay_queue_minutes = total_avg_pay_queue_minutes / simulate_num;
}

void System::resetQueueMinutes() {
	reg_queue_minutes = 0;
	pac_queue_minutes = 0;
	che_queue_minutes = 0;
	pay_queue_minutes = 0;
}

void System::resetTotalAvgQueueMinutes() {
	total_avg_reg_queue_minutes = 0;
	total_avg_pac_queue_minutes = 0;
	total_avg_che_queue_minutes = 0;
	total_avg_pay_queue_minutes = 0;
}

void System::resetAvgQueueMinutes() {
	avg_reg_queue_minutes = 0;
	avg_pac_queue_minutes = 0;
	avg_che_queue_minutes = 0;
	avg_pay_queue_minutes = 0;
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

	calAvgUtilityRate(simulate_num);
	printf("Reg:%f Pac:%f Che:%f Pay:%f\n",
		avg_reg_utility_rate,
		avg_pac_utility_rate,
		avg_che_utility_rate,
		avg_pay_utility_rate);
	resetAvgUtilityRate();
	resetUtilityRate();

	calAvgQueueMinutes(simulate_num);
	printf("RegW:%.2f PacW:%.2f CheW:%.2f PayW:%.2f\n", 
		avg_reg_queue_minutes,
		avg_pac_queue_minutes,
		avg_che_queue_minutes,
		avg_pay_queue_minutes);
	resetTotalAvgQueueMinutes();
	resetAvgQueueMinutes();
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
		} else if (current_event->type == 3) {
			prescRedo();
		} else if (current_event->type == 4) {
			prescShift();
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
		techs[i].resetCurrentBusyMinutes();
	}

	accumulateAllUtilityRate();
	resetBusyMinutes();

	accumulateQueueMinutes();
	resetQueueMinutes();

	clearQueue(reg_queue);
	clearQueue(pac_queue);
	clearQueue(che_queue);
	clearQueue(pay_queue);

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
		if (che_queue.size()) {
			for (int i = pacIndexBegin(); i != cheIndexBegin(); ++i) {
				if (techs[i].isIdle()) {
					return i;
				}
			}
		} else {
			for (int i = pacIndexBegin(); i != flxIndexEnd(); ++i) {
				if (techs[i].isIdle()) {
					return i;
				}
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

void System::determineCurrAndNext(int from) {
	if (from >= 0 && from < pacIndexBegin()) {
		curr = &reg_queue;
		next = &pac_queue;
	} else if (from >= pacIndexBegin() && from < cheIndexBegin()) {
		curr = &pac_queue;
		next = &che_queue;
	} else if (from >= cheIndexBegin() && from < payIndexBegin()) {
		curr = &che_queue;
		next = &pay_queue;
	}
}

void System::accumulateBusyMinutes(int from, double minutes, bool shift) {
	if (shift) {
		shift_busy_minutes += minutes;
	} else {
		if (from >= 0 && from < pacIndexBegin()) {
			reg_busy_minutes += minutes;
		} else if (from >= pacIndexBegin() && from < cheIndexBegin()) {
			pac_busy_minutes += minutes;
		} else if (from >= cheIndexBegin() && from < payIndexBegin()) {
			che_busy_minutes += minutes;
		} else if (from >= payIndexBegin() && from < endIndex()) {
			pay_busy_minutes += minutes;
		}
	}
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

	determineCurrAndNext(current_event->from);

	next->push(*old_presc);

	int idleIndex = getIdleTech(current_event->from);
	if (idleIndex >= 0) {
		Prescription direct(next->front());
		next->pop();
		double event_time = 0;
		bool isLeaving = false;
		bool isRedo = false;
		bool isShift = false;
		if (idleIndex >= pacIndexBegin() && idleIndex < cheIndexBegin()) {
			direct.pac_start = current_event->occur_time;
			direct.pac_end = current_event->occur_time + direct.pac_duration;
			event_time = direct.pac_end;
		} else if (idleIndex >= cheIndexBegin() && idleIndex < payIndexBegin()) {
			if (!direct.redoed)
				direct.che_dis_start = current_event->occur_time;
			direct.che_dis_end = current_event->occur_time + direct.che_dis_duration;

			event_time = direct.che_dis_end;
			isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !direct.redoed) ? true : false;

			if (idleIndex >= cheIndexBegin() && idleIndex < flxIndexEnd() && current_event->from < pacIndexBegin()) {
				direct.pac_start = current_event->occur_time;
				direct.pac_end = current_event->occur_time + direct.pac_duration;
				event_time = direct.pac_end;
				techs[idleIndex].doShift();
				isShift = true;
			}
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
		} else if (isShift) {
			Event shift_event(event_time, 4, idleIndex);
			event_queue.push(shift_event);
		} else if (isRedo) {
			Event redo_event(event_time, 3, idleIndex);
			event_queue.push(redo_event);
		} else {
			Event transfer_event(event_time, 1, idleIndex);
			event_queue.push(transfer_event);
		}
	}

	if (curr->size()) {
		Prescription follow(curr->front());
		curr->pop();
		double event_time;
		bool isRedo = false;
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
			isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !follow.redoed) ? true : false;
		}
		techs[current_event->from].serve(follow);

		if (isRedo) {
			Event new_redo_event(event_time, 3, current_event->from);
			event_queue.push(new_redo_event);
		} else {
			Event new_transfer_event(event_time, 1, current_event->from);
			event_queue.push(new_transfer_event);
		}
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		accumulateBusyMinutes(current_event->from, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
}

void System::prescLeave() {
	std::shared_ptr<Prescription> out = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	reg_queue_minutes += out->getRegWait();
	pac_queue_minutes += out->getPacWait();
	che_queue_minutes += out->getCheWait();
	pay_queue_minutes += out->getPayWait();
	total_stay_minutes += out->pay_end - out->arrive_time;
	
	printf("No. %d presc type %d: %.2f(arrive) %.2f(reg) %.2f(pac) %.2f(che) %.2f(pay) %.2f(pay_end) Total: %.2f\n", 
		out->id, 
		(out->pType == LONGRX ? 1 : (out->pType == SHORTRX ? 2 : 3)),
		out->arrive_time, 
		out->reg_typ_duration,
		out->pac_duration,
		out->che_dis_duration,
		out->pay_duration,
		out->pay_end,
		(out->pay_end - out->arrive_time));
	printf("%.2f(reg wait) %.2f(pac wait) %.2f(che wait) %.2f(pay wait)\n", 
		out->getRegWait(),
		out->getPacWait(),
		out->getCheWait(),
		out->getPayWait());

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
		accumulateBusyMinutes(current_event->from, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
}

void System::prescRedo() {
	std::shared_ptr<Prescription> old_presc = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	curr = &che_queue;
	next = &pac_queue;

	old_presc->redoed = true;
	next->push(*old_presc);

	int idleIndex = -1;
	for (int i = pacIndexBegin(); i != cheIndexBegin(); ++i) {
		if (techs[i].isIdle()) {
			idleIndex = i;
		}
	}

	if (idleIndex >= 0) {
		Prescription direct(next->front());
		next->pop();
		double event_time = 0;

		event_time = current_event->occur_time + direct.pac_duration;

		techs[idleIndex].serve(direct);
		techs[idleIndex].setBusy(current_event->occur_time);

		Event transfer_event(event_time, 1, idleIndex);
		event_queue.push(transfer_event);
	}

	if (curr->size()) {
		Prescription follow(curr->front());
		curr->pop();
		double event_time;
		bool isRedo = false;

		follow.che_dis_start = current_event->occur_time;
		follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
		event_time = follow.che_dis_end;

		isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !follow.redoed) ? true : false;

		techs[current_event->from].serve(follow);

		if (isRedo) {
			Event new_redo_event(event_time, 3, current_event->from);
			event_queue.push(new_redo_event);
		} else {
			Event new_transfer_event(event_time, 1, current_event->from);
			event_queue.push(new_transfer_event);
		}
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		accumulateBusyMinutes(current_event->from, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
}

void System::prescShift() {
	std::shared_ptr<Prescription> old_presc = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	next = &che_queue;

	next->push(*old_presc);


	int idleIndex = getIdleTech(cheIndexBegin() - 1);
	if (idleIndex >= 0) {
		Prescription direct(next->front());
		next->pop();
		double event_time = 0;
		bool isRedo = false;
		
		if (!direct.redoed)
			direct.che_dis_start = current_event->occur_time;
		direct.che_dis_end = current_event->occur_time + direct.che_dis_duration;

		event_time = direct.che_dis_end;
		isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !direct.redoed) ? true : false;
			
		techs[idleIndex].serve(direct);
		techs[idleIndex].setBusy(current_event->occur_time);

		if (isRedo) {
			Event redo_event(event_time, 3, idleIndex);
			event_queue.push(redo_event);
		} else {
			Event transfer_event(event_time, 1, idleIndex);
			event_queue.push(transfer_event);
		}
	}

	techs[current_event->from].undoShift();

	if (next->size()) {
		Prescription follow(next->front());
		next->pop();
		double event_time;

		follow.che_dis_start = current_event->occur_time;
		follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
		event_time = follow.che_dis_end;

		techs[current_event->from].serve(follow);

		Event new_transfer_event(event_time, 1, current_event->from);
		event_queue.push(new_transfer_event);
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		accumulateBusyMinutes(current_event->from, techs[current_event->from].getCurrentBusyMinutes(), true);
	}
}