#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>

using namespace std;

/**
 * Problem
 *      노드 수 n[1, 100000] 과 간선 m[1, 1000000] 이 주어지고,
 *      이어 m 줄에 걸쳐 간선이 연결하는 두 노드가 주어진다.
 *      중복되는 간선이 주어지지 않고 주어지는 그래프는 모든 노드가 연결되있다.
 * 
 *      단절선의 갯수를 출력하고 각 단절선이 연결하는 두노드를 출력하자.
 * 
 * Ideas
 *      링크를 순서대로 구성해서 스패닝트리를 구성한다.
 *      이후 역순으로 돌아 스패닝트리를 구성하고 중복되어 쓰인 링크들이 단절선이다
 * 
 *      스패닝트리를 구성하기 위해서는 단절선이 무조건 쓰인다.
 *      그렇다면 단절선이 아닌 간선이 중복되어 쓰이는 경우가 존재하는가?
 *      존재한다
 * 
 *      힌트에서 dfs 가 보인다...탐색이 생명인데...
 * 
 *      단절점 단절선 알고리즘을 봤다. 타잔 알고리즘 같이 discovery를 써서 dfs 탐색에 발견 순서를 메기고
 *      스패닝트리 구성하도록 dfs 돌릴 때 부모와 자신을 잇는 간선을 사용하지 않고 이전 탐색 노드로
 *      접근 가능한지를 본다, 접근 가능하다면 이는 단절선이 아닌것이다.
 * 
 * Comments
 *      단절선에 대한 블로그 코드를 참조해서 풀었다.
 *      discovery를 함수의 진입에서 체크하면 이미 dfs 를 타고 이미 방문했던 노드라면 이것을 가지고
 *      단절선이라 판단되므로 루프 안에서 visited 한 경우를 continue 처리해야한다.
 * 
 *      단절선 판단은 dfs 로 접근 가능한 모든 노드에서 다시 위로 돌아가지 못함을 확인하는 것이다.
 */

#define MAX_NODE 100000

int n, m;
int discovery[MAX_NODE + 1];
vector<vector<int> > link(MAX_NODE + 1);
vector<pair<int, int> > articulation_bridge;

int calculate_articulation_bridge(int node, int parent)
{
    static int discovery_cnt = 0;
    int min_discovery;

    discovery[node] = ++discovery_cnt;
    min_discovery = discovery[node];
    for (vector<int>::iterator next_node = link[node].begin(); next_node != link[node].end(); ++next_node) {
        if (*next_node == parent) {
            continue;
        }
        if (discovery[*next_node] > 0) {
            min_discovery = min(discovery[*next_node], min_discovery);
            continue;
        }
        min_discovery = min(calculate_articulation_bridge(*next_node, node), min_discovery);
    }
    if (min_discovery > discovery[parent]) {
        articulation_bridge.push_back({min(node, parent), max(node, parent)});
    }
    return min_discovery;
}

int main()
{
    int node_a, node_b;

    scanf("%d %d\n", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d\n", &node_a, &node_b);
        link[node_a].push_back(node_b);
        link[node_b].push_back(node_a);
    }
    calculate_articulation_bridge(1, 1);
    printf("%ld\n", articulation_bridge.size());
    sort(articulation_bridge.begin(), articulation_bridge.end());
    for (vector<pair<int, int> >::iterator ans = articulation_bridge.begin(); ans != articulation_bridge.end(); ++ans) {
        printf("%d %d\n", ans->first, ans->second);
    }
}