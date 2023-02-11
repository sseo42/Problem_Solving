#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

/**
 * Problem
 *      1 <= n <= 1,000     : 노드의 수
 *      1 <= m <= 10,000    : 간선의 수
 *      
 *      n 과 m 그리고 타겟노드 x 가 주어진다.
 * 
 *      이어 m 즐어 걸쳐 간선에 대한 정보가 주어진다.
 * 
 *      a, b, c -> a에서 b 로 가는 간선의 비용 c[1,100]
 * 
 *      x 노드까지의 왕복 비용이 가장 큰 값을 구하라.
 *      a -> b 방향의 간선은 1개만이 주어진다.
 *      모든 노드는 x 에 접근 가능하며 x 에서도 모든 노드에 접근가능하다.
 * 
 * Ideas
 *      각 노드에서 크루스칼 돌린다.
 *      방문 순서를 저장하자, 이러면 경로에 있는 노드에 대해서는 다시 계산할 필요 없다.
 * 
 *      이를 통해 최소거리를 배열에 저장하고, 순회하여 최소값을 얻은 뒤 출력한다.
 * 
 * Comments
 *      출발노드에서 모든 노드에 대한 최소 거리를 얻는다....
 *      간선을 역전한다면 1번만 돌려서 x_node 까지의 값을 구할 수 있었을 것이다..!
 * 
 *      추적을 안하므로 불필요한 착업도 없다
 * 
 *      실수는 따로 없었지만 핵심을 파악했다면 더 좋았을 듯
 */

#define MAX_NODE 1000

int n, m, x_node;
int dist[MAX_NODE + 1];
int distance_to_x[MAX_NODE + 1];
vector<vector<pair<int, int> > > links;

void dijkstra_with_trace(int start)
{
    typedef pair<int, int> cost_at_node;

    priority_queue<cost_at_node, vector<cost_at_node>, greater<cost_at_node> > pq;
    int from_node_of[n + 1];
    bool visited[n + 1];
    int cost, node;

    memset(dist, 127, sizeof(int) * (n + 1));
    memset(from_node_of, 0, sizeof(int) * (n + 1));
    memset(visited, 0, sizeof(bool) * (n + 1));

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        cost = pq.top().first;
        node = pq.top().second;
        pq.pop();

        visited[node] = true;
        for (vector<pair<int, int> >::iterator cost_to_node = links[node].begin(); cost_to_node != links[node].end(); ++cost_to_node) {
            if (cost + cost_to_node->first < dist[cost_to_node->second]) {
                dist[cost_to_node->second] = cost + cost_to_node->first;
                from_node_of[cost_to_node->second] = node;
                pq.push({cost + cost_to_node->first, cost_to_node->second});
            }
        }
    }
    node = x_node;
    while (node != start) {
        int from_node = from_node_of[node];

        distance_to_x[from_node] = distance_to_x[node] + dist[node] - dist[from_node];
        node = from_node;
    }
}

int main(void)
{
    int from_node, to_node, cost;
    int max_val = 0;

    scanf("%d %d %d", &n, &m, &x_node);
    links.resize(n + 1);

    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &from_node, &to_node, &cost);
        links[from_node].push_back({cost, to_node});
    }
    for (int i = 1; i <= n; ++i) {
        if (distance_to_x[i] == 0 && i != x_node) {
            dijkstra_with_trace(i);
        }
    }
    dijkstra_with_trace(x_node);

    for (int i = 1; i <= n; ++i) {
        if (dist[i] + distance_to_x[i] > max_val) {
            max_val = dist[i] + distance_to_x[i];
        }
    }
    printf("%d\n", max_val);
}