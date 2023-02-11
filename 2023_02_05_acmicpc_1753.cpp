#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      정점의 개수 v[1, 20000] 와 간선의 개수 e[1, 300000] 이 주어진다.
 *      이어 시작 정점의 번호가 주어지고, e 줄에 걸쳐 간선의 정보
 *      node_a, node_b, weight 이 주어진다.
 *      간선의 가중치는 10 이하의 자연수 이며, 만약 경로가 존재하지 않을 경우 INF 를 출력하자.
 * 
 * Ideas
 *      양의 가중치, 기본 다익스트라 문제.
 * 
 * Comments
 *      문제를 꼼꼼히 읽어야한다.
 *      가중치 그래프에서 pair 로 표현할 때, cost 를 first 로 node 를 second 으로 통일하자.
 */

#define INIT_COST -1

int n, m;
vector<vector<pair<int, int> > > weight_edge;
vector<int> minimum_cost;

void dijkstra(int start_node)
{
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    pq.push({0, start_node});
    while (pq.empty() == false) {
        int cost = pq.top().first;
        int node = pq.top().second;

        pq.pop();
        if (minimum_cost[node] != INIT_COST) {
            continue;
        }
        minimum_cost[node] = cost;
        for (vector<pair<int, int> >::iterator next = weight_edge[node].begin(); next != weight_edge[node].end(); ++next) {
            if (minimum_cost[next->second] != INIT_COST) {
                continue;
            }
            pq.push({cost + next->first, next->second});
        }
    }
}

int main()
{
    int start_node;
    int node_a, node_b, cost;

    scanf("%d %d\n", &n, &m);
    scanf("%d\n", &start_node);
    weight_edge.resize(n + 1);
    minimum_cost.resize(n + 1, INIT_COST);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d\n", &node_a, &node_b, &cost);
        weight_edge[node_a].push_back({cost, node_b});
    }
    dijkstra(start_node);
    for (int i = 1; i <= n; ++i) {
        if (minimum_cost[i] == INIT_COST) {
            printf("INF\n");
        } else {
            printf("%d\n", minimum_cost[i]);
        }
    }
}