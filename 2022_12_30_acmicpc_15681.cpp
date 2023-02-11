#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      트리가 주어진다.
 *      2 <= N <= 100,000   : 노드의 갯수
 *      1 <= R <= N         : 루트의 번호
 *      1 <= Q <= 100,000   : 쿼리의 수
 * 
 *      N - 1 개의 간선이 주어지고 이어 Q 개의 쿼리가 주어진다.
 * 
 *      쿼리에서는 노드의 번호가 주어지며 해당 노드의 서브트리에 속한 노드의 수를 구하자.
 * 
 * Ideas
 *      NlogN 혹은 N 으로 문제를 풀어야한다.
 *      트리가 주어진다 보장되므로 사이클은 없다.
 * 
 *      U, V 로 간선이 주어질 때 양방향으로 간선을 뚫어준다면 한 노드에서 연결된 노드를 탐색할 떄 두가지 케이스로 나눌 수 있다.
 * 
 *      하나는 처음 방문하는 노드, 혹은 부모 노드.
 * 
 *      따라서 한 노드에서 다음 노드로 넘어갈 떄 자신의 번호를 알려준다면 양방향으로 인한 사이클 없이 탐색 가능하다.
 * 
 *      이를 재귀적으로 구성하고 리턴으로 서브트리의 노드 수를 돌려준다면 이 후 쿼리는 Q 로 처리된다.
 * 
 *      탐색 작업은 N이므로 N + Q 로 해결 가능하다.
 * 
 * Comments
 *      변수의 스코프를 잡을 때 고민한 부분이 있다.
 *      문제의 핵심이 되는 구조는 전역으로 뺴는게 맞을 거 같다. 실전이라면 namespace 등으로 상위 스코프를 두었겠지..?
 * 
 *      간선정보를 읽을 때 갯수를 잘못 읽었다. 차분하게 생각하자.
 */

#define MAX_NODE_CNT 100000

int subnode_counter[MAX_NODE_CNT + 1];
vector<vector<int> > connection(MAX_NODE_CNT + 1);

int make_subtree(int current_node, int parent_node)
{
    int cnt_subnode = 1;

    for (vector<int>::iterator subnode_iter = connection[current_node].begin();
         subnode_iter != connection[current_node].end(); ++subnode_iter) {
        if (*subnode_iter == parent_node) {
            continue;
        }
        cnt_subnode += make_subtree(*subnode_iter, current_node);
    }
    subnode_counter[current_node] = cnt_subnode;
    return cnt_subnode;
}

void push_connection(int u, int v)
{
    connection[u].push_back(v);
    connection[v].push_back(u);
}

int main(void)
{
    int n, r, q;
    int u, v, query;

    scanf("%d %d %d", &n, &r, &q);
    for (int i = 1; i < n; i++) {
        scanf("%d %d", &u, &v);
        push_connection(u, v);
    }
    make_subtree(r, -1);
    
    for (int i = 0; i < q; i++) {
        scanf("%d", &query);
        printf("%d\n", subnode_counter[query]);
    }
}
