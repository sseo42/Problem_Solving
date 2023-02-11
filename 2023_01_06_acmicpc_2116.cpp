#include <iostream>

using namespace std;

/**
 * Problem
 *      주사위의 6면에 대한 각 value 가 주어진다.
 *      주사위를 쌓아올려 한쪽 옆면의 값이 MAX 되게 하고 그 값을 출력하자
 * 
 *      단 쌓아올릴 떄 인접하는 면은 같은 값이여야한다.
 * 
 *      n <= 10,000     : 주사위의 개수
 * 
 *        a
 *      b c d e
 *        f
 * 
 *      의 주사위의 값들이 a,b,c,d,e,f 순으로 n 줄에 이어 주어진다.
 * 
 * Ideas
 *      이런, 주사위의 쌓기 순서가 정해져있다!
 * 
 *      첫번째 주사위를 놓는 순간 나머지가 정해진다 따라서 6번 돌면 끝.
 * 
 *      쉽게 추적하기 위해서 a, b, c, d, e, f 값을 담을 dice_value 배열을 두자.
 * 
 *      또한 dice_value 를 통해 다음 위치를 추적하기 용이하게 dice_value_idx 를 두자.
 *      이 배열에서의 위치에 해당하는 값의 dice_value에서의 index를 나타낸다.
 * 
 * Comments
 *      불필요한 연산을 더해서 점화식이 꼬였다, 이어지는 대상을 명확히 기억해야된다.
 * 
 */

#define MAX_DICE_AMOUNT 10000
#define MAX_FACE 6

int n;
int dice_value[MAX_DICE_AMOUNT][MAX_FACE + 1];
int dice_value_idx[MAX_DICE_AMOUNT][MAX_FACE + 1];
int dice_max_side_value[MAX_DICE_AMOUNT][MAX_FACE + 1];

void init_dice_max_side_value(void)
{
    for (int i = 0; i < n; ++i) {
        int x = max(dice_value[i][1], dice_value[i][6]);
        int y = max(dice_value[i][2], dice_value[i][4]);
        int z = max(dice_value[i][3], dice_value[i][5]);

        dice_max_side_value[i][1] = max(y, z);
        dice_max_side_value[i][6] = dice_max_side_value[i][1];
        
        dice_max_side_value[i][2] = max(x, z);
        dice_max_side_value[i][4] = dice_max_side_value[i][2];

        dice_max_side_value[i][3] = max(x, y);
        dice_max_side_value[i][5] = dice_max_side_value[i][3];
    }
}

void init_dice_value_idx(void)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j <= MAX_FACE; ++j) {
            dice_value_idx[i][dice_value[i][j]] = j;
        }
    }
}

int get_opponent_idx(int idx)
{
    switch (idx) {
        case 1:
            return 6;
        case 6:
            return 1;
        case 2:
            return 4;
        case 4:
            return 2;
        case 3:
            return 5;
        case 5:
            return 3;
        default:
            return 0;
    };
}


int trace_next_dice_idx(int floor, int idx)
{
    if (floor >= n - 1) {
        return 0;
    }
    return dice_value_idx[floor + 1][dice_value[floor][get_opponent_idx(idx)]];
}

int get_max_sum(void)
{
    int res = 0;
    
    for (int i = 1; i <= MAX_FACE; ++i) {
        int total_value = 0;
        int floor = 0;
        int idx = i;

        while (idx != 0) {
            total_value += dice_max_side_value[floor][idx];
            idx = trace_next_dice_idx(floor, idx);
            floor++;
        }
        if (total_value > res) {
            res = total_value;
        }
    }
    return res;
}

int main(void)
{
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d %d %d %d %d %d",
              &(dice_value[i][1]),
              &(dice_value[i][2]),
              &(dice_value[i][3]),
              &(dice_value[i][4]),
              &(dice_value[i][5]),
              &(dice_value[i][6]));
    }
    init_dice_value_idx();
    init_dice_max_side_value();

    printf("%d\n", get_max_sum());
}