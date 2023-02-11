#include <iostream>
#include <utility>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

/**
 * Problem
 *      노드의 수 n[2, 200]와 간선의 수 m[n-1,20000] 이 주어지고
 *      이어 start, end, weight[1,100] 의 간선 정보가 주어진다. 
 * 
 *      한 노드에 불을 붙이면 이는 weight 만큼 소요해 이어진노드로 간다.
 *      만약 간선의 양노드가 불이 붙으면 간선의 중앙까지 태운 후 멈춘다.
 * 
 *      임의의 한 노드에 불을 붙을 떄 모든 노드를 태우는 최소시간을 소수점 아래 한자리까지 출력하자
 * 
 * Ideas
 *      각 노드에서 다익스트라로 최소 거리를 구하자
 *      이어 모든 간선을 살펴서 양쪽 노드에서 작은쪽 s + 간선 weight > 큰쪽 b 일 때
 *      그 차이를 반으로 나눠 extra_time 이라하자. max(extra_time) 을 구해 알아낸 최대거리 노드에 더해 출력하자
 * 
 *      같은 노드를 잇는 여러 간선이 있을 수 있으므로 vector로 밀어 넣자.
 * 
 * Comments
 *      max(extra_time) 을 구해더하면 엉뚱한 최대값이랑 붙을 수 있다.
 *      요지는 간선이 덜타서 나중에 도착한 노드에서도 진행이 조금 되야하는 점이므로 나중에 도착한 노드의 dist 에 더해서
 *      갱신한다.
 */

#define MAX_NODE 200

int n, m;
vector<vector<pair<int, int> > > link(MAX_NODE + 1);
int min_distance[MAX_NODE + 1][MAX_NODE + 1];

void dijkstra(int start_node)
{
    static const int init_val = -1;
    int cost, node;
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;

    pq.push({0, start_node});
    while (pq.empty() == false) {
        cost = pq.top().first;
        node = pq.top().second;
        pq.pop();

        if (min_distance[start_node][node] != init_val) {
            continue;
        }
        min_distance[start_node][node] = cost;
        for (vector<pair<int, int> >::iterator next_node = link[node].begin(); next_node != link[node].end(); ++next_node) {
            if (min_distance[start_node][next_node->second] == init_val) {
                pq.push({cost + next_node->first, next_node->second});
            }
        }
    }
}

int main()
{
    static const int weight_bias = 10;
    int from_node, to_node, weight;
    int ans = 2000000000;

    memset(min_distance, -1, sizeof(int) * (MAX_NODE + 1) * (MAX_NODE + 1));
    scanf("%d %d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d %d", &from_node, &to_node, &weight);
        link[from_node].push_back({weight * weight_bias, to_node});
        link[to_node].push_back({weight * weight_bias, from_node});
    }
    for (int i = 1; i <= n; ++i) {
        dijkstra(i);
    }
    for (int i = 1; i <= n; ++i) {
        int longest_distance = 0;

        for (int j = 1; j <= n; ++j) {
            if (min_distance[i][j] > longest_distance) {
                longest_distance = min_distance[i][j];
            }
            for (vector<pair<int, int> >::iterator next_node = link[j].begin(); next_node != link[j].end(); ++next_node) {
                int a_arrived = min_distance[i][j];
                int b_arrived = min_distance[i][next_node->second];

                if (a_arrived > b_arrived) {
                    swap(a_arrived, b_arrived);
                }
                if (a_arrived + next_node->first > b_arrived) {
                    longest_distance = max(longest_distance, (a_arrived + next_node->first + b_arrived) / 2);
                }
            }
        }
        if (longest_distance < ans) {
            ans = longest_distance;
        }
    }
    printf("%.1f\n", (float)ans / 10);
}