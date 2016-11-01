#include <cstdlib>
#include <vector>
#include <cassert>
#include <iostream>

bool calSchedule(std::vector<int> &combination, long int dividend);

int main() {
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

    // 1 8 0 1 1
    // 1 7 0 2 1
    // 1 6 0 3 1
    // 1 5 0 4 1
    // 1 7 0 1 2
    // 1 6 0 2 2
    // 1 5 0 3 2
    // 1 4 0 4 2
    // 2 7 0 1 1
    // 2 6 0 2 1
    // 2 5 0 3 1
    // 2 4 0 4 1
    // 2 6 0 1 2
    // 2 5 0 2 2
    // 2 4 0 3 2
    // 2 3 0 4 2
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

 //    long total = 39321600;
 //    int myId, np = 10;
 //    long index;
 //    long partition;

 //    partition = (total + (np / 2)) / np;
    std::vector<int> combination;

 //    for (myId = 0; myId < 10; ++myId) {
	//     for (index = 0; index < 10; ++index) {
	//     	printf("myId:%d, index:%ld, partition:%ld, src:%ld ", myId, index, partition, partition * myId + index);
	//         bool isValid = calSchedule(combination, partition * myId + index);
	//         if (!isValid) {
	//             printf("INVALID\n");
	//         } else {
	//         	printf("VALID\n");	        	
	//         }
	//         combination.clear();
	//     }
	//     printf("\n");
	// }

	calSchedule(combination, 8671869);
	for (int i : combination)
		printf("%d ", i);
	printf("\n");

    for (int i : hour1[combination[8]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour2[combination[7]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour3[combination[6]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour4[combination[5]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour5[combination[4]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour6[combination[3]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour7[combination[2]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour8[combination[1]])
        printf("%d ", i);
    printf("\n");
    for (int i : hour9[combination[0]])
        printf("%d ", i);
    printf("\n");
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
