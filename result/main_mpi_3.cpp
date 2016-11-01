#include "mpi.h"
#include "System.h"
#include <cstdlib>
#include <vector>
#include <cassert>
#include <iostream>

bool calSchedule(std::vector<int> &combination, long int dividend);

int main(int argc, char *argv[]) {
    double total_service_minutes = 60 * 9;
    int tech_num = 11;
    int simulate_num = 3;

    double resTime = 1000;
    long int resSrc = 0;
    int tag = 123;

    long int total = 39321600;
    int myId, np;
    long int index;
    long int partition;

    int break_schedule[9][2] = {
        {11, 4},
        {11, 4},
        {9, 3},
        {11, 4},
        {7, 3},
        {7, 3},
        {11, 4},
        {11, 4},
        {11, 4}
    };

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    MPI_Comm_rank(MPI_COMM_WORLD, &myId);
    MPI_Status status;
    MPI_Request request;

    std::vector<double> minTime(np, 0);
    std::vector<long> minSrc(np, 0);
    double allMinTime = 10000;
    long int allMinSrc;

	std::vector<std::vector<int>> hour1;
	std::vector<std::vector<int>> hour2;
	std::vector<std::vector<int>> hour3;
	std::vector<std::vector<int>> hour4;
	std::vector<std::vector<int>> hour5;
	std::vector<std::vector<int>> hour6;
	std::vector<std::vector<int>> hour7;
	std::vector<std::vector<int>> hour8;
	std::vector<std::vector<int>> hour9;

	for (int i = 0; i < 9; ++i) {
		for (int reg  = 2; reg != 3; ++reg) {
			for (int pay = 1; pay != 3; ++pay) {
				for (int flxche = 1; flxche != break_schedule[i][1] + 1; ++flxche) {
					for (int flx = 0; flx <= flxche && flx < 1; ++flx) {
						int che = flxche - flx;
						int pac = break_schedule[i][0] - reg - pay - flx - che;
						if (pac > 0) {
							std::vector<int> temp{reg, pac, flx, che, pay};
							switch (i) {
								case 0:
									hour1.push_back(temp);
									break;
								case 1:
									hour2.push_back(temp);
									break;
								case 2:
									hour3.push_back(temp);
									break;
								case 3:
									hour4.push_back(temp);
									break;
								case 4:
									hour5.push_back(temp);
									break;
								case 5:
									hour6.push_back(temp);
									break;
								case 6:
									hour7.push_back(temp);
									break;
								case 7:
									hour8.push_back(temp);
									break;
								case 8:
									hour9.push_back(temp);
									break;
								default:
									break;
							}
						}
					}
				}
			}
		}
	}

    std::vector<int> combination;
    partition = (total + (np / 2)) / np;

    System system(total_service_minutes, tech_num);
    for (index = 0; index < partition; ++index) {
        bool isValid = calSchedule(combination, partition * myId + index);
        if (!isValid) {
            MPI_Finalize();
            exit(1);
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
        system.simulate(simulate_num);
        if (system.getAvgStayMinutes() < resTime) {
            resSrc = partition * myId + index;
            resTime = system.getAvgStayMinutes();
        }

        system.clearReschedule();
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
    if (myId == 0) {
        for (int i = 0; i < np; ++i) {
            if (minTime[i] < allMinTime) {
                allMinTime = minTime[i];
                allMinSrc = minSrc[i];
            }
        }
        printf("Result: %f using %ld\n", allMinTime, allMinSrc);
    }
    MPI_Finalize();
    return 0;
}

bool calSchedule(std::vector<int> &combination, long int dividend) {
    assert(combination.size() == 0);
    long int total = 39321600;
    std::vector<int> base{8, 8, 8, 5, 5, 8, 6, 8, 8};
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
