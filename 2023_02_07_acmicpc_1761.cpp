#include <iostream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      첫줄에 노드의 개수 n[1,40000] 이 주어지고,
 *      이후 n - 1 줄에 걸쳐 트리를 구성하는 간선정보가 주어진다.
 *      간선정보는 node_a, node_b, weight 으로 구성되있으며,
 * 
 *      이후 질문수 m[1,10000] 이 주어지고,
 *      m 줄에 걸쳐 두 노드 번호가 주어진다. 두 노드사이의 거리를 물어보는 각 질문에 대해 답을 출력하자.
 *      노드 사이거리는 10,000 이하의 자연수이다.
 * 
 * Ideas
 *      매 질문마다 구하게 된다면 40,000 * 10,000 이므로 넘친다.
 *      두 노드의 거리는 항상 공통조상을 거쳐야하므로 이는 lca 를 이용하되 pair 를 써서 거리 또한 같이 저장하면된다.
 * 
 * Comments
 *      level 이 낮을 수록 root 에 가까운 노드다 lca 를 위해 level 을 맞출 때는 level 큰놈을 조정하는데
 *      level 이 더 낮아지는 경우를 continue 처리해야된다.
 */

int n;
vector<int> node_level;
vector<vector<pair<int, int> > > distance_graph;
vector<vector<pair<int, int> > > distance_to_ancestor;

int get_pow_to_ceiling()
{
    int res = 0;

    while ((1<<res) <= n) {
        ++res;
    }
    return res;
}

void fill_level_and_ancestor(int pow_to_ceiling)
{
    queue<pair<int, int> > level_q;

    distance_to_ancestor[1][0].first = 0;
    distance_to_ancestor[1][0].second = 1;
    level_q.push({1, 1});

    while (level_q.empty() == false) {
        int level = level_q.front().first;
        int node = level_q.front().second;

        level_q.pop();
        node_level[node] = level;
        for (vector<pair<int, int> >::iterator next = distance_graph[node].begin(); next != distance_graph[node].end(); ++next) {
            if (node_level[next->second] > 0) {
                distance_to_ancestor[node][0].first = next->first;
                distance_to_ancestor[node][0].second = next->second;
                continue;
            }
            level_q.push({level + 1, next->second});
        }
    }
    for (int i = 1; i < pow_to_ceiling; ++i) {
        for (int j = 1; j <= n; ++j) {
            int ancestor_distance = distance_to_ancestor[j][i - 1].first + distance_to_ancestor[distance_to_ancestor[j][i - 1].second][i - 1].first;
            int ancestor_node = distance_to_ancestor[distance_to_ancestor[j][i - 1].second][i - 1].second;

            distance_to_ancestor[j][i].first = ancestor_distance;
            distance_to_ancestor[j][i].second = ancestor_node;
        }
    }
    
}

int initialize()
{
    int pow_to_ceiling;
    int node_a, node_b, weight;

    scanf("%d\n", &n);
    node_level.resize(n + 1);
    distance_graph.resize(n + 1);
    distance_to_ancestor.resize(n + 1);
    pow_to_ceiling = get_pow_to_ceiling();

    for (int i = 1; i < n; ++i) {
        scanf("%d %d %d\n", &node_a, &node_b, &weight);
        distance_graph[node_a].push_back({weight, node_b});
        distance_graph[node_b].push_back({weight, node_a});
    }
    for (int i = 1; i <= n; ++i) {
        distance_to_ancestor[i].resize(pow_to_ceiling);
    }
    fill_level_and_ancestor(pow_to_ceiling);
    return pow_to_ceiling;
}

int lca_distance(int node_a, int node_b, int pow_to_ceiling)
{
    int res = 0;

    if (node_level[node_a] < node_level[node_b]) {
        swap(node_a, node_b);
    }
    for (int i = pow_to_ceiling - 1; i >= 0; --i) {
        if (node_level[distance_to_ancestor[node_a][i].second] < node_level[node_b]) {
            continue;
        }
        res += distance_to_ancestor[node_a][i].first;
        node_a = distance_to_ancestor[node_a][i].second;
    }
    if (node_a == node_b) {
        return res;
    }
    for (int i = pow_to_ceiling - 1; i >= 0; --i) {
        if (distance_to_ancestor[node_a][i].second == distance_to_ancestor[node_b][i].second) {
            continue;
        }
        res += distance_to_ancestor[node_a][i].first + distance_to_ancestor[node_b][i].first;
        node_a = distance_to_ancestor[node_a][i].second;
        node_b = distance_to_ancestor[node_b][i].second;
    }

    res += distance_to_ancestor[node_a][0].first;
    res += distance_to_ancestor[node_b][0].first;
    return res;
}

int main()
{
    int m, node_a, node_b;
    int pow_to_ceiling = initialize();

    scanf("%d\n", &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d\n", &node_a, &node_b);

        printf("%d\n", lca_distance(node_a, node_b, pow_to_ceiling));
    }
}