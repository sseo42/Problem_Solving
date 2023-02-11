#include <iostream>
#include <queue>

using namespace std;


/**
 * Problem
 *      간단한 탐색 문제.
 * 
 * Comments
 *      하루 1문제 떄문에 쉬운거로 급하게 풀었다.
 *      작성을 마친 뒤 visited 를 초기화 하지 않았다는 것과 최초 1번 노드의 visited 를 체크 안했다는 걸 깨닳고 수정했다.
 * 
 *      초기화 매우 중요하다 또한 visit 체크를 까먹지 말자, 이해가 낮다는 방증이다.
 * 
 */
#define MAX_NODE 100

int visited[MAX_NODE + 1];

int main()
{
    int n, link_count;
    vector<vector<int> > links;
    queue<int> bfs_q;
    int a, b;
    int ans = -1;

    scanf("%d\n%d", &n, &link_count);
    links.resize(n + 1);

    for (int i = 1; i <= link_count; ++i) {
        scanf("%d %d", &a, &b);
        links[a].push_back(b);
        links[b].push_back(a);
    }

    visited[1] = true;
    bfs_q.push(1);
    while (bfs_q.empty() == false) {
        a = bfs_q.front();
        bfs_q.pop();
        ++ans;

        for (vector<int>::iterator next_node = links[a].begin(); next_node != links[a].end(); ++next_node) {
            if (visited[*next_node] == false) {
                visited[*next_node] = true;

                bfs_q.push(*next_node);
            }
        }
    }
    printf("%d\n", ans);
}