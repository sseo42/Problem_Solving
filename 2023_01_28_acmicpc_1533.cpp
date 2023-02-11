#include <iostream>
#include <cstring>

using namespace std;

/**
 * Problem
 *      행렬의 크기 n[1,10] 이 주어지고 시작노드 s, 끝 노드 e, 소요시간 t[1, 1000000000] 가주어진다.
 *      이어 행렬의 간선 cost 정보가 n x n 꼴로 주어진다(0 인 경우 간선이 없는 것이다).
 * 
 *      s에서부터 e 까지 t 가 걸리는 경로의 가지수를 1,000,003 으로 나누어 출력하자.
 * 
 * Ideas
 *      DP 로 풀수 있다면 t 를 축삼아서는 안된다.
 *      만약 주어진 그래프에 사이클이 없다면?
 * 
 *      아니 각 노드마다 다른 노드를 중복방문하지 않고 돌아오는 경우를 구한다면?
 *      흠....별다른 조치 없이는 결국 다 돌아봐야한다.... 
 * 
 *      힌트에서 분할정복을 이용한 거듭제곱이라...? 행렬에 이용하는게 있다..?
 * 
 *      시간별로 가능한 경우를 행렬화한다면 처음 입력으로 t=(1,2,3,4,5) 다섯개를 얻을 수 있다.
 * 
 *      2: 2 / 1+1
 *      3: 3 / 2+1 / 1+1+1
 *      4: 4 / 3+1 / 2+2 / 2+1+1 / 1+1+1+1
 *      5: 5 / 4+1 / 3+2(com) / 2+2+1 / 2+1+1+1 / 1+1+1+1+1
 *      6: 5+1 / 4+2(com) / 3+3(com) / 2+2+2(com) / 2+1+1+1+1 / 1+1..6
 *      7: 5+2(com) / 4+3(com) / 3+3+1 / 3+2+2(com) / 3+1+1+1+1 / 2+2+2+1 / 2+2+1+1+1 / 2+1+1+1+1+1 / 1+..7
 *      8: 5+3(com) / 4+4(com) / 3+3+2(com) / 3+2+1+2(com) / 3+1+1+1+1+1 /  2+2+2+2(com) / 2+2+1..4 / 2+2..6 / 1+..8
 * 
 *      2^n 을 다구한다면 조합하여 1000000000 의 가짓수를 구할수 있다....?
 *      정확한 시간에 대해 거듭제곱을 하게되면 포함되지 않는 경우가 생긴다.....
 * 
 *      그럼 1,2,3,4,5 의 lcm 으로 한다면?
 *      -> 60, 60의 곱수는 제곱연산으로 모든 경우 표현이 가능하다.
 * 
 *      따라서 t < 60 인 경우에 대해서 세부적으로 알면된다.
 * 
 *      주어진 행렬을 1~60회 모두 해봐서 60 만족하는 값들 구해다 더해서 t <= 60 의 모든 경우를 쟁여놓자. 
 * 
 * ----------------------------------------------------------------------------------------------------------------------
 *      lcm 써도 걸국 해당 지점에서 끊기는 것은 변함이 없다.
 * 
 *      다른 사람의 풀이를 참조한 결과 정점 분할을 통해 가중치 없는 간선을 만드는 것이 중요하다...!
 * 
 *      012     010100
 *      201     001010
 *      120     100001
 *              001000
 *              100000
 *              010000
 * 
 * Comments
 *      문제를 단순화하는게 중요하다.
 *      단순한 방법으로 전환할 수 있는 방법을 생각하는데 초점을 맞추자.
 *      변수를 돌릴 때 사용할 곳에 초점을 맞추자.
 */

#define MAX_N 10
#define MAX_EXTRA_PER_N 4
#define MAX_POW2 30
#define REMAINDER_BIAS 1000003

long long board[MAX_N * (1 + MAX_EXTRA_PER_N)][MAX_N * (1 + MAX_EXTRA_PER_N)];
long long pow2board[MAX_POW2 + 1][MAX_N * (1 + MAX_EXTRA_PER_N)][MAX_N * (1 + MAX_EXTRA_PER_N)];

int fill_pow2board(int t)
{
    static long long tmp_board[MAX_N * (1 + MAX_EXTRA_PER_N)][MAX_N * (1 + MAX_EXTRA_PER_N)];
    int pow2_cnt = 1;

    memcpy(pow2board[0], board, sizeof(long long) * (MAX_N * (1 + MAX_EXTRA_PER_N)) * (MAX_N * (1 + MAX_EXTRA_PER_N))); 
    while ((1<<pow2_cnt) <= t) {
        memcpy(tmp_board, pow2board[pow2_cnt - 1], sizeof(long long) * (MAX_N * (1 + MAX_EXTRA_PER_N)) * (MAX_N * (1 + MAX_EXTRA_PER_N))); 
        for (int i = 0; i < (MAX_N * (1 + MAX_EXTRA_PER_N)); ++i) {
            for (int j = 0; j < (MAX_N * (1 + MAX_EXTRA_PER_N)); ++j) {
                for (int k = 0; k < (MAX_N * (1 + MAX_EXTRA_PER_N)); ++k) {
                    pow2board[pow2_cnt][i][j] += (tmp_board[i][k] * tmp_board[k][j]) % REMAINDER_BIAS;
                }
                pow2board[pow2_cnt][i][j] %= REMAINDER_BIAS;
            }
        }
        ++pow2_cnt;
    }
    return pow2_cnt;
}

int main()
{
    
    int n, start_node, end_node, t;
    char buf[MAX_N + 1];
    long long ans_board[MAX_N * (1 + MAX_EXTRA_PER_N)][MAX_N * (1 + MAX_EXTRA_PER_N)];
    long long tmp_board[MAX_N * (1 + MAX_EXTRA_PER_N)][MAX_N * (1 + MAX_EXTRA_PER_N)];

    scanf("%d %d %d %d", &n, &start_node, &end_node, &t);
    --start_node;
    --end_node;
    for (int i = 0; i < n; ++i) {
        scanf("%s", buf);
        board[i][MAX_N + i * MAX_EXTRA_PER_N] = 1;
        board[MAX_N + i * MAX_EXTRA_PER_N][MAX_N + i * MAX_EXTRA_PER_N + 1] = 1;
        board[MAX_N + i * MAX_EXTRA_PER_N + 1][MAX_N + i * MAX_EXTRA_PER_N + 2] = 1;
        board[MAX_N + i * MAX_EXTRA_PER_N + 2][MAX_N + i * MAX_EXTRA_PER_N + 3] = 1;

        for (int j = 0; j < n; ++j) {
            int val = buf[j] - '0';

            if (val > 1) {
                board[i][j] = 0;
                board[MAX_N + i * MAX_EXTRA_PER_N + val - 2][j] = 1;
            } else {
                board[i][j] = val;
            }
        }
    }
    
    int pow2_cnt = fill_pow2board(t);

    for (int i = 0; i < MAX_N * (1 + MAX_EXTRA_PER_N); ++i) {
        ans_board[i][i] = 1;
    }
    for (int i = pow2_cnt - 1; i >= 0; --i) {
        if (t & (1<<i)) {
            memcpy(tmp_board, ans_board, sizeof(long long) * (MAX_N * (1 + MAX_EXTRA_PER_N)) * (MAX_N * (1 + MAX_EXTRA_PER_N)));
            memset(ans_board, 0, sizeof(long long) * (MAX_N * (1 + MAX_EXTRA_PER_N)) * (MAX_N * (1 + MAX_EXTRA_PER_N)));
            for (int j = 0; j < MAX_N * (1 + MAX_EXTRA_PER_N); ++j) {
                for (int k = 0; k < MAX_N * (1 + MAX_EXTRA_PER_N); ++k) {
                    for (int l = 0; l < MAX_N * (1 + MAX_EXTRA_PER_N); ++l) {
                        ans_board[j][k] += (tmp_board[j][l] * pow2board[i][l][k]) % REMAINDER_BIAS;
                    }
                    ans_board[j][k] %= REMAINDER_BIAS;
                }
            }
        }
    }
    printf("%lld\n", ans_board[start_node][end_node] % REMAINDER_BIAS);
}