#include <iostream>
#include <utility>
#include <cstring>
#include <stack>

using namespace std;

/**
 * Problem
 *      n[5,1000] 크기의 배열이 주어진다.
 * 
 *      {0,0} 과 {n-1,n-1} 에 1대씩 총 2대의 경찰차가 존재하고 경찰차는 배열을 따라 상하좌우로 움직을 수 있다.
 *      이어 사건의 개수 w[1,1000] 이 주어지고, 이어 w 줄에 걸쳐 사건이 발생한 좌표를 준다. 사건 간의 위치가 동일할 수 있다.
 * 
 *      사건들은 주어진 순서대로 해결되야하며, 경찰차가 사건에 도달하면 해결된다. 경찰차의 이동거리를 최소화하도록 사건을 분배하고
 * 
 *      총 이동거리룰 출력, 이후 각 사건을 배정한 경찰차 번호(1 or 2)를 출력하자(1: {0,0} 2:{n-1,n-1})
 * 
 * Ideas
 *      경찰차가 단 2대이므로 비트마스킹을 이용한 dp 를 생각할 수 있지만 사건의 개수가 1000 이하이므로 시간이 문제가 된다.
 *      또한, 한번 이동하고 나면 그이전의 좌표 정보가 필요없으므로 부적절하다.
 *      경찰차 각 각의 좌표를 축삼아 2차원 배열을 구성한다면, 이는 사건의 개수인 1000 이하로 표현가능하다.
 *      사건은 순서대로 해결되야하므로 배열에서 min_cost 를 구하기 위해서는 자신보다 이전 사건들만 포함하는 원소 중,
 *      자신의 좌표에서 {-1, -1} 한 지점과 row 혹은 col 이 동일한 놈들만 보면된다.
 * 
 * Comments
 *      실제 구현을 하는데 시간을 당기면 좋겠다.
 * 
 *      이문제를 풀기 위해서는 dp 에서 어느 부분들을 참조해야하는지 알아야한다. 이전 케이스를 참조해야된다는 것은 맞지만
 *      하나라도 빠트려서는 안되기에 이전 상대방이 움직인 경우에만 스스로의 0~현재 탐색을 해야한다.
 * 
 *      또한 누가 움직였는지를 알아야하기에 어느 경우에서 왔는지를 추적할수 있는 정보를 담을 곳이 필요하다.
 * 
 *      배열을 다룰 때 기준이 되는 value 가 어딘지 인식하고 있는편이 좋다, 문제 마다 네이밍을 생각하기에는 시간이 아까우니
 *      주어진 symbol 에 맞춰 추상화해 표현하는 편이 좋다.
 */

#define MAX_CASE 1000
#define IMPOSSIBLE 100000000

int n, w;
int min_cost[MAX_CASE + 1][MAX_CASE + 1];
pair<int, int> path_trace[MAX_CASE + 1][MAX_CASE + 1];
pair<int, int> index_to_case_location[MAX_CASE + 1];

void calculate_min_cost(int r, int c)
{
    int r_from, c_from, local_min;

    if (r < c) {
        r_from = r;
        if (c - r == 1) {
            c_from = 0;
            local_min = min_cost[r][0] + (n - 1 - index_to_case_location[c].first) + (n - 1 - index_to_case_location[c].second);
            for (int i = 1; i < c; ++i) {
                int candidate = min_cost[r][i] + abs(index_to_case_location[c].first - index_to_case_location[i].first)
                                               + abs(index_to_case_location[c].second - index_to_case_location[i].second);
                if (candidate < local_min) {
                    c_from = i;
                    local_min = candidate;
                }
            }
        } else {
            c_from = c - 1;
            local_min = min_cost[r][c - 1] + abs(index_to_case_location[c].first - index_to_case_location[c - 1].first)
                                           + abs(index_to_case_location[c].second - index_to_case_location[c - 1].second);
        }
    } else if (r > c) {
        c_from = c;
        if (r - c == 1) {
            r_from = 0;
            local_min = min_cost[0][c] + index_to_case_location[r].first + index_to_case_location[r].second;
            for (int i = 0; i < r; ++i) {
                int candidate = min_cost[i][c] + abs(index_to_case_location[r].first - index_to_case_location[i].first)
                                               + abs(index_to_case_location[r].second - index_to_case_location[i].second);
                if (candidate < local_min) {
                    r_from = i;
                    local_min = candidate;
                }
            }
        } else {
            r_from = r - 1;
            local_min = min_cost[r - 1][c] + abs(index_to_case_location[r].first - index_to_case_location[r - 1].first)
                                           + abs(index_to_case_location[r].second - index_to_case_location[r - 1].second);
        }
    } else {
        exit(1);
    }
    min_cost[r][c] = local_min;
    path_trace[r][c] = make_pair(r_from, c_from);
}

void load_case_to_index()
{
    int row, col;

    for (int i = 1; i <= w; ++i) {
        scanf("%d %d", &row, &col);
        index_to_case_location[i] = make_pair(row - 1, col - 1);
    }
}

void print_trace(int row, int col)
{
    int tmp;
    stack<int> ans;

    while (row > 0 || col > 0) {
        if (row > col) {
            ans.push(1);
            row = path_trace[row][col].first;
        } else if (row < col) {
            ans.push(2);
            col = path_trace[row][col].second;
        } else {
            exit(2);
        }
    }

    while (ans.empty() == false) {
        printf("%d\n", ans.top());
        ans.pop();
    }
}

int main()
{
    int row, col;
    int ans = IMPOSSIBLE;

    scanf("%d\n%d", &n, &w);
    load_case_to_index();
    for (int i = 1; i <= w; ++i) {
        min_cost[i][i] = IMPOSSIBLE;
    }

    for (int i = 0; i <= w; ++i) {
        for (int j = 0; j <= w; ++j) {
            if (i != j) {
                calculate_min_cost(i, j);
            }
        }
    }
    for (int i = 0; i < w; ++i) {
        if (min_cost[i][w] < ans) {
            row = i;
            col = w;
            ans = min_cost[i][w];
        }
        if (min_cost[w][i] < ans) {
            row = w;
            col = i;
            ans = min_cost[w][i];
        }
    }
    printf("%d\n", ans);
    print_trace(row, col);
}