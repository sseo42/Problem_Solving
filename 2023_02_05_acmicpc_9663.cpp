#include <iostream>

using namespace std;

/**
 * Problem
 *      N-queen 문제.
 *      n x n 체스보드에서 n 개의 퀸을 놓을 수 있는 경우의 수를 구하자.
 *      n[1, 15].
 * 
 * Ideas
 *      15! 은 1500973039 이다. 중간 중간 값이 더 작은 것을 고려하면 10초 안에 들어올수 있다.
 *      백트래킹으로 bit masking 해서 풀자.
 * 
 * Comments
 *      전개할 때 대상을 잘 특정하는게 좋다. 특이사항 없음
 */

int n;

int place_queen(int order, int straight_bit, int left_crossing_bit, int right_crossing_bit)
{
    int current_bit_mask;
    int current_left_crossing_bit;
    int current_right_crossing_bit;
    int cnt = 0;

    if (order < 1) {
        return 1;
    }
    current_left_crossing_bit = (left_crossing_bit << 1);
    current_right_crossing_bit = (right_crossing_bit >> 1);
    current_bit_mask = straight_bit | current_left_crossing_bit | current_right_crossing_bit;
    for (int i = 1; i <= n; ++i) {
        if ((current_bit_mask & (1<<i)) == 0) {
            cnt += place_queen(order - 1, straight_bit | (1<<i), current_left_crossing_bit | (1<<i), current_right_crossing_bit | (1<<i));
        }
    }
    return cnt;
}


int main()
{
    scanf("%d", &n);
    printf("%d\n", place_queen(n, 0, 0, 0));
}