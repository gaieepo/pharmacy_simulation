#include "mpi.h"
#include "System.h"
#include <cstdlib>
#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm>

bool calSchedule(std::vector<int> &combination, unsigned long dividend);
double calAvg(std::vector<double> &candTime);

int main(int argc, char *argv[]) {
    double total_service_minutes = 60 * 9;
    int tech_num = 11;
    int simulate_num = 20;

    double resTime = 1000;
    unsigned long resSrc = 0;
    double avg;
    int tag = 123;

    unsigned long total = 42806400;
    int myId, np;
    unsigned long index;
    unsigned long partition;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);
    MPI_Status status;
    MPI_Request request;

    std::vector<double> minTime(np, 0);
    std::vector<unsigned long> minSrc(np, 0);
    double allMinTime = 10000;
    unsigned long allMinSrc;

	std::vector<std::vector<int>> hour1;
	std::vector<std::vector<int>> hour2;
	std::vector<std::vector<int>> hour3;
	std::vector<std::vector<int>> hour4;
	std::vector<std::vector<int>> hour5;
	std::vector<std::vector<int>> hour6;
	std::vector<std::vector<int>> hour7;
	std::vector<std::vector<int>> hour8;
	std::vector<std::vector<int>> hour9;

    std::vector<int> temp;
    temp = {1, 6, 0, 2, 2};
    hour1.push_back(temp);
    temp = {1, 5, 0, 3, 2};
    hour1.push_back(temp);
    temp = {1, 4, 0, 4, 2};
    hour1.push_back(temp);
    temp = {2, 5, 0, 3, 1};
    hour1.push_back(temp);
    temp = {2, 4, 0, 4, 1};
    hour1.push_back(temp);
    temp = {2, 4, 0, 3, 2};
    hour1.push_back(temp);
    temp = {2, 3, 0, 4, 2};
    hour1.push_back(temp);

    temp = {2, 6, 0, 2, 1};
    hour2.push_back(temp);
    temp = {2, 5, 0, 3, 1};
    hour2.push_back(temp);
    temp = {2, 4, 0, 4, 1};
    hour2.push_back(temp);
    temp = {2, 6, 0, 1, 2};
    hour2.push_back(temp);
    temp = {2, 5, 0, 2, 2};
    hour2.push_back(temp);
    temp = {2, 4, 0, 3, 2};
    hour2.push_back(temp);
    temp = {2, 3, 0, 4, 2};
    hour2.push_back(temp);

    temp = {1, 4, 0, 3, 1};
    hour3.push_back(temp);
    temp = {1, 4, 0, 2, 2};
    hour3.push_back(temp);
    temp = {1, 3, 0, 3, 2};
    hour3.push_back(temp);
    temp = {2, 4, 0, 2, 1};
    hour3.push_back(temp);
    temp = {2, 3, 0, 3, 1};
    hour3.push_back(temp);
    temp = {2, 3, 0, 2, 2};
    hour3.push_back(temp);
    temp = {2, 2, 0, 3, 2};
    hour3.push_back(temp);

    temp = {1, 6, 0, 3, 1};
    hour4.push_back(temp);
    temp = {1, 5, 0, 4, 1};
    hour4.push_back(temp);
    temp = {1, 5, 0, 3, 2};
    hour4.push_back(temp);
    temp = {1, 4, 0, 4, 2};
    hour4.push_back(temp);
    temp = {2, 5, 0, 3, 1};
    hour4.push_back(temp);
    temp = {2, 4, 0, 4, 1};
    hour4.push_back(temp);
    temp = {2, 4, 0, 3, 2};
    hour4.push_back(temp);
    temp = {2, 3, 0, 4, 2};
    hour4.push_back(temp);

    temp = {1, 4, 0, 1, 1};
    hour5.push_back(temp);
    temp = {1, 3, 0, 2, 1};
    hour5.push_back(temp);
    temp = {1, 2, 0, 3, 1};
    hour5.push_back(temp);
    temp = {1, 2, 0, 2, 2};
    hour5.push_back(temp);
    temp = {1, 1, 0, 3, 2};
    hour5.push_back(temp);

    temp = {1, 4, 0, 1, 1};
    hour6.push_back(temp);
    temp = {1, 3, 0, 2, 1};
    hour6.push_back(temp);
    temp = {1, 2, 0, 3, 1};
    hour6.push_back(temp);
    temp = {1, 3, 0, 1, 2};
    hour6.push_back(temp);
    temp = {1, 2, 0, 2, 2};
    hour6.push_back(temp);
    temp = {1, 1, 0, 3, 2};
    hour6.push_back(temp);

    temp = {2, 6, 0, 1, 1};
    hour7.push_back(temp);
    temp = {2, 5, 0, 2, 1};
    hour7.push_back(temp);
    temp = {2, 4, 0, 3, 1};
    hour7.push_back(temp);
    temp = {2, 4, 0, 2, 2};
    hour7.push_back(temp);
    temp = {2, 3, 0, 3, 2};
    hour7.push_back(temp);

    temp = {1, 6, 0, 3, 1};
    hour8.push_back(temp);
    temp = {1, 5, 0, 4, 1};
    hour8.push_back(temp);
    temp = {1, 5, 0, 3, 2};
    hour8.push_back(temp);
    temp = {1, 4, 0, 4, 2};
    hour8.push_back(temp);
    temp = {2, 5, 0, 3, 1};
    hour8.push_back(temp);
    temp = {2, 4, 0, 4, 1};
    hour8.push_back(temp);
    temp = {2, 4, 0, 3, 2};
    hour8.push_back(temp);
    temp = {2, 3, 0, 4, 2};
    hour8.push_back(temp);

    temp = {1, 7, 0, 2, 1};
    hour9.push_back(temp);
    temp = {1, 6, 0, 3, 1};
    hour9.push_back(temp);
    temp = {1, 5, 0, 4, 1};
    hour9.push_back(temp);
    temp = {1, 6, 0, 2, 2};
    hour9.push_back(temp);
    temp = {1, 5, 0, 3, 2};
    hour9.push_back(temp);
    temp = {1, 4, 0, 4, 2};
    hour9.push_back(temp);
    temp = {2, 6, 0, 2, 1};
    hour9.push_back(temp);
    temp = {2, 5, 0, 3, 1};
    hour9.push_back(temp);
    temp = {2, 4, 0, 4, 1};
    hour9.push_back(temp);
    temp = {2, 6, 0, 1, 2};
    hour9.push_back(temp);
    temp = {2, 5, 0, 2, 2};
    hour9.push_back(temp);
    temp = {2, 4, 0, 3, 2};
    hour9.push_back(temp);
    temp = {2, 3, 0, 4, 2};
    hour9.push_back(temp);

    std::vector<int> combination;
    std::vector<double> candTime;
    partition = (total + (np / 2)) / np;

    System system(total_service_minutes, tech_num);
    for (index = 0; index <= partition; ++index) {
        unsigned long src = partition * myId + index;
        bool isValid = calSchedule(combination, src);

        if (!isValid) {
            break;
        }

        system.setTechAllocation(hour1[combination[8]][0], hour1[combination[8]][1], hour1[combination[8]][2], hour1[combination[8]][3], hour1[combination[8]][4]);
        system.setReschedule(hour2[combination[7]][0], hour2[combination[7]][1], hour2[combination[7]][2], hour2[combination[7]][3], hour2[combination[7]][4], 60);
        system.setReschedule(hour3[combination[6]][0], hour3[combination[6]][1], hour3[combination[6]][2], hour3[combination[6]][3], hour3[combination[6]][4], 120);
        system.setReschedule(hour4[combination[5]][0], hour4[combination[5]][1], hour4[combination[5]][2], hour4[combination[5]][3], hour4[combination[5]][4], 180);
        system.setReschedule(hour5[combination[4]][0], hour5[combination[4]][1], hour5[combination[4]][2], hour5[combination[4]][3], hour5[combination[4]][4], 240);
        system.setReschedule(hour6[combination[3]][0], hour6[combination[3]][1], hour6[combination[3]][2], hour6[combination[3]][3], hour6[combination[3]][4], 300);
        system.setReschedule(hour7[combination[2]][0], hour7[combination[2]][1], hour7[combination[2]][2], hour7[combination[2]][3], hour7[combination[2]][4], 360);
        system.setReschedule(hour8[combination[1]][0], hour8[combination[1]][1], hour8[combination[1]][2], hour8[combination[1]][3], hour8[combination[1]][4], 420);
        system.setReschedule(hour9[combination[0]][0], hour9[combination[0]][1], hour9[combination[0]][2], hour9[combination[0]][3], hour9[combination[0]][4], 480);
        for (int i = 0; i < 10; ++i) {
            system.simulate(simulate_num);
            candTime.push_back(system.getAvgStayMinutes());
        }

        avg = calAvg(candTime);

        if (avg < resTime) {
            resSrc = src;
            resTime = avg;
        }

        system.clearReschedule();
        candTime.clear();
        combination.clear();
    }

    if (myId == 0) {
        minTime[0] = resTime;
        minSrc[0] = resSrc;
        for (int i = 1; i < np; ++i) {
            MPI_Recv(&minTime[i], 1, MPI_DOUBLE, i, tag, MPI_COMM_WORLD, &status);
            MPI_Recv(&minSrc[i], 1, MPI_LONG, i, tag, MPI_COMM_WORLD, &status);
        }
    } else {
        MPI_Send(&resTime, 1, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
        MPI_Send(&resSrc, 1, MPI_LONG, 0, tag, MPI_COMM_WORLD);
    }
    printf("Process %d calculated %.2f for %ld\n", myId, resTime, resSrc);
    MPI_Finalize();
    return 0;
}

bool calSchedule(std::vector<int> &combination, unsigned long dividend) {
    assert(combination.size() == 0);
    unsigned long total = 42806400;
    std::vector<int> base{13, 8, 5, 6, 5, 8, 7, 7, 7};
    if (dividend >= total) return false;
    auto it = base.begin();
    while (dividend != 0) {
        combination.push_back(dividend % *it);
        dividend /= *it;
        ++it;
    }
    while (combination.size() < 9) {
        combination.push_back(0);
    }
    return true;
}

double calAvg(std::vector<double> &candTime) {
    auto max = std::max_element(candTime.begin(), candTime.end());
    auto min = std::min_element(candTime.begin(), candTime.end());
    double sum = 0;
    for (auto it = candTime.begin(); it != candTime.end(); ++it) {
        if (it != max && it != min) {
            sum += *it;
        }
    }
    return sum;
}