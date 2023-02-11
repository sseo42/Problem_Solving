#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

/**
 * Problem
 *      정점의 수 n[1,1000] 이 주어지고 이어
 *      n x n 행렬이 주어진다.
 *      행렬의 원소[0,10]는 노드간 간선의 수를 의미한다.
 * 
 *      입력으로 주어지는 행렬에는 자기자신으로의 간선이 없으며, 모든 노드는 연결되있다.
 * 
 *      이 떄 오일러 회로의 경로를 출력하자, 시작 위치는 상관없으며 가능한 아무경로나
 *      출력하면 된다. 만약 불가능하다면 -1을 출력하자.
 * 
 * Ideas
 *      오일러 경로, 회로에 대해 봤다. dfs 를 수행해되 지나가는 간선은 제거해주자.
 *      dfs 수행이 불가능하다면 백트래킹하며 자신의 노드 번호를 출력하자.
 * 
 * Comments
 *      지난날에 풀지 못한 문제를 풀었다.
 *      시간복잡도로 인해 매번 for 로 간선을 탐색할 수 는 없었다.
 * 
 *      그렇다고 모든 간선을 들고 있다가 삭제하기에는 메모리가 부담이다.
 *      따라서 한 간선만을 바라보고 있다가 해당 간선이 없어질경우 변경해주도록 했다.
 *      이제 바라보고 있지 않던 간선이 삭제 될 경우에 대해 처리가 필요해 heap 과 stack 을 사용했다.
 * 
 *      자료구조에 대해 좀더 생각해볼 필요가 있다.
 */

#define MAX_NODE 1000

int n;
int board[MAX_NODE + 1][MAX_NODE + 1];
vector<stack<int> > next_of;
vector<vector<int > > removed_edge;

void dfs(int node)
{
    while (next_of[node].empty() == false) {
        if (removed_edge[node].empty() == false && next_of[node].top() <= removed_edge[node][0]) {
            next_of[node].pop();
            pop_heap(removed_edge[node].begin(), removed_edge[node].end());
            removed_edge[node].pop_back();
            continue;
        }
        int next = next_of[node].top();

        --board[node][next];
        --board[next][node];
        if (board[node][next] <= 0) {
            removed_edge[node].push_back(next);
            push_heap(removed_edge[node].begin(), removed_edge[node].end());
            removed_edge[next].push_back(node);
            push_heap(removed_edge[next].begin(), removed_edge[next].end());
        }
        dfs(next);
    }
    printf("%d ", node);
}

int main()
{
    scanf("%d\n", &n);
    next_of.resize(n + 1);
    removed_edge.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        int edge_cnt = 0;

        for (int j = 1; j <= n; ++j) {
            scanf("%d", board[i] + j);
            edge_cnt += board[i][j];
            if (board[i][j] > 0) {
                next_of[i].push(j);
            }
        }
        if (edge_cnt % 2 != 0) {
            printf("-1\n");
            return 0;
        }
    }
    dfs(1);
}