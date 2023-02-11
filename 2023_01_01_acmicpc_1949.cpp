#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      마을의 수와 마을의 주민수가 주어진다. 마을들은 트리구조로 연결되있다.
 *      마을을 마킹할 때 마킹된 주민 수가 최대가 되게하자.
 *      인접한 마을(간선으로 연결된) 은 마킹할 수 없다.
 *      자기 자신 혹은 인접한 마을이 마킹되야한다.
 * 
 *      1 <= N <= 10,000        : 마을의 수
 *      0 <= citizen <= 10,000  : 마을의 주민수
 * 
 * Ideas
 *      특정 노드의 서브트리에서 최대값을 구하는데 필요한 정보는
 *      자식노드들의 (자식노드의 마킹을 포함하는 최대값) (자식의 마킹을 포함하지 않는 최대값) 이다.
 * 
 *      자신을 마킹을 안하는 경우는 자식의 합이다(max[마킹/non 마킹]).
 *      자신을 마킹하는 경우는 자식의 non 마킹합 + 자신이다.
 * 
 *      이는 인접한 마을이 마킹되있음이 보장된다.
 * 
 *      간선을 받아 양방향으로 구성한다음 1번 노드를 루트로 두고 탐색하면서 업데이트한다.
 * 
 * Comments
 *      특이사항은 없었다, 선택에 속도를 올리면 좋을 거 같다. 경험과 회고가 바탕이 되야할 둣?
 */

#define MAX_TOWN 10000

vector<int> citizen(MAX_TOWN + 1);
vector<vector<int> > links(MAX_TOWN + 1);
vector<pair<int, int> > citizen_sum(MAX_TOWN + 1);

void set_link(int a, int b)
{
    links[a].push_back(b);
    links[b].push_back(a);
}

void update_citizen_sum(int town, int parent)
{
    int sum_with_mark = 0, sum_without_mark = 0;

    for (vector<int>::iterator linked_town_iter = links[town].begin(); linked_town_iter != links[town].end(); ++linked_town_iter) {
        if (*linked_town_iter == parent) {
            continue;
        }
        update_citizen_sum(*linked_town_iter, town);

        sum_without_mark += citizen_sum[*linked_town_iter].first;
        sum_with_mark += citizen_sum[*linked_town_iter].second;
    }
    sum_with_mark = max(sum_with_mark + citizen[town], sum_without_mark);

    citizen_sum[town].first = sum_with_mark;
    citizen_sum[town].second = sum_without_mark;
}

int main(void)
{
    int n;
    int a, b;

    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        scanf("%d", &a);
        citizen[i] = a;
    }
    for (int i = 1; i < n; i++) {
        scanf("%d %d", &a, &b);
        set_link(a, b);
    }
    update_citizen_sum(1, -1);
    printf("%d\n", citizen_sum[1].first);
}