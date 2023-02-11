#include <iostream>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      n[2, 50000] 개의 정점으로 이루어진 트리가 주어진다.
 *      각 노드는 각 1번 부터 n번까지 번호가 메겨져있으며, 루트는 1번이다.
 * 
 *      첫줄 n이 주어지고, 이어 n - 1 줄에 걸쳐 트리의 연결 정보가 주어진다.
 *      이어 m[1, 10000]이 주어지고 m줄에 걸쳐 노드 두개가 주어진다.
 * 
 *      두 노드의 lca 를 구하자.
 * 
 * Ideas
 *      일요일날 학습했었던 lca를 익히기 위한 문제, 노드 마다 부모 및 shortcut 을 잡아주자.
 * 
 * Comments
 *      ancestor 를 구성할 때 init value 를 어떻게 둬야할지 고민했다.
 *      root node 인 1로 둘까 했지만, 루트노드와 그 자식 사이에서 핑퐁칠 수 있어서 기각했다.
 * 
 *      처음 제출 할 때 node_level 등을 main 의 local 로 두었는데, 이 부분이 stack memory를 오버해서 런타임에러를 유발했다.
 * 
 *      수정 후 제출했을 때 make_ancestor 의 이중 루프 순서 떄문에 틀렸다.
 *      전체노드의 부모, 할아버지 ... 순서로 구성을 해야 참조가 올바른데 n 에 대한 루프를 바깥으로 두어 로직이 올바른 참조를 못했다.
 * 
 *      알고리즘에 대한 이해가 부족했던 것이기에 충분히 숙지만 한다면 문제는 없을 것이다.
 *      + 다른 알고리즘 문제를 풀 때도 큰 것들은 전역으로 뺴놓자.
 */

int n;
vector<int> node_level;
vector<vector<int> > link;
vector<vector<int> > ancestor;

int get_p2(int n)
{
    int p2 = 0;
    int val = 1;

    while (val < n) {
        ++p2;
        val *= 2;
    }
    return p2;
}

void make_ancestor()
{
    static const int root_node = 1;
    static const int init_val = 0;
    
    int p2 = get_p2(n);
    queue<pair<int, int> > node_q;

    ancestor.resize(n + 1);
    for (int i = 0; i <= n; ++i) {
        ancestor[i].resize(p2, init_val);
    }
    ancestor[root_node][0] = root_node;
    node_level[root_node] = 1;
    node_q.push({root_node, 1});
    while (node_q.empty() == false) {
        int current_node = node_q.front().first;
        int current_level = node_q.front().second;

        node_q.pop();
        for (vector<int>::iterator next_node = link[current_node].begin(); next_node != link[current_node].end(); ++next_node) {
            if (ancestor[*next_node][0] == init_val) {
                ancestor[*next_node][0] = current_node;
                node_level[*next_node] = current_level + 1;
                node_q.push({*next_node, current_level + 1});
            }
        }
    }
    for (int j = 1; j < p2; ++j) {
        for (int i = 1; i <= n; ++i) {
            ancestor[i][j] = ancestor[ancestor[i][j - 1]][j - 1];
        }
    }
}

int lca(int node_a, int node_b)
{
    if (node_level[node_a] > node_level[node_b]) {
        swap(node_a, node_b);
    }
    for (int i = ancestor[node_b].size() - 1; i >= 0; --i) {
        if (node_level[ancestor[node_b][i]] < node_level[node_a]) {
            continue;
        } 
        node_b = ancestor[node_b][i];
    }
    if (node_a == node_b) {
        return node_a;
    }
    for (int i = ancestor[1].size() - 1; i >= 0; --i) {
        if (ancestor[node_a][i] != ancestor[node_b][i]) {
            node_a = ancestor[node_a][i];
            node_b = ancestor[node_b][i];
        }
    }
    return ancestor[node_a][0];
}

int main()
{
    int m;
    int node_a, node_b;

    scanf("%d", &n);
    node_level.resize(n + 1);
    link.resize(n + 1);

    for (int i = 1; i < n; ++i) {
        scanf("%d %d", &node_a, &node_b);
        link[node_a].push_back(node_b);
        link[node_b].push_back(node_a);
    }
    make_ancestor();

    scanf("%d", &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d", &node_a, &node_b);
        printf("%d\n", lca(node_a, node_b));
    }
}