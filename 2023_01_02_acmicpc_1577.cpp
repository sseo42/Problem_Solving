#include <iostream>

using namespace std;

/**
 * Problem
 *      N x M 의 격자가 주어진다.
 * 
 *      1 <= N, M <= 100
 * 
 *      격자를 연결하는 간선이 끊겨 있을 수 있으며, 이는 K 개 존재한다.
 * 
 *      0 <= K <= 50
 * 
 *      이어 a, b, c, d 가 주어지며, 이는 (a, b) - (c, d) 간선이 끊겨 있음을 의미한다.
 *      (a, b) (c, d) 점은 항상 인접하여 주어진다.
 * 
 *      (0, 0) 부터 (N, M) 까지 이동하는 최단 거리(N + M) 의 경우의 수를 구하자(2^63 - 1 보다 작거나 같다는 게 보장된다) 
 * 
 * Ideas
 *      한 점에서 경우의 수를 구하기 위해서는 왼쪽과 아래 두 포인트의 경우의 수만 알면된다.
 *      이는 끊긴 간선이 계산에 미치는 영향이 오직 한 점에만 국한됨을 의미하고, 해당 점은 좌측 혹은 아래 중 끊긴 곳만 알면된다.
 * 
 * Comments
 *      비슷한 유형의 문제에서 끝지점을 1 설정하는 것을 아무생각 없이 써서 시간이 허비됬다.
 *      간선이 끊겨 있다면 그 뒤에는 0 임을 인지하지 못했다 따라서 초기화가 잘못되어 엉뚱한 겂이 나왔다.
 *      전형적으로 사용하는 초기값 / 로직의 정확한 이유를 알아야한다. 얼렁뚱땅 넘어가지 말자.
 */

#define MAX_BOARD_SIZE 100

long long board[MAX_BOARD_SIZE + 1][MAX_BOARD_SIZE + 1];

bool left_broken[MAX_BOARD_SIZE + 1][MAX_BOARD_SIZE + 1];
bool down_broken[MAX_BOARD_SIZE + 1][MAX_BOARD_SIZE + 1];

int main(void)
{
    int n, m, k;
    int a, b, c, d;

    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < k; i++) {
        scanf("%d %d %d %d", &a, &b, &c, &d);
        if (a > c) {
            left_broken[a][b] = true;
        } else if (c > a) {
            left_broken[c][d] = true;
        } else if (b > d) {
            down_broken[a][b] = true;
        } else {
            down_broken[c][d] = true;
        }
    }

    for (int i = 0; i <= n; i++) {
        if (left_broken[i][0]) {
            break;
        }
        board[i][0] = 1;
    }

    for (int j = 0; j <= m; j++) {
        if (down_broken[0][j]) {
            break;
        }
        board[0][j] = 1;
    }

    for (int i = 1; i <= n; i ++) {
        for (int j = 1; j <= m; j++) {
            if (!left_broken[i][j]) {
                board[i][j] += board[i - 1][j];
            }
            if (!down_broken[i][j]) {
                board[i][j] += board[i][j - 1];
            }
        }
    }

    printf("%lld\n", board[n][m]);
}