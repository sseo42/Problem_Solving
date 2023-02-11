#include <iostream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      노드의 수 n[1, 100000] 에 이에 n-1줄에 걸쳐 트리의 간선 정보가 주어진다.
 *      이어 m[1, 100000] 이 주어지고 m줄에 걸쳐 공통조상을 알고자하는 두 노드의
 *      번호가 주어진다.
 * 
 *      m 줄에 걸쳐 공통 조상을 출력하자.
 * 
 * Ideas
 *      LCA 구현. anscestor 를 놓고 채우자. 이를 이용해 찾아가기전에 두 노드간 level 을
 *      맞춰야하므로 bfs + pair 로 트리를 돌면서 level 을 적어주자.
 * 
 * Comments
 *      loop 의 범위와 트리의 level 개념을 좀더 탄탄히 해야된다.
 *      헷갈려하니 구현에서 실수가 나온다.
 */

#define ROOT_NODE 1

vector<vector<int> > link;
vector<vector<int> > ancestor;
vector<int> node_level;

int get_pow2ceiling(int n)
{
    int res = 0;

    while ((1 << res) <= n) {
        ++res;
    }
    return res;
}

void initialize(int n, int pow2ceiling)
{
    link.resize(n + 1);
    ancestor.resize(n + 1);
    node_level.resize(n + 1);

    for (int i = 1; i <= n; ++i) {
        ancestor[i].resize(pow2ceiling);
    }
}

void prepare(int n, int pow2ceiling)
{
    queue<pair<int, int> > bfs_q;

    ancestor[ROOT_NODE][0] = ROOT_NODE;
    node_level[ROOT_NODE] = 1;
    bfs_q.push({ROOT_NODE, 1});

    while (bfs_q.empty() == false) {
        int node = bfs_q.front().first;
        int level = bfs_q.front().second;

        bfs_q.pop();
        for (vector<int>::iterator next = link[node].begin(); next != link[node].end(); ++next) {
            if (node_level[*next] < 1) {
                ancestor[*next][0] = node;
                node_level[*next] = level + 1;
                bfs_q.push({*next, level + 1});
            }
        }
    }
    for (int i = 1; i < pow2ceiling; ++i) {
        for (int j = 1; j <= n; ++j) {
            ancestor[j][i] = ancestor[ancestor[j][i - 1]][i - 1];
        }
    }
}

int lca(int node_a, int node_b, int n, int pow2ceiling)
{
    if (node_level[node_a] < node_level[node_b]) {
        swap(node_a, node_b);
    }
    for (int i = pow2ceiling - 1; i >= 0; --i) {
        if (node_level[node_a] == node_level[node_b]) {
            break;
        }
        if (node_level[ancestor[node_a][i]] >= node_level[node_b]) {
            node_a = ancestor[node_a][i];
        }
    }
    if (node_a == node_b) {
        return node_a;
    }
    for (int i = pow2ceiling - 1; i >= 0; --i) {
        if (ancestor[node_a][i] != ancestor[node_b][i]) {
            node_a = ancestor[node_a][i];
            node_b = ancestor[node_b][i];
        }
    }
    return ancestor[node_a][0];
}
int main()
{
    int n, m;
    int pow2ceiling;
    int node_a, node_b;

    scanf("%d\n", &n);
    pow2ceiling = get_pow2ceiling(n);
    
    initialize(n, pow2ceiling);
    for (int i = 1; i < n; ++i) {
        scanf("%d %d\n", &node_a, &node_b);
        link[node_a].push_back(node_b);
        link[node_b].push_back(node_a);
    }
    prepare(n, pow2ceiling);
    scanf("%d\n", &m);
    for (int i = 0; i < m; ++i) {
        scanf("%d %d\n", &node_a, &node_b);
        printf("%d\n", lca(node_a, node_b, n, pow2ceiling));
    }
}