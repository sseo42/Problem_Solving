#include <iostream>
#include <algorithm>
#include <cstring>

using namespace std;

/**
 * Problem
 *        1
 *      2 0 4
 *        3
 * 
 *      위와 같은 2차원 DDR 보드가 있다.
 * 
 *      [1,4] 값으로 이루어진 수열이 주어질 때, 해당 값들을 밟을 수 있는 최소 비용을 구하자.
 *      (마지막에는 0이 주어지며, 수열의 길이는 10,000 을 넘지 않는다.)
 *      
 *      처음 양발은 0 에서 시작하며, 시작 이후 양발이 같은 곳에 있어서는 안된다.
 *      같은 곳을 다시 밟기 위해서는 1의 힘이 필요하며, 0 에서 이동하기 위해서는 2의 힘이 필요하다.
 *      이후 인접한(1-> 2/4) 곳으로 이동하는데는 3의 비용, 반대(1->4) 는 4의 비용이 필요하다.
 *      앵발을 동시에 움직일 수는 없다.
 * 
 * Ideas
 *      2(각 발) * 4(수열의 값) * 10,000(최대 수열길이) 로 구성된 cube 를 이용하자.
 * 
 *      2 2 2 2 2 2 같이 올수 있으니 0 을 포함해서 구성하자(4 -> 5).
 * 
 * Comments
 *      오늘 좀 힘들어서 집중이 제대로 안된다...
 * 
 *      비효율적인 부분이 있지만, 무난한것 같다.
 * 
 *      아이디어에서는 각 발을 배열의 한 줄로 생각했는데 각발의 위치를 담아야하니 4 * 4 가 맞다.
 * 
 *      제출했을 때 MAX_SEQUENCE 잘못 넣어서 메모리 이상한데 건드렸다.
 */

#define MAX_STEP_VALUE 4
#define MAX_SEQUENCE 100000
#define IMPOSSIBLE 1000000

int dp_cube[MAX_SEQUENCE + 1][MAX_STEP_VALUE + 1][MAX_STEP_VALUE + 1];

int step_cost[MAX_STEP_VALUE + 1][MAX_STEP_VALUE + 1] = {
                                                         1, 2, 2, 2, 2,
                                                         3, 1, 3, 4, 3,
                                                         3, 3, 1, 3, 4,
                                                         3, 4, 3, 1, 3,
                                                         3, 3, 4, 3, 1};


void step_up_left(int step_seq, int step_val)
{
    int min_val = IMPOSSIBLE;
    
    for (int i = 0; i <= MAX_STEP_VALUE; ++i) {
        for (int j = 0; j <= MAX_STEP_VALUE; ++j) {
            if (dp_cube[step_seq - 1][i][j] == IMPOSSIBLE || step_val == j) {
                continue;
            }
            dp_cube[step_seq][step_val][j] = min(dp_cube[step_seq][step_val][j], dp_cube[step_seq - 1][i][j] + step_cost[i][step_val]);
        }
    }
}

void step_up_right(int step_seq, int step_val)
{
    int min_val = IMPOSSIBLE;

    for (int i = 0; i <= MAX_STEP_VALUE; ++i) {
        for (int j = 0; j <= MAX_STEP_VALUE; ++j) {
            if (dp_cube[step_seq - 1][i][j] == IMPOSSIBLE || step_val == i) {
                continue;
            }
            dp_cube[step_seq][i][step_val] = min(dp_cube[step_seq][i][step_val], dp_cube[step_seq - 1][i][j] + step_cost[j][step_val]);
        }
    }
}

int main()
{
    int step_seq = 0;
    int step_val;
    int ans = IMPOSSIBLE;

    scanf("%d", &step_val);
    if (step_val == 0) {
        printf("0\n");
        return 0;
    }
    for (int i = 0; i <= MAX_SEQUENCE; ++i) {
        for (int j = 0; j <= MAX_STEP_VALUE; ++j) {
            for (int k = 0; k <= MAX_STEP_VALUE; ++k) {
                dp_cube[i][j][k] = IMPOSSIBLE;
            }
        }
    }
    dp_cube[0][0][0] = 0;
    while (step_val != 0) {
        ++step_seq;
        step_up_left(step_seq, step_val);
        step_up_right(step_seq, step_val);

        scanf("%d", &step_val);
    }
    for (int i = 0; i <= MAX_STEP_VALUE; ++i) {
        for (int j = 0; j <= MAX_STEP_VALUE; ++j) {
            ans = min(ans, dp_cube[step_seq][i][j]);
       }
    }
    if (ans == IMPOSSIBLE) {
        exit(1);
    }
    printf("%d\n", ans);
}