#include <stdio.h>

/**
 * Problem
 *      2 <= N <= 1000 집의 갯수와
 * 
 *      N 줄에 걸쳐 집에다가 R, G, B 칠하는 cost 가 주어진다.
 *      인접한 집들은 서로 다른 색을 가져야한다.(집들은 원형으로 놓여있다)
 * 
 *      최소 비용을 구하자.
 * 
 * Ideas
 *      첫줄에서 하나씩 강제 해서 총 3가지 경우로 돌리고 마지막에서 같은 색은 쳐낸 6가지 경우로 비교하는 것만 생각난다...
 * 
 *      
 */

#define MAX_HOUSE 1000
#define IMPOSSIBLE 1000000

typedef enum E_COLOR
{
    RED = 0,
    BLUE,
    GREEN,
    MAX_COLOR
} COLOR;

int n;

int input_board[MAX_HOUSE][MAX_COLOR];
int cost[MAX_HOUSE][MAX_COLOR];

int min(int a, int b)
{
    if (a < b) {
        return a;
    }
    return b;
}

int get_min_val(COLOR start)
{
    for (COLOR c = RED; c != MAX_COLOR; c++) {
        if (c == start) {
            cost[0][c] = input_board[0][c];
        } else {
            cost[0][c] = IMPOSSIBLE;
        }
    }

    for (int i = 1; i < n; i++) {
        for (COLOR c = RED; c != MAX_COLOR; c++) {
            cost[i][c] = min(cost[i - 1][(c + 1) % MAX_COLOR], cost[i - 1][(c + 2) % MAX_COLOR]) + input_board[i][c];
        }
    }
    return min(cost[n - 1][(start + 1) % MAX_COLOR], cost[n - 1][(start + 2) % MAX_COLOR]);
}

int main(void)
{    
    int answer = IMPOSSIBLE;

    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d %d %d", &(input_board[i][RED]), &(input_board[i][BLUE]), &(input_board[i][GREEN]));
    }

    for (COLOR c = RED; c != MAX_COLOR; c++) {
        int local_min = get_min_val(c);

        if (answer > local_min) {
            answer = local_min;
        }
    }
    printf("%d\n", answer);
}