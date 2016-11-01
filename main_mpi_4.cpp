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
    int simulate_num = 1;

    double resTime = 1000;
    long int resSrc = 0;
    int tag = 123;

    long int total = 39321600;
    int myId, np;
    long int index;
    long int partition;

    double allMinTime = 0;
    long int allMinSrc;

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

    if (myId != 0) {
        std::vector<int> combination;
        partition = (total + (np / 2)) / np;

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
        System system(total_service_minutes, tech_num);
        for (index = 0; index < 10; ++index) {
            bool isValid = calSchedule(combination, partition * myId + index);
            if (!isValid) {
                MPI_Finalize();
                exit(1);
            }

            // system.setTechAllocation(2, 4, 0, 4, 1);
            // system.setReschedule(2, 4, 0, 4, 1, 60); // 7, 4
            // system.setReschedule(2, 3, 0, 3, 1, 120); // 6, 3
            // system.setReschedule(2, 4, 0, 4, 1, 180); // 7, 4
            // system.setReschedule(1, 2, 0, 3, 1, 240); // 4, 3
            // system.setReschedule(1, 2, 0, 3, 1, 300); // 4, 3
            // system.setReschedule(2, 4, 0, 3, 1, 360); // 7, 3
            // system.setReschedule(2, 4, 0, 4, 1, 420); // 7, 4
            // system.setReschedule(2, 4, 0, 4, 1, 480); // 7, 4

            system.setTechAllocation(hour1[combination[8]][0], hour1[combination[8]][1], hour1[combination[8]][2], hour1[combination[8]][3], hour1[combination[8]][4]);
            system.setReschedule(hour2[combination[7]][0], hour2[combination[7]][1], hour2[combination[7]][2], hour2[combination[7]][3], hour2[combination[7]][4], 60);
            system.setReschedule(hour3[combination[6]][0], hour3[combination[6]][1], hour3[combination[6]][2], hour3[combination[6]][3], hour3[combination[6]][4], 120);
            system.setReschedule(hour4[combination[5]][0], hour4[combination[5]][1], hour4[combination[5]][2], hour4[combination[5]][3], hour4[combination[5]][4], 180);
            system.setReschedule(hour5[combination[4]][0], hour5[combination[4]][1], hour5[combination[4]][2], hour5[combination[4]][3], hour5[combination[4]][4], 240);
            system.setReschedule(hour6[combination[3]][0], hour6[combination[3]][1], hour6[combination[3]][2], hour6[combination[3]][3], hour6[combination[3]][4], 300);
            system.setReschedule(hour7[combination[2]][0], hour7[combination[2]][1], hour7[combination[2]][2], hour7[combination[2]][3], hour7[combination[2]][4], 360);
            system.setReschedule(hour8[combination[1]][0], hour8[combination[1]][1], hour8[combination[1]][2], hour8[combination[1]][3], hour8[combination[1]][4], 420);
            system.setReschedule(hour9[combination[0]][0], hour9[combination[0]][1], hour9[combination[0]][2], hour9[combination[0]][3], hour9[combination[0]][4], 480);            system.simulate(simulate_num);
            if (system.getAvgStayMinutes() < resTime) {
                resSrc = partition * myId + index;
                resTime = system.getAvgStayMinutes();
            }

            system.clearReschedule();
            combination.clear();
        }
    }

    printf("Process %d calculated %.2f for %ld, %f, %ld\n", myId, resTime, resSrc, allMinTime, allMinSrc);

    // MPI_Status status;
    // MPI_Request request;

    MPI_Reduce(&resTime, &allMinTime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    if (myId == 0) {
        printf("Result: %f using %ld\n", allMinTime, allMinSrc);
    }

    MPI_Finalize();
    return 0;
}