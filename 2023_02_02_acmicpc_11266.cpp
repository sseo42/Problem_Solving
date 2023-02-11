#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      단절점 문제.
 *      정점 v[1, 10000] 과 간선 e[1,100000] 이 주어진다.
 *      이후 e 줄에 걸쳐 간선이 연결하는 두 정점이 주어진다.
 * 
 * Ideas
 *      단절선 문제를 풀며 봤던 알고리즘을 단절점으로 구현하자.
 * 
 * Comments
 *      루트의 경로가 2개이상일 떄 단절점이라는 것은 단순한 간선의 수가 아니라
 *      루트에서 dfs 를 2번 이상 출발한다는 것이다. 따라서 dfs 안에서 뻗어나가는 수를 체크해야된다.
 * 
 *      단절선에서는 parent 를 걸러줬지만 단절점에서는 그러지 않아도 괜찮다,
 *      왜냐하면 단절선의 경우 자식으로 출발해서 자신으로 돌아온다면 해당 간선이 단절선이 아니지만
 *      단절점은 자식으로 출발해 자신으로 돌아온다면 자신은 단절점이기 떄문이다.
 * 
 *      단절선은 마지막 리프노드를 잇는 간선이 단절선이 될수 있으므로, for loop 밖에서 체크가 
 *      가능하지만(함수의 parent 와 current_node 가 단절선이 됨), 단절점은 for loop 안에서 
 *      체크해야한다.
 */

vector<int> discovery;
vector<vector<int> > edge;
vector<bool> is_articulation_point;

int find_articulation_point(int node, bool is_root)
{
    static int discovery_num;

    int min_reachable_point;
    int child_cnt = 0;

    discovery[node] = ++discovery_num;
    min_reachable_point = discovery[node];
    for (vector<int>::iterator next_node = edge[node].begin(); next_node != edge[node].end(); ++next_node) {
        if (discovery[*next_node] > 0) {
            min_reachable_point = min(discovery[*next_node], min_reachable_point);
            continue;
        }
        ++child_cnt;
        
        int prev = find_articulation_point(*next_node, false);

        if (prev >= discovery[node]) {
            is_articulation_point[node] = true;
        }
        min_reachable_point = min(prev, min_reachable_point);
    }
    if (is_root == true) {
        if (child_cnt > 1) {
            is_articulation_point[node] = true;
        } else {
            is_articulation_point[node] = false;
        }
    }
    return min_reachable_point;
}

int main()
{
    int node_cnt, edge_cnt;
    int node_a, node_b;
    int total_cnt = 0;

    scanf("%d %d", &node_cnt, &edge_cnt);
    discovery.resize(node_cnt + 1);
    edge.resize(node_cnt + 1);
    is_articulation_point.resize(node_cnt + 1);
    for (int i = 0; i < edge_cnt; ++i) {
        scanf("%d %d", &node_a, &node_b);
        edge[node_a].push_back(node_b);
        edge[node_b].push_back(node_a);
    }
    for (int i = 1; i <= node_cnt; ++i) {
        if (discovery[i] == false) {
            find_articulation_point(i, true);
        }
        if (is_articulation_point[i] == true) {
            ++total_cnt;
        }
    }
    printf("%d\n", total_cnt);
    for (int i = 1; i <= node_cnt; ++i) {
        if (is_articulation_point[i] == true) {
            printf("%d ", i);
        }
    }
}