#include <iostream>
#include <queue>
#include <utility>

using namespace std;

/**
 * Problem
 *      여러 개의 테스트 케이스로 구성된다.
 *      각 테케의 첫줄에는 동굴의 크기를 나타내는 정수 n [2,125] 가 주어진다.
 *      n = 0 이 주어지면 종료이다.
 * 
 *      n 이 주어진 뒤에는 n 줄에 걸쳐 행렬이 주어진다.
 *      왼쪽 위가 {0,0} 오른쪽 아래가 {n-1, n-1} 이다.
 * 
 *      주인공은 {0,0} 에서 시작해서 {n-1,n-1} 까지 이동해야하며 상하좌우로 밖에 이동하지 못한다.
 *      이동을 하면 배열의 해당 값 만큼 돈을 잃는다, 잃는 최소금액을 구하자.
 * 
 * Ideas
 *      dp 로 해당 지점까지 ...
 * 
 *      흠 그럼 돌아가는 경우는? 다익스트라로 풀어야겠다.
 * 
 * Comments
 *      특이사항은 없었다, 일반적인 노드를 저장하는 형식대로 pq를 저장했다가 다시 수정했다.
 *      또한 pq 의 우선 순서를 잘못 지정하여 디버깅에 시간이 소모됬다.
 * 
 *      새벽에 풀은 탓에 졸린 이유도 있겠지만 충분히 신경만 쓴다면 고려할 수 있는 부분이였다.
 *      핵심에 대한 이해가 부족한 탓이다, 문제를 풀고 사고하는 시간을 가져 이해를 높일 필요가 있다.
 */

#define MAX_NODE 125
#define IMPOSSIBLE 10000000

int n;
int cost[MAX_NODE][MAX_NODE];
int min_cost[MAX_NODE][MAX_NODE];
bool visited[MAX_NODE][MAX_NODE];

void init_min_cost(int n)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            min_cost[i][j] = IMPOSSIBLE;
        }
    }
}

void init_vistied(int n)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            visited[i][j] = false;
        }
    }
}

void read_cost(int n)
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            scanf("%d", cost[i] + j);
        }
    }
}

int solution(int n)
{
    int min_val;
    int r, c;
    int dx[4] = {-1, 0, 1, 0};
    int dy[4] = {0, -1, 0, 1};

    priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> > > > pq;

    min_cost[0][0] = cost[0][0];
    pq.push({min_cost[0][0], {0, 0}});
    while (!pq.empty()) {
        min_val = pq.top().first;
        r = pq.top().second.first;
        c = pq.top().second.second;
        pq.pop();

        if (visited[r][c] == true) {
            continue;
        }
        visited[r][c] = true;
        min_cost[r][c] = min_val;

        for (int i = 0; i < 4; ++i) {
            if (r + dx[i] < 0 || r + dx[i] >= n || c + dy[i] < 0 || c + dy[i] >= n) {
                continue;
            }
            pq.push({{min_val + cost[r + dx[i]][c + dy[i]]}, {r + dx[i], c + dy[i]}});
        }
    }
    return min_cost[n - 1][n - 1];
}

int main()
{
    int n;
    int test_case_count = 1;

    scanf("%d", &n);
    while (n != 0) {
        read_cost(n);
        init_min_cost(n);
        init_vistied(n);
        printf("Problem %d: %d\n", test_case_count, solution(n));
        ++test_case_count;
        scanf("%d", &n);
    }
}