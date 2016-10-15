#include <queue>
#include <cstdio>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
#include "System.h"
#include "Event.h"
#include "Random.h"

bool operator<(const Event &lhs, const Event &rhs) {
	return rhs.occur_time < lhs.occur_time;
}

System::System(int total_service_minutes, int tech_num):
	total_service_minutes(total_service_minutes), tech_num(tech_num) {
		this->techs = new Tech[tech_num];
		for (int i = 0; i != tech_num; ++i) {
			techs[i].setIndex(i);
		}
	}

System::~System() {
	delete [] techs;
	delete current_event;
}

void System::setTechAllocation(int reg, int pac, int flx, int che, int pay) {
	reg_num = reg;
	pac_num = pac;
	flx_num = flx;
	che_num = che;
	pay_num = pay;

	for (int i = 0; i < reg; ++i)
		techs[i].loc = REG;
	for (int i = reg; i < reg + pac; ++i)
		techs[i].loc = PAC;
	for (int i = reg + pac; i < reg + pac + flx; ++i)
		techs[i].loc = FLX;
	for (int i = reg + pac + flx; i < reg + pac + flx + che; ++i)
		techs[i].loc = CHE;
	for (int i = reg + pac + flx + che; i < reg + pac + flx + che + pay; ++i)
		techs[i].loc = PAY;
}

void System::setReschedule(int reg, int pac, int flx, int che, int pay, double time) {
	this->reschedule_time.push_back(time);
	this->reg_alloc.push_back(reg);
	this->pac_alloc.push_back(pac);
	this->flx_alloc.push_back(flx);
	this->che_alloc.push_back(che);
	this->pay_alloc.push_back(pay);
}

void System::clearReschedule() {
	this->reschedule_time.clear();
	this->reg_alloc.clear();
	this->pac_alloc.clear();
	this->flx_alloc.clear();
	this->che_alloc.clear();
	this->pay_alloc.clear();
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
	total_prescription_num = 0;
	avg_stay_minutes = 0;
	double sum = 0;
	for (int i = 0; i != simulate_num; ++i) {
		if (i == 0)
			display_helper = true;
		else
			display_helper = false;
		sum += run();
		// printf("%d/%d\n", boundary_case, total_prescription_num);
		total_stay_minutes = 0;
		total_prescription_num = 0;
		boundary_case = 0;
		resetId();
	}

	avg_stay_minutes = sum / simulate_num;

	for (int i = 0; i < tech_num; ++i) {
		techs[i].calAvgUtilityRate(simulate_num);
		// printf("%f ", techs[i].getAvgUtilityRate());
		techs[i].resetUtilityRate();
	}
	// printf("\n");

	calAvgUtilityRate(simulate_num);
	printf("Reg:%f Pac:%f Che:%f Pay:%f\n",
		avg_reg_utility_rate,
		avg_pac_utility_rate,
		avg_che_utility_rate,
		avg_pay_utility_rate);
	resetAvgUtilityRate();
	resetUtilityRate();

	calAvgQueueMinutes(simulate_num);
	// printf("RegW:%.2f PacW:%.2f CheW:%.2f PayW:%.2f\n", 
	// 	avg_reg_queue_minutes,
	// 	avg_pac_queue_minutes,
	// 	avg_che_queue_minutes,
	// 	avg_pay_queue_minutes);
	resetTotalAvgQueueMinutes();
	resetAvgQueueMinutes();
}

double System::getRandomInterval(double time) {
	double ret = 0;
	if (time >= 0 && time < 60) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0086961) / 60;
		} while (ret < 0.5);
	} else if (time >= 60 && time < 120) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0124773) / 60;
		} while (ret < 0.5);
	} else if (time >= 120 && time < 180) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0132653) / 60;
		} while (ret < 0.5);
	} else if (time >= 180 && time < 240) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0125397) / 60;
		} while (ret < 0.5);
	} else if (time >= 240 && time < 300) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0077324) / 60;
		} while (ret < 0.5);
	} else if (time >= 300 && time < 360) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0073753) / 60;
		} while (ret < 0.5);
	} else if (time >= 360 && time < 420) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0120862) / 60;
		} while (ret < 0.5);
	} else if (time >= 420 && time < 480) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0116213) / 60;
		} while (ret < 0.5);
	} else {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.0078118) / 60;
		} while (ret < 0.5);
	}
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
		} else if (current_event->type == 5) {
			changeSchedule();
		} else if (current_event->type == 6) {
			curr = nullptr;
			next = nullptr;
			break;
		}
	} while (!event_queue.empty());
	this->end();

	return total_stay_minutes / total_prescription_num;
}

void System::init() {
	current_event = new Event;
	event_queue.push(*current_event);

	for (int i = 0; i < reschedule_time.size(); ++i) {
		Event reschedule_event(reschedule_time[i], 5, 0, i);
		event_queue.push(reschedule_event);
	}

	Event shutdown_event(600, 6);
	event_queue.push(shutdown_event);
}

void System::end() {
	for (int i = 0; i != tech_num; ++i) {
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

int System::getIdleTech(double time) {
	for (int i = 0; i != tech_num; ++i) {
		if (techs[i].isIdle() && techs[i].loc == REG) {
			return i;
		}
	}
	return -1;
}

int System::getIdleTech(Location from, double time) {
	if (from == REG) {
		if (che_queue.size()) {
			for (int i = 0; i < tech_num; ++i) {
				if (techs[i].loc == PAC && techs[i].isIdle())
					return i;
			}
		} else {
			for (int i = 0; i < tech_num; ++i) {
				if ((techs[i].loc == PAC || techs[i].loc == FLX) && techs[i].isIdle())
					return i;
			}
		}
	} else if (from == PAC) {
		for (int i = 0; i < tech_num; ++i) {
			if ((techs[i].loc == FLX || techs[i].loc == CHE) && techs[i].isIdle())
				return i;
		}
	} else if (from == FLX || from == CHE) {
		for (int i = 0; i < tech_num; ++i) {
			if (techs[i].loc == PAY && techs[i].isIdle())
				return i;
		}
	}
	return -1;
}

void System::determineCurrAndNext(int from) {
	if (techs[from].loc == REG) {
		curr = &reg_queue;
		next = &pac_queue;
	} else if (techs[from].loc == PAC) {
		curr = &pac_queue;
		next = &che_queue;
	} else if (techs[from].loc == CHE || techs[from].loc == FLX) {
		curr = &che_queue;
		next = &pay_queue;
	}
}

Location System::determineCurrentLocation(int from) {
	if (rescheduled && !techs[from].rescheduled) {
		if (techs[from].reloc == REG)
			curr = &reg_queue;
		else if (techs[from].reloc == PAC)
			curr = &pac_queue;
		else if (techs[from].reloc == FLX || techs[from].reloc == CHE)
			curr = &che_queue;
		else if (techs[from].reloc == PAY)
			curr = &pay_queue;
		else if (techs[from].reloc == BRK)
			curr = nullptr;

		techs[from].loc = techs[from].reloc;
		techs[from].rescheduled = true;

		// for (int i = 0; i < tech_num; ++i) {
		// 	std::string loc = "";
		// 	if (techs[i].loc == REG)
		// 		loc = "REG";
		// 	else if (techs[i].loc == PAC)
		// 		loc = "PAC";
		// 	else if (techs[i].loc == FLX)
		// 		loc = "FLX";
		// 	else if (techs[i].loc == CHE)
		// 		loc = "CHE";
		// 	else if (techs[i].loc == PAY)
		// 		loc = "PAY";
		// 	else if (techs[i].loc == BRK)
		// 		loc = "BRK";
		// 	std::string reloc = "";
		// 	if (techs[i].reloc == REG)
		// 		reloc = "REG";
		// 	else if (techs[i].reloc == PAC)
		// 		reloc = "PAC";
		// 	else if (techs[i].reloc == FLX)
		// 		reloc = "FLX";
		// 	else if (techs[i].reloc == CHE)
		// 		reloc = "CHE";
		// 	else if (techs[i].reloc == PAY)
		// 		reloc = "PAY";
		// 	else if (techs[i].reloc == BRK)
		// 		reloc = "BRK";
		// 	std::cout << "tech " << i << ": loc-" << loc << " reloc-" << reloc << " rescheduled-" << techs[i].rescheduled << std::endl;
		// }
		// std::cout << "---------------------" << std::endl;

		bool all_scheduled = true;
		for (int i = 0; i < tech_num; ++i) {
			if (!techs[i].rescheduled)
				all_scheduled = false;
		}
		if (all_scheduled) {
			rescheduled = false;
			for (int i = 0; i < tech_num; ++i)
				techs[i].rescheduled = false;
		}
	}
	return techs[from].loc;
}

void System::accumulateBusyMinutes(Location from, double minutes, bool shift) {
	if (shift) {
		shift_busy_minutes += minutes;
	} else {
		if (from == REG) {
			reg_busy_minutes += minutes;
		} else if (from == PAC) {
			pac_busy_minutes += minutes;
		} else if (from == CHE) {
			che_busy_minutes += minutes;
		} else if (from == PAY) {
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
	} else {
		// std::cout << "Queue remain: reg-" << reg_queue.size() << " "
		// 							"pac-" << pac_queue.size() << " "
		// 							"che-" << che_queue.size() << " "
		// 							"pay-" << pay_queue.size() << std::endl;
	}

	Prescription* presc = new Prescription(current_event->occur_time, getRandomPrescType());
	presc->id = getId();
	reg_queue.push(*presc);

	int idleIndex = getIdleTech(current_event->occur_time);
	if (idleIndex >= 0) {
		*presc = reg_queue.front();
		reg_queue.pop();
		presc->reg_typ_start = current_event->occur_time;
		presc->reg_typ_end = current_event->occur_time + presc->reg_typ_duration;
		techs[idleIndex].serve(*presc);
		techs[idleIndex].setBusy(current_event->occur_time);
		techs[idleIndex].setNextFinishTime(presc->reg_typ_end);

		Event transfer_event(presc->reg_typ_end, 1, idleIndex);
		event_queue.push(transfer_event);
	}

	delete presc;
}

void System::prescTransfer() {
	std::shared_ptr<Prescription> old_presc = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	determineCurrAndNext(current_event->from);

	next->push(*old_presc);

	int idleIndex = getIdleTech(techs[current_event->from].loc, current_event->occur_time);
	if (idleIndex >= 0) {
		Prescription direct(next->front());
		next->pop();
		double event_time = 0;
		bool isLeaving = false;
		bool isRedo = false;
		bool isShift = false;
		Location location = techs[idleIndex].loc;
		if (location == PAC) {
			direct.pac_start = current_event->occur_time;
			direct.pac_end = current_event->occur_time + direct.pac_duration;
			event_time = direct.pac_end;
		} else if (location == FLX || location == CHE) {
			if (!direct.redoed)
				direct.che_dis_start = current_event->occur_time;
			direct.che_dis_end = current_event->occur_time + direct.che_dis_duration;

			event_time = direct.che_dis_end;
			isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !direct.redoed) ? true : false;

			if (location == FLX && techs[current_event->from].loc == REG) {
				direct.pac_start = current_event->occur_time;
				direct.pac_end = current_event->occur_time + direct.pac_duration;
				event_time = direct.pac_end;
				techs[idleIndex].doShift();
				isShift = true;
			}
		} else if (location == PAY) {
			direct.pay_start = current_event->occur_time;
			direct.pay_end = current_event->occur_time + direct.pay_duration;
			event_time = direct.pay_end;
			isLeaving = true;
		}
		techs[idleIndex].serve(direct);
		techs[idleIndex].setBusy(current_event->occur_time);
		techs[idleIndex].setNextFinishTime(event_time);


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

	Location curr_loc = determineCurrentLocation(current_event->from);
	if (curr_loc == PAY) {
		if (pay_queue.size()) {
			Prescription presc = pay_queue.front();
			pay_queue.pop();
			presc.pay_start = current_event->occur_time;
			presc.pay_end = current_event->occur_time + presc.pay_duration;
			techs[current_event->from].serve(presc);
			techs[current_event->from].setNextFinishTime(presc.pay_end);

			Event temp_event(presc.pay_end, 2, current_event->from);
			event_queue.push(temp_event);
		} else {
			techs[current_event->from].setIdle(current_event->occur_time);
			techs[current_event->from].setNextFinishTime(0);
			accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
		}
		curr = nullptr;
		next = nullptr;
		return;
	}                                                                                                              

	if (curr && curr->size() && techs[current_event->from].loc != BRK) {
		Prescription follow(curr->front());
		curr->pop();
		double event_time;
		bool isRedo = false;
		Location location = techs[current_event->from].loc;
		if (location == REG) {
			follow.reg_typ_start = current_event->occur_time;
			follow.reg_typ_end = current_event->occur_time + follow.reg_typ_duration;
			event_time = follow.reg_typ_end;
		} else if (location == PAC) {
			follow.pac_start = current_event->occur_time;
			follow.pac_end = current_event->occur_time + follow.pac_duration;
			event_time = follow.pac_end;
		} else if (location == FLX || location == CHE) {
			follow.che_dis_start = current_event->occur_time;
			follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
			event_time = follow.che_dis_end;
			isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !follow.redoed) ? true : false;
		}
		techs[current_event->from].serve(follow);
		techs[current_event->from].setNextFinishTime(event_time);

		if (isRedo) {
			Event new_redo_event(event_time, 3, current_event->from);
			event_queue.push(new_redo_event);
		} else {
			Event new_transfer_event(event_time, 1, current_event->from);
			event_queue.push(new_transfer_event);
		}
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		techs[current_event->from].setNextFinishTime(0);
		accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
	curr = nullptr;
	next = nullptr;
}

void System::prescLeave() {
	std::shared_ptr<Prescription> out = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	reg_queue_minutes += out->getRegWait();
	pac_queue_minutes += out->getPacWait();
	che_queue_minutes += out->getCheWait();
	pay_queue_minutes += out->getPayWait();
	total_stay_minutes += out->pay_end - out->arrive_time;

	if ((out->pay_end - out->arrive_time) > 20)
		boundary_case++;

	
	// printf("No. %d presc type %d: %.2f(arrive) %.2f(reg) %.2f(pac) %.2f(che) %.2f(pay) %.2f(pay_end) Total: %.2f\n", 
	// 	out->id, 
	// 	(out->pType == LONGRX ? 1 : (out->pType == SHORTRX ? 2 : 3)),
	// 	out->arrive_time, 
	// 	out->reg_typ_duration,
	// 	out->pac_duration,
	// 	out->che_dis_duration,
	// 	out->pay_duration,
	// 	out->pay_end,
	// 	(out->pay_end - out->arrive_time));
	// printf("%.2f(reg wait) %.2f(pac wait) %.2f(che wait) %.2f(pay wait)\n", 
	// 	out->getRegWait(),
	// 	out->getPacWait(),
	// 	out->getCheWait(),
	// 	out->getPayWait());

	Location curr_loc = determineCurrentLocation(current_event->from);
	if (curr_loc != PAY) {
		if (curr && curr->size() && techs[current_event->from].loc != BRK) {
			Prescription follow(curr->front());
			curr->pop();
			double event_time;
			bool isRedo = false;
			Location location = techs[current_event->from].loc;
			if (location == REG) {
				follow.reg_typ_start = current_event->occur_time;
				follow.reg_typ_end = current_event->occur_time + follow.reg_typ_duration;
				event_time = follow.reg_typ_end;
			} else if (location == PAC) {
				follow.pac_start = current_event->occur_time;
				follow.pac_end = current_event->occur_time + follow.pac_duration;
				event_time = follow.pac_end;
			} else if (location == FLX || location == CHE) {
				follow.che_dis_start = current_event->occur_time;
				follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
				event_time = follow.che_dis_end;
				isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !follow.redoed) ? true : false;
			}
			techs[current_event->from].serve(follow);
			techs[current_event->from].setNextFinishTime(event_time);

			if (isRedo) {
				Event new_redo_event(event_time, 3, current_event->from);
				event_queue.push(new_redo_event);
			} else {
				Event new_transfer_event(event_time, 1, current_event->from);
				event_queue.push(new_transfer_event);
			}
		} else {
			techs[current_event->from].setIdle(current_event->occur_time);
			techs[current_event->from].setNextFinishTime(0);
			accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
		}
		curr = nullptr;
		next = nullptr;
		return;
	}

	if (pay_queue.size() && techs[current_event->from].loc != BRK) {
		Prescription presc = pay_queue.front();
		pay_queue.pop();
		presc.pay_start = current_event->occur_time;
		presc.pay_end = current_event->occur_time + presc.pay_duration;
		techs[current_event->from].serve(presc);
		techs[current_event->from].setNextFinishTime(presc.pay_end);

		Event temp_event(presc.pay_end, 2, current_event->from);
		event_queue.push(temp_event);
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		techs[current_event->from].setNextFinishTime(0);
		accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
	curr = nullptr;
	next = nullptr;
}

void System::prescRedo() {
	std::shared_ptr<Prescription> old_presc = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	curr = &che_queue;
	next = &pac_queue;

	old_presc->redoed = true;
	next->push(*old_presc);

	int idleIndex = -1;
	for (int i = 0; i != tech_num; ++i) {
		if (techs[i].isIdle() && techs[i].loc == PAC) {
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
		techs[idleIndex].setNextFinishTime(event_time);

		Event transfer_event(event_time, 1, idleIndex);
		event_queue.push(transfer_event);
	}

	Location curr_loc = determineCurrentLocation(current_event->from);
	if (curr_loc == PAY) {
		if (pay_queue.size() && techs[current_event->from].loc != BRK) {
			Prescription presc = pay_queue.front();
			pay_queue.pop();
			presc.pay_start = current_event->occur_time;
			presc.pay_end = current_event->occur_time + presc.pay_duration;
			techs[current_event->from].serve(presc);
			techs[current_event->from].setNextFinishTime(presc.pay_end);

			Event temp_event(presc.pay_end, 2, current_event->from);
			event_queue.push(temp_event);
		} else {
			techs[current_event->from].setIdle(current_event->occur_time);
			techs[current_event->from].setNextFinishTime(0);
			accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
		}
		curr = nullptr;
		next = nullptr;
		return;
	} else if (curr_loc != CHE && curr_loc != FLX) {
		if (curr && curr->size() && techs[current_event->from].loc != BRK) {
			Prescription follow(curr->front());
			curr->pop();
			double event_time;
			Location location = techs[current_event->from].loc;
			if (location == REG) {
				follow.reg_typ_start = current_event->occur_time;
				follow.reg_typ_end = current_event->occur_time + follow.reg_typ_duration;
				event_time = follow.reg_typ_end;
			} else if (location == PAC) {
				follow.pac_start = current_event->occur_time;
				follow.pac_end = current_event->occur_time + follow.pac_duration;
				event_time = follow.pac_end;
			}
			techs[current_event->from].serve(follow);
			techs[current_event->from].setNextFinishTime(event_time);

			Event new_transfer_event(event_time, 1, current_event->from);
			event_queue.push(new_transfer_event);
		} else {
			techs[current_event->from].setIdle(current_event->occur_time);
			techs[current_event->from].setNextFinishTime(0);
			accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
		}
		curr = nullptr;
		next = nullptr;
		return;
	}

	if (curr->size() && techs[current_event->from].loc != BRK) {
		Prescription follow(curr->front());
		curr->pop();
		double event_time;
		bool isRedo = false;

		follow.che_dis_start = current_event->occur_time;
		follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
		event_time = follow.che_dis_end;

		isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !follow.redoed) ? true : false;

		techs[current_event->from].serve(follow);
		techs[current_event->from].setNextFinishTime(event_time);

		if (isRedo) {
			Event new_redo_event(event_time, 3, current_event->from);
			event_queue.push(new_redo_event);
		} else {
			Event new_transfer_event(event_time, 1, current_event->from);
			event_queue.push(new_transfer_event);
		}
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		techs[current_event->from].setNextFinishTime(0);
		accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
	curr = nullptr;
	next = nullptr;
}

void System::prescShift() {
	std::shared_ptr<Prescription> old_presc = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	next = &che_queue;

	next->push(*old_presc);


	int idleIndex = getIdleTech(PAC, current_event->occur_time);
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
		techs[idleIndex].setNextFinishTime(event_time);

		if (isRedo) {
			Event redo_event(event_time, 3, idleIndex);
			event_queue.push(redo_event);
		} else {
			Event transfer_event(event_time, 1, idleIndex);
			event_queue.push(transfer_event);
		}
	}

	techs[current_event->from].undoShift();

	Location curr_loc = determineCurrentLocation(current_event->from);
	if (curr_loc == PAY) {
		if (pay_queue.size() && techs[current_event->from].loc != BRK) {
			Prescription presc = pay_queue.front();
			pay_queue.pop();
			presc.pay_start = current_event->occur_time;
			presc.pay_end = current_event->occur_time + presc.pay_duration;
			techs[current_event->from].serve(presc);
			techs[current_event->from].setNextFinishTime(presc.pay_end);

			Event temp_event(presc.pay_end, 2, current_event->from);
			event_queue.push(temp_event);
		} else {
			techs[current_event->from].setIdle(current_event->occur_time);
			techs[current_event->from].setNextFinishTime(0);
			accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
		}
		curr = nullptr;
		next = nullptr;
		return;
	} else if (curr_loc != CHE && curr_loc != FLX) {
		if (curr && curr->size() && techs[current_event->from].loc != BRK) {
			Prescription follow(curr->front());
			curr->pop();
			double event_time;
			bool isRedo = false;
			Location location = techs[current_event->from].loc;
			if (location == REG) {
				follow.reg_typ_start = current_event->occur_time;
				follow.reg_typ_end = current_event->occur_time + follow.reg_typ_duration;
				event_time = follow.reg_typ_end;
			} else if (location == PAC) {
				follow.pac_start = current_event->occur_time;
				follow.pac_end = current_event->occur_time + follow.pac_duration;
				event_time = follow.pac_end;
			} else if (location == FLX || location == CHE) {
				follow.che_dis_start = current_event->occur_time;
				follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
				event_time = follow.che_dis_end;
				isRedo = (Random::getRandom(UNIFORM, 100) < 2 && !follow.redoed) ? true : false;
			}
			techs[current_event->from].serve(follow);
			techs[current_event->from].setNextFinishTime(event_time);

			if (isRedo) {
				Event new_redo_event(event_time, 3, current_event->from);
				event_queue.push(new_redo_event);
			} else {
				Event new_transfer_event(event_time, 1, current_event->from);
				event_queue.push(new_transfer_event);
			}
		} else {
			techs[current_event->from].setIdle(current_event->occur_time);
			techs[current_event->from].setNextFinishTime(0);
			accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
		}
		curr = nullptr;
		next = nullptr;
		return;
	}

	if (next->size() && techs[current_event->from].loc != BRK) {
		Prescription follow(next->front());
		next->pop();
		double event_time;

		follow.che_dis_start = current_event->occur_time;
		follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
		event_time = follow.che_dis_end;

		techs[current_event->from].serve(follow);
		techs[current_event->from].setNextFinishTime(event_time);

		Event new_transfer_event(event_time, 1, current_event->from);
		event_queue.push(new_transfer_event);
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		techs[current_event->from].setNextFinishTime(0);
		accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), true);
	}
	curr = nullptr;
	next = nullptr;
}

void System::changeSchedule() {
	int idx = current_event->schedule_index;
	rescheduled = true;
	int re_tech_num = reg_alloc[idx] + pac_alloc[idx] + flx_alloc[idx] + che_alloc[idx] + pay_alloc[idx];

	for (int i = 0; i < reg_alloc[idx]; ++i)
		techs[i].reloc = REG;
	for (int i = reg_alloc[idx]; i < reg_alloc[idx] + pac_alloc[idx]; ++i)
		techs[i].reloc = PAC;
	for (int i = reg_alloc[idx] + pac_alloc[idx]; i < reg_alloc[idx] + pac_alloc[idx] + flx_alloc[idx]; ++i)
		techs[i].reloc = FLX;
	for (int i = reg_alloc[idx] + pac_alloc[idx] + flx_alloc[idx]; i < reg_alloc[idx] + pac_alloc[idx] + flx_alloc[idx] + che_alloc[idx]; ++i)
		techs[i].reloc = CHE;
	for (int i = reg_alloc[idx] + pac_alloc[idx] + flx_alloc[idx] + che_alloc[idx]; i < re_tech_num; ++i)
		techs[i].reloc = PAY;
	for (int i = re_tech_num; i < tech_num; ++i) {
		techs[i].reloc = BRK;
	}

	// if (display_helper) {
	// 	for (int i = 0; i < tech_num; ++i) {
	// 		std::string loc = "";
	// 		if (techs[i].loc == REG)
	// 			loc = "REG";
	// 		else if (techs[i].loc == PAC)
	// 			loc = "PAC";
	// 		else if (techs[i].loc == FLX)
	// 			loc = "FLX";
	// 		else if (techs[i].loc == CHE)
	// 			loc = "CHE";
	// 		else if (techs[i].loc == PAY)
	// 			loc = "PAY";
	// 		else if (techs[i].loc == BRK)
	// 			loc = "BRK";
	// 		std::string reloc = "";
	// 		if (techs[i].reloc == REG)
	// 			reloc = "REG";
	// 		else if (techs[i].reloc == PAC)
	// 			reloc = "PAC";
	// 		else if (techs[i].reloc == FLX)
	// 			reloc = "FLX";
	// 		else if (techs[i].reloc == CHE)
	// 			reloc = "CHE";
	// 		else if (techs[i].reloc == PAY)
	// 			reloc = "PAY";
	// 		else if (techs[i].reloc == BRK)
	// 			reloc = "BRK";
	// 		std::cout << "tech " << i << ": loc-" << loc << " reloc-" << reloc << " rescheduled-" << techs[i].rescheduled << std::endl;
	// 	}
	// 	std::cout << "===========" << std::endl;
	// }

	for (int i = 0; i < tech_num; ++i) {
		if (techs[i].isIdle()) {
			techs[i].loc = techs[i].reloc;
			techs[i].rescheduled = true;
		}
	}

	// for (int i = 0; i < tech_num; ++i) {
	// 	std::string loc = "";
	// 	if (techs[i].loc == REG)
	// 		loc = "REG";
	// 	else if (techs[i].loc == PAC)
	// 		loc = "PAC";
	// 	else if (techs[i].loc == FLX)
	// 		loc = "FLX";
	// 	else if (techs[i].loc == CHE)
	// 		loc = "CHE";
	// 	else if (techs[i].loc == PAY)
	// 		loc = "PAY";
	// 	else if (techs[i].loc == BRK)
	// 		loc = "BRK";
	// 	std::string reloc = "";
	// 	if (techs[i].reloc == REG)
	// 		reloc = "REG";
	// 	else if (techs[i].reloc == PAC)
	// 		reloc = "PAC";
	// 	else if (techs[i].reloc == FLX)
	// 		reloc = "FLX";
	// 	else if (techs[i].reloc == CHE)
	// 		reloc = "CHE";
	// 	else if (techs[i].reloc == PAY)
	// 		reloc = "PAY";
	// 	else if (techs[i].reloc == BRK)
	// 		reloc = "BRK";
	// 	std::cout << "tech " << i << ": loc-" << loc << " reloc-" << reloc << " rescheduled-" << techs[i].rescheduled << std::endl;
	// }
}