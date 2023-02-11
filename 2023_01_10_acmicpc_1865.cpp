#include <iostream>

using namespace std;

/**
 * Problem
 *      1 <= n <= 500       : 노드의 수
 *      1 <= m <= 2,500     : 도로의 개수
 *      1 <= w <= 200       : 웜홀의 개수
 * 
 *      도로는 양방향 간선이고 이동에 양의 비용이 든다.
 * 
 *      웜홀은 방향이 있는 간선이며 이동 시 음의 비용이 든다.
 * 
 *      테케의 수 t 와
 *      각 테케의 n, m, w 그리고 
 *      m 줄에 걸쳐 도로가 연결하는 두 노드 + 비용(양)
 *      w 줄에 걸쳐 웜홀이 열결하는 시작, 도착 노드 + 비용(음)
 *      0 <= cost <= 10,000 이다.
 * 
 *      이 주어질 때 한 지점에서 시작해 다시 돌아왔을 때 음의 비용이 발생하는 경우가 있는지 찾자.
 * 
 * Ideas
 *      한 노드로 부터 나머지 노드까지의 최소비용을 구하자, 이는 일반적인 min cost 로 풀 수 있다.
 *      재방문을 통해 코스트를 낮출수 있다면 음의 사이클이 생긴다는 것이고 이는 사이클 중 한 노드에서 서칭할 때 중복 방문없이 가능할 것이다.
 * 
 *      한 노드당 m 의 작업이 걸리므로 n * m 으로 전체를 찾을 수 있다. 이를 n x n 배열에 저장하고
 * 
 *      (a -> b) + (b -> a) 중 음수 가 있나 보면 된다.
 * 
 * Comments
 *      이 문제 다익스트라로 풀려 시도했다. 너무나도 당연하지만 틀렸고, 심지어 MST 랑 이상하게 섞어 생각해서
 *      다익스트라도 아니였다. 요렇게 풀 때 priority_queue 썻는데 대충 오름차순 정렬 -> greater<vector<모시기> > 정도 기억하면 좋겠다.
 * 
 *      질문 및 블로그 쓱 봐서 벨만포드로 풀어야함을 알았다.
 *      음수에 대해 다익스트라 못쓰는 것도 완전 까먹고, 이상하게 생각해서 왜 안 되지 생각했다.
 * 
 *      벨만포드 쓱 봐서 구현해서 틀렸다, 일반적인 해당 알고리즘의 구현은 시작정점이 있기에 그대로 써서 틀렸다.
 * 
 *      1번 정점을 임의로 시작으로 두었는데, 1 번 정점으로부터 방문할 수 없는 곳에 음의 사이클이 있을 경우 이를 탐지 못함을 깨닳고 매 정점을 시작으로
 *      두려 했으나 시간복잡도를 맞출 수 없음을 깨닳았다, 이후 질문게시판을 보고 이해했다.
 * 
 *      해당 문제의 목적은 음의 사이클이 있는 것이므로 이동에 필요한 구체적인 최소값이 얼마냐인지는 필요가 없다.
 * 
 *      따라서 방문을 따질 필요 없이 그냥 최소값으로 업데이트하면 된다 -> 어느 곳을 시작점으로 잡든 n - 1 회 노드로부터 간선들을 업데이트하면
 *      최솟값이 갱신될 것이다, 최솟값으로만 업데이트하므로 섞여서 수행되더라도 n - 1 회면 안정되게 된다(음의 사이클이 없다면)
 * 
 *      따라서 시작점을 잡지 않고 모든 간선을 n - 1 회 업데이트하고 n 회째에서 업데이트가 수행된다면 음의 사이클이 있음을 알수 있으며,
 *      모든 정점에 대한 탐색이 가능하다.
 */

#define IMPOSSIBLE 100000000
#define MAX_NODE 500

int n, m, w;
int graph[MAX_NODE + 1][MAX_NODE + 1];
int total_cost[MAX_NODE + 1];

void init_graph()
{
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            graph[i][j] = IMPOSSIBLE;
        }
    }
}

void init_total_cost()
{
    for (int i = 1; i <= n; ++i) {
        total_cost[i] = IMPOSSIBLE;
    }
}

bool exist_negative_cycle(void)
{
    init_total_cost();

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            for (int k = 1; k <= n; ++k) {
                if (graph[j][k] < IMPOSSIBLE) {
                    if (total_cost[j] + graph[j][k] < total_cost[k]) {
                        total_cost[k] = total_cost[j] + graph[j][k];
                        if (i == n) {
                            return true;
                        }
                    }
                }
            }
        }
    }
   
    return false;
}

int main(void)
{
    int test_case;
    int a, b, cost;

    scanf("%d", &test_case);

    for (int i = 0; i < test_case; ++i) {
        scanf("%d %d %d", &n, &m, &w);
        init_graph();

        for (int j = 1; j <= m; ++j) {
            scanf("%d %d %d", &a, &b, &cost);

            if (cost < graph[a][b]) {
                graph[a][b] = cost;
            }
            if (cost < graph[b][a]) {
                graph[b][a] = cost;
            }
        }
        for (int j = 1; j <= w; ++j) {
            scanf("%d %d %d", &a, &b, &cost);

            if (-cost < graph[a][b]) {
                graph[a][b] = -cost;
            }
        }
        if (exist_negative_cycle()) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }
}