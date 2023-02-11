#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      1 <= n <= 10,000    :  노드의 개수
 * 
 *      이어 n - 1 줄에 걸쳐 트리에 대한 정보 a, b, c 가 주어진다.
 * 
 *      a           : 부모 노드
 *      b           : 자식 노드
 *      c <= 100    : 간선의 가중치 
 * 
 *      루트 노트는 항상 1번이다. 이때 트리의 지름을 구하라(누 노드 사이의 가중치 합이 가장 큰 값).
 * 
 * Ideas
 *      트리 구조 이므로 특정 노드에서 서브트리에서 정답인 노드 양쪽이 포함된 경우, 한 쪽만 포함된 경우, 포함 안된 경우.
 * 
 *      세가지 케이스가 있을 것이다. 노드마다 서브트리에서 얻을 수 있는 가장 큰 가중치 합을 가지자.
 *      또한 자식 노드들이 가진 값을 통해 서브트리 내에서 구할 수 있는 트리지름을 구해 ans 를 업데이트 시도하자.
 * 
 * Comments
 *      특이사항은 없었다, 어떤 2 개의 노드를 선택할 때 반드시 한 노드의 서브트리에 속할 수 있다는 점을
 *      이해하고 있었다면 시간을 줄일 수 있었을 것이다
 */

int n;
int ans;
vector<vector<pair<int, int> > > links;

int get_answer(int node)
{
    int prim_dis = 0;
    int second_dis = 0;

    for (vector<pair<int, int> >::iterator link_iter = links[node].begin(); link_iter != links[node].end(); ++link_iter) {
        int distance = get_answer(link_iter->first) + link_iter->second;

        if (distance > prim_dis) {
            second_dis = prim_dis;
            prim_dis = distance;
        } else if (distance > second_dis) {
            second_dis = distance;
        }
    }
    if (prim_dis + second_dis > ans) {
        ans = prim_dis + second_dis;
    }
    return prim_dis;
}

int main(void)
{
    int a, b, c;

    scanf("%d", &n);

    links.assign(n + 1, vector<pair<int, int> >());
    for (int i = 1; i < n; ++i) {
        scanf("%d %d %d", &a, &b, &c);

        links[a].push_back({b, c});
    }
    get_answer(1);
    printf("%d\n", ans);
}