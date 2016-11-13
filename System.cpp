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
	total_service_minutes(total_service_minutes), 
	tech_num(tech_num),
	reschedule_time(8, 0),
	reg_alloc(8, 0),
	pac_alloc(8, 0),
	flx_alloc(8, 0),
	che_alloc(8, 0),
	pay_alloc(8, 0) {
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
	if (time == 60) {
		reschedule_time[0] = 60;
		reg_alloc[0] = reg;
		pac_alloc[0] = pac;
		flx_alloc[0] = flx;
		che_alloc[0] = che;
		pay_alloc[0] = pay;
	} else if (time == 120) {
		reschedule_time[1] = 120;
		reg_alloc[1] = reg;
		pac_alloc[1] = pac;
		flx_alloc[1] = flx;
		che_alloc[1] = che;
		pay_alloc[1] = pay;
	} else if (time == 180) {
		reschedule_time[2] = 180;
		reg_alloc[2] = reg;
		pac_alloc[2] = pac;
		flx_alloc[2] = flx;
		che_alloc[2] = che;
		pay_alloc[2] = pay;
	} else if (time == 240) {
		reschedule_time[3] = 240;
		reg_alloc[3] = reg;
		pac_alloc[3] = pac;
		flx_alloc[3] = flx;
		che_alloc[3] = che;
		pay_alloc[3] = pay;
	} else if (time == 300) {
		reschedule_time[4] = 300;
		reg_alloc[4] = reg;
		pac_alloc[4] = pac;
		flx_alloc[4] = flx;
		che_alloc[4] = che;
		pay_alloc[4] = pay;
	} else if (time == 360) {
		reschedule_time[5] = 360;
		reg_alloc[5] = reg;
		pac_alloc[5] = pac;
		flx_alloc[5] = flx;
		che_alloc[5] = che;
		pay_alloc[5] = pay;
	} else if (time == 420) {
		reschedule_time[6] = 420;
		reg_alloc[6] = reg;
		pac_alloc[6] = pac;
		flx_alloc[6] = flx;
		che_alloc[6] = che;
		pay_alloc[6] = pay;
	} else if (time == 480) {
		reschedule_time[7] = 480;
		reg_alloc[7] = reg;
		pac_alloc[7] = pac;
		flx_alloc[7] = flx;
		che_alloc[7] = che;
		pay_alloc[7] = pay;
	}
}

void System::clearReschedule() {
	for (int i = 0; i < 8; ++i) {
		reschedule_time[i] = 0;
		reg_alloc[i] = 0;
		pac_alloc[i] = 0;
		flx_alloc[i] = 0;
		che_alloc[i] = 0;
		pay_alloc[i] = 0;
	}
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
	total_avg_reg_queue_minutes += reg_queue_minutes / prescription_num;
	total_avg_pac_queue_minutes += pac_queue_minutes / prescription_num;
	total_avg_che_queue_minutes += che_queue_minutes / prescription_num;
	total_avg_pay_queue_minutes += pay_queue_minutes / prescription_num;
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
	prescription_num = 0;
	total_stay_minutes = 0;
	avg_stay_minutes = 0;
	double sum = 0;
	for (int i = 0; i != simulate_num; ++i) {
		if (i == 0)
			display_helper = true;
		else
			display_helper = false;
		sum += run();
		total_stay_minutes = 0;
		prescription_num = 0;
		debug_prescription_num = 0;
		limit = 0;
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

	printf("==============================RESULT==============================\n");
	printf("=== Reg: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f ===\n",
		reg_hourly_utility[0]/simulate_num,
		reg_hourly_utility[1]/simulate_num,
		reg_hourly_utility[2]/simulate_num,
		reg_hourly_utility[3]/simulate_num,
		reg_hourly_utility[4]/simulate_num,
		reg_hourly_utility[5]/simulate_num,
		reg_hourly_utility[6]/simulate_num,
		reg_hourly_utility[7]/simulate_num,
		reg_hourly_utility[8]/simulate_num);
	printf("=== Pac: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f ===\n",
		pac_hourly_utility[0]/simulate_num,
		pac_hourly_utility[1]/simulate_num,
		pac_hourly_utility[2]/simulate_num,
		pac_hourly_utility[3]/simulate_num,
		pac_hourly_utility[4]/simulate_num,
		pac_hourly_utility[5]/simulate_num,
		pac_hourly_utility[6]/simulate_num,
		pac_hourly_utility[7]/simulate_num,
		pac_hourly_utility[8]/simulate_num);
	printf("=== Che: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f ===\n",
		che_hourly_utility[0]/simulate_num,
		che_hourly_utility[1]/simulate_num,
		che_hourly_utility[2]/simulate_num,
		che_hourly_utility[3]/simulate_num,
		che_hourly_utility[4]/simulate_num,
		che_hourly_utility[5]/simulate_num,
		che_hourly_utility[6]/simulate_num,
		che_hourly_utility[7]/simulate_num,
		che_hourly_utility[8]/simulate_num);
	printf("=== Pay: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f ===\n",
		pay_hourly_utility[0]/simulate_num,
		pay_hourly_utility[1]/simulate_num,
		pay_hourly_utility[2]/simulate_num,
		pay_hourly_utility[3]/simulate_num,
		pay_hourly_utility[4]/simulate_num,
		pay_hourly_utility[5]/simulate_num,
		pay_hourly_utility[6]/simulate_num,
		pay_hourly_utility[7]/simulate_num,
		pay_hourly_utility[8]/simulate_num);
	printf("==================================================================\n");
	printf("AVG: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
		hourly_avg_stay_minutes[0]/simulate_num,
		hourly_avg_stay_minutes[1]/simulate_num,
		hourly_avg_stay_minutes[2]/simulate_num,
		hourly_avg_stay_minutes[3]/simulate_num,
		hourly_avg_stay_minutes[4]/simulate_num,
		hourly_avg_stay_minutes[5]/simulate_num,
		hourly_avg_stay_minutes[6]/simulate_num,
		hourly_avg_stay_minutes[7]/simulate_num,
		hourly_avg_stay_minutes[8]/simulate_num);
	// printf("Limit: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
	// 	hourly_limit[0]/simulate_num,
	// 	hourly_limit[1]/simulate_num,
	// 	hourly_limit[2]/simulate_num,
	// 	hourly_limit[3]/simulate_num,
	// 	hourly_limit[4]/simulate_num,
	// 	hourly_limit[5]/simulate_num,
	// 	hourly_limit[6]/simulate_num,
	// 	hourly_limit[7]/simulate_num,
	// 	hourly_limit[8]/simulate_num);

	// printf("Reg:%f Pac:%f Che:%f Pay:%f\n",
	// 	avg_reg_utility_rate,
	// 	avg_pac_utility_rate,
	// 	avg_che_utility_rate,
	// 	avg_pay_utility_rate);
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
	if (prescription_limit == 0) {
		ret = 600;
	}
	if (time >= 0 && time < 60) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.00798) / 60;
		} while (ret < 0.2);
	} else if (time >= 60 && time < 120) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.01122) / 60;
		} while (ret < 0.2);
	} else if (time >= 120 && time < 180) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.01190) / 60;
		} while (ret < 0.2);
	} else if (time >= 180 && time < 240) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.01047) / 60;
		} while (ret < 0.2);
	} else if (time >= 240 && time < 300) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.00621) / 60;
		} while (ret < 0.2);
	} else if (time >= 300 && time < 360) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.00724) / 60;
		} while (ret < 0.2);
	} else if (time >= 360 && time < 420) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.01127) / 60;
		} while (ret < 0.2);
	} else if (time >= 420 && time < 480) {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.01054) / 60;
		} while (ret < 0.2);
	} else {
		do {
			ret = Random::getRandom(EXPONENTIAL, 0.00740) / 60;
		} while (ret < 0.2);
	}
	--prescription_limit;
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
		} else if (current_event->type == 7) {
			debug(latest_event_time+0.0001);
		}
	} while (!event_queue.empty());
	this->end();

	double avg = total_stay_minutes / prescription_num;
	// printf("%.2f ", avg);
	// printf("Compare: %d vs %d\n", debug_prescription_num, prescription_num);
	return avg;
}

void System::init() {
	total_stay_minutes = 0;
	current_event = new Event;
	event_queue.push(*current_event);

	/* debug event */
	for (int i = 0; i < 9; ++i) {
		Event debug_event((i+1)*60-0.0001, 7);
		event_queue.push(debug_event);
	}

	for (int i = 0; i < reschedule_time.size() && reschedule_time[i] != 0; ++i) {
		Event reschedule_event(reschedule_time[i], 5, 0, i);
		event_queue.push(reschedule_event);
	}

	// Event shutdown_event(600, 6);
	// event_queue.push(shutdown_event);
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

	total_prescription_num += prescription_num;
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
	debug_prescription_num++;

	int idleIndex = getIdleTech(current_event->occur_time);
	if (idleIndex >= 0) {
		*presc = reg_queue.front();
		reg_queue.pop();
		presc->reg_typ_start = current_event->occur_time;
		presc->reg_typ_end = current_event->occur_time + presc->reg_typ_duration;
		// printf("No.%d: REG %.3f - %.3f - %.3f\n", presc->id, presc->reg_typ_start, presc->reg_typ_end, presc->reg_typ_duration);
		techs[idleIndex].serve(*presc);
		techs[idleIndex].setBusy(current_event->occur_time);
		// printf("tech %d busy at %.3f for %d\n", idleIndex, current_event->occur_time, presc->id);
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
			// printf("No.%d: PAC %.3f - %.3f - %.3f in transfer from %d to %d\n", direct.id, direct.pac_start, direct.pac_end, direct.pac_duration, current_event->from, idleIndex);
			event_time = direct.pac_end;
		} else if (location == FLX || location == CHE) {
			if (!direct.redoed)
				direct.che_dis_start = current_event->occur_time;
			direct.che_dis_end = current_event->occur_time + direct.che_dis_duration;
			// printf("No.%d: CHE %.3f - %.3f - %.3f in transfer from %d to %d\n", direct.id, direct.che_dis_start, direct.che_dis_end, direct.che_dis_duration, current_event->from, idleIndex);

			event_time = direct.che_dis_end;
			isRedo = (Random::getRandom(UNIFORM, 100) < 0 && !direct.redoed) ? true : false;

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
			// printf("No.%d: PAY %.3f - %.3f - %.3f in transfer from %d to %d\n", direct.id, direct.pay_start, direct.pay_end, direct.pay_duration, current_event->from, idleIndex);
			isLeaving = true;
		}
		techs[idleIndex].serve(direct);
		techs[idleIndex].setBusy(current_event->occur_time);
		// printf("tech %d busy at %.3f for %d when %d\n", idleIndex, current_event->occur_time, direct.id, techs[idleIndex].loc == BRK);
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
			// printf("No.%d: PAY %.3f - %.3f - %.3f in transfer2\n", presc.id, presc.pay_start, presc.pay_end, presc.pay_duration);
			techs[current_event->from].serve(presc);
			techs[current_event->from].setBusy(current_event->occur_time);
			// printf("tech %d busy at %.3f for %d when %d\n", current_event->from, current_event->occur_time, presc.id, techs[current_event->from].loc == BRK);
			techs[current_event->from].setNextFinishTime(presc.pay_end);

			Event leave_event(presc.pay_end, 2, current_event->from);
			event_queue.push(leave_event);
		} else {
			techs[current_event->from].setIdle(current_event->occur_time);
			// printf("tech %d idle at %.3f when %d\n", current_event->from, current_event->occur_time, techs[current_event->from].loc == BRK);
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
			// printf("No.%d: REG %.3f - %.3f - %.3f in transfer3 from %d\n", follow.id, follow.reg_typ_start, follow.reg_typ_end, follow.reg_typ_duration, current_event->from);
			event_time = follow.reg_typ_end;
		} else if (location == PAC) {
			follow.pac_start = current_event->occur_time;
			follow.pac_end = current_event->occur_time + follow.pac_duration;
			// printf("No.%d: PAC %.3f - %.3f - %.3f in transfer3 from %d\n", follow.id, follow.pac_start, follow.pac_end, follow.pac_duration, current_event->from);
			event_time = follow.pac_end;
		} else if (location == FLX || location == CHE) {
			follow.che_dis_start = current_event->occur_time;
			follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
			// printf("No.%d: CHE %.3f - %.3f - %.3f in transfer3 from %d\n", follow.id, follow.che_dis_start, follow.che_dis_end, follow.che_dis_duration, current_event->from);
			event_time = follow.che_dis_end;
			isRedo = (Random::getRandom(UNIFORM, 100) < 0 && !follow.redoed) ? true : false;
		}
		techs[current_event->from].serve(follow);
		// techs[current_event->from].setBusy(current_event->occur_time);
		// printf("tech %d busy at %.3f for %d when %d\n", current_event->from, current_event->occur_time, follow.id, techs[current_event->from].loc == BRK);
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
		// printf("tech %d idle at %.3f when %d\n", current_event->from, current_event->occur_time, techs[current_event->from].loc == BRK);
		accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
	curr = nullptr;
	next = nullptr;
}

void System::prescLeave() {
	prescription_num++;
	hourly_prescription_num++;
	std::shared_ptr<Prescription> out = std::make_shared<Prescription>(techs[current_event->from].getPrescription());

	reg_queue_minutes += out->getRegWait();
	pac_queue_minutes += out->getPacWait();
	che_queue_minutes += out->getCheWait();
	pay_queue_minutes += out->getPayWait();
	double real_duration = out->che_dis_end - (out->che_dis_duration)/2 - out->arrive_time;
	total_stay_minutes += real_duration;
	hourly_stay_minutes += real_duration;

	if (real_duration > 20) {
		++limit;
	}

	// printf("No.%d: LEAVE %.3f in leave\n", out->id, out->pay_end);

	printf("No. %d presc type %d: %.2f(arrive) %.2f(reg) %.2f(pac) %.2f(che) %.2f(pay_end) Total: %.2f\n", 
		out->id, 
		(out->pType == LONGRX ? 1 : (out->pType == SHORTRX ? 2 : 3)),
		out->arrive_time, 
		out->reg_typ_end,
		out->pac_end,
		out->che_dis_end,
		out->pay_end,
		real_duration);

	printf("%.2f(reg wait) %.2f(pac wait) %.2f(che wait) %.2f(pay wait)\n", 
		out->getRegWait(),
		out->getPacWait(),
		out->getCheWait(),
		out->getPayWait());

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
				// printf("No.%d: REG %.3f - %.3f - %.3f in leave\n", follow.id, follow.reg_typ_start, follow.reg_typ_end, follow.reg_typ_duration);
				event_time = follow.reg_typ_end;
			} else if (location == PAC) {
				follow.pac_start = current_event->occur_time;
				follow.pac_end = current_event->occur_time + follow.pac_duration;
				// printf("No.%d: PAC %.3f - %.3f - %.3f in leave\n", follow.id, follow.pac_start, follow.pac_end, follow.pac_duration);
				event_time = follow.pac_end;
			} else if (location == FLX || location == CHE) {
				follow.che_dis_start = current_event->occur_time;
				follow.che_dis_end = current_event->occur_time + follow.che_dis_duration;
				// printf("No.%d: CHE %.3f - %.3f - %.3f in leave\n", follow.id, follow.che_dis_start, follow.che_dis_end, follow.che_dis_duration);
				event_time = follow.che_dis_end;
				isRedo = (Random::getRandom(UNIFORM, 100) < 0 && !follow.redoed) ? true : false;
			}
			techs[current_event->from].serve(follow);
			techs[current_event->from].setBusy(current_event->occur_time);
			// printf("tech %d busy at %.3f for %d when %d\n", current_event->from, current_event->occur_time, follow.id, techs[current_event->from].loc == BRK);
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
			// printf("tech %d idle at %.3f when %d\n", current_event->from, current_event->occur_time, techs[current_event->from].loc == BRK);
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
		// printf("No.%d: PAY %.3f - %.3f - %.3f in leave2\n", presc.id, presc.pay_start, presc.pay_end, presc.pay_duration);
		techs[current_event->from].serve(presc);
		// techs[current_event->from].setBusy(current_event->occur_time);
		// printf("tech %d busy at %.3f for %d when %d\n", current_event->from, current_event->occur_time, presc.id, techs[current_event->from].loc == BRK);
		techs[current_event->from].setNextFinishTime(presc.pay_end);

		Event leave_event(presc.pay_end, 2, current_event->from);
		event_queue.push(leave_event);
	} else {
		techs[current_event->from].setIdle(current_event->occur_time);
		techs[current_event->from].setNextFinishTime(0);
		// printf("tech %d idle at %.3f when %d\n", current_event->from, current_event->occur_time, techs[current_event->from].loc == BRK);
		accumulateBusyMinutes(techs[current_event->from].loc, techs[current_event->from].getCurrentBusyMinutes(), false);
	}
	curr = nullptr;
	next = nullptr;
}

void System::prescRedo() {
	printf("REDO\n");
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

			Event leave_event(presc.pay_end, 2, current_event->from);
			event_queue.push(leave_event);
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

		isRedo = (Random::getRandom(UNIFORM, 100) < 0 && !follow.redoed) ? true : false;

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
	printf("SHIFT\n");
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
		isRedo = (Random::getRandom(UNIFORM, 100) < 0 && !direct.redoed) ? true : false;
			
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

			Event leave_event(presc.pay_end, 2, current_event->from);
			event_queue.push(leave_event);
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
				isRedo = (Random::getRandom(UNIFORM, 100) < 0 && !follow.redoed) ? true : false;
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

	int reg_alloc_idx = 0;
	int pac_alloc_idx = 0;
	int flx_alloc_idx = 0;
	int che_alloc_idx = 0;
	int pay_alloc_idx = 0;

	reg_alloc_idx = reg_alloc[idx];
	pac_alloc_idx = pac_alloc[idx];
	flx_alloc_idx = flx_alloc[idx];
	che_alloc_idx = che_alloc[idx];
	pay_alloc_idx = pay_alloc[idx];

	reg_num = reg_alloc[idx];
	pac_num = pac_alloc[idx];
	flx_num = flx_alloc[idx];
	che_num = che_alloc[idx];
	pay_num = pay_alloc[idx];

	// printf("reschedule: %d %d %d %d %d\n",
	// 	reg_alloc_idx,
	// 	pac_alloc_idx,
	// 	flx_alloc_idx,
	// 	che_alloc_idx,
	// 	pay_alloc_idx);

	rescheduled = true;
	int re_tech_num = reg_alloc_idx + pac_alloc_idx + flx_alloc_idx + che_alloc_idx + pay_alloc_idx;

	for (int i = 0; i < reg_alloc_idx; ++i)
		techs[i].reloc = REG;
	for (int i = reg_alloc_idx; i < reg_alloc_idx + pac_alloc_idx; ++i)
		techs[i].reloc = PAC;
	for (int i = reg_alloc_idx + pac_alloc_idx; i < reg_alloc_idx + pac_alloc_idx + flx_alloc_idx; ++i)
		techs[i].reloc = FLX;
	for (int i = reg_alloc_idx + pac_alloc_idx + flx_alloc_idx; i < reg_alloc_idx + pac_alloc_idx + flx_alloc_idx + che_alloc_idx; ++i)
		techs[i].reloc = CHE;
	for (int i = reg_alloc_idx + pac_alloc_idx + flx_alloc_idx + che_alloc_idx; i < re_tech_num; ++i)
		techs[i].reloc = PAY;
	for (int i = re_tech_num; i < tech_num; ++i) {
		techs[i].reloc = BRK;
	}

	for (int i = 0; i < tech_num; ++i) {
		if (techs[i].loc == techs[i].reloc)
			techs[i].rescheduled = true;
		if (techs[i].isIdle()) {
			techs[i].loc = techs[i].reloc;
			techs[i].rescheduled = true;
		}
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

void System::debug(double latest_event_time) {
	// std::cout << "Before Time: " << latest_event_time << std::endl;
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
	// 	std::cout << "tech " << i << ": " << loc << std::endl;
	// }
	// printf("Idle:");
	for (int i = 0; i < tech_num; ++i) {
		// printf("%d ", techs[i].isIdle());
		if (!techs[i].isIdle()) {
			techs[i].setIdle(current_event->occur_time);
			accumulateBusyMinutes(techs[i].loc, techs[i].getCurrentBusyMinutes(), false);
			techs[i].setBusy(current_event->occur_time);
		}
		techs[i].resetCurrentBusyMinutes();
	}
	// printf("\n");
	// printf("utilized: %.3f %.3f %.3f %.3f\n", reg_busy_minutes, pac_busy_minutes, che_busy_minutes, pay_busy_minutes);
	reg_hourly_utility[(int)latest_event_time/60-1] += reg_busy_minutes / (60 * reg_num);
	pac_hourly_utility[(int)latest_event_time/60-1] += (pac_busy_minutes + shift_busy_minutes) / (60 * pac_num + shift_busy_minutes);
	che_hourly_utility[(int)latest_event_time/60-1] += (che_busy_minutes - shift_busy_minutes) / (60 * (che_num + flx_num) - shift_busy_minutes);
	pay_hourly_utility[(int)latest_event_time/60-1] += pay_busy_minutes / (60 * pay_num);
	resetBusyMinutes();

	// printf("Time %f: ", latest_event_time);
	// printf("%f %f %d\n", hourly_stay_minutes/hourly_prescription_num, hourly_stay_minutes, hourly_prescription_num);
	hourly_avg_stay_minutes[(int)latest_event_time/60-1] += (hourly_prescription_num == 0) ? 0.100 : hourly_stay_minutes / hourly_prescription_num;

	for (int i = 0; i < 9; ++i)
		printf("%f ", hourly_avg_stay_minutes[i]);
	printf("\n");

	// printf("Show limit: %d / %d\n", limit, hourly_prescription_num);
	hourly_limit[(int)latest_event_time/60-1] += (hourly_prescription_num == 0) ? 0 : (float) limit / hourly_prescription_num;
	// for (int i = 0; i < 9; ++i) {
	// 	printf("%f ", hourly_limit[i]);
	// }
	// printf("\n");

	hourly_stay_minutes = 0;
	hourly_prescription_num = 0;
	limit = 0;

	// printf("Time:%f\n", latest_event_time);
	// printf("Reg: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
	// 	reg_hourly_utility[0]/1,
	// 	reg_hourly_utility[1]/1,
	// 	reg_hourly_utility[2]/1,
	// 	reg_hourly_utility[3]/1,
	// 	reg_hourly_utility[4]/1,
	// 	reg_hourly_utility[5]/1,
	// 	reg_hourly_utility[6]/1,
	// 	reg_hourly_utility[7]/1,
	// 	reg_hourly_utility[8]/1);
	// printf("Pac: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
	// 	pac_hourly_utility[0]/1,
	// 	pac_hourly_utility[1]/1,
	// 	pac_hourly_utility[2]/1,
	// 	pac_hourly_utility[3]/1,
	// 	pac_hourly_utility[4]/1,
	// 	pac_hourly_utility[5]/1,
	// 	pac_hourly_utility[6]/1,
	// 	pac_hourly_utility[7]/1,
	// 	pac_hourly_utility[8]/1);
	// printf("Che: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
	// 	che_hourly_utility[0]/1,
	// 	che_hourly_utility[1]/1,
	// 	che_hourly_utility[2]/1,
	// 	che_hourly_utility[3]/1,
	// 	che_hourly_utility[4]/1,
	// 	che_hourly_utility[5]/1,
	// 	che_hourly_utility[6]/1,
	// 	che_hourly_utility[7]/1,
	// 	che_hourly_utility[8]/1);
	// printf("Pay: %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",
	// 	pay_hourly_utility[0]/1,
	// 	pay_hourly_utility[1]/1,
	// 	pay_hourly_utility[2]/1,
	// 	pay_hourly_utility[3]/1,
	// 	pay_hourly_utility[4]/1,
	// 	pay_hourly_utility[5]/1,
	// 	pay_hourly_utility[6]/1,
	// 	pay_hourly_utility[7]/1,
	// 	pay_hourly_utility[8]/1);
	// printf("===================\n");
}