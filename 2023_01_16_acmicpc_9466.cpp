#include <iostream>
#include <stack>
#include <cstring>

using namespace std;

/**
 * Problem
 *      2 <= n <= 100,000   : 노드의 수
 * 
 *      특정 노드로 부터 나가는 간선은 1개 뿐이다.
 *      노드는 자기 자신으로 간선을 가질 수 있다.
 * 
 *      테스트 케이스가 주어지고 각 노드의 수 이어 각 노드로부터 출발하는 간선이 주어진다.
 * 
 *      싸이클이 없는 노드의 수를 구하라.
 * 
 * Ideas
 *      대놓고 사이클 문제이다.
 * 
 *      타잔 쓸거도 없다, 오히려 쓰면 지저분한 if 절 넣어야될 둣.
 * 
 *      visited 체크하고 간선타는 루프에서 이미 vistied 된거 발견하면 이거 사이클이다.
 *      자기 자신으로 간선에 대해서도 성립된다. 리턴으로 발견한 번호 돌려주자.
 * 
 *      리턴 받은게 자신과 같으면 스택에서 터트린다.
 *      어디 까지? 스택에서 탐지한 번호까지.
 * 
 *      리턴 받은게 없다? 이건 사이클아니다.
 * 
 * Comments
 *      처음 아이디어대로 문제를 풀었다. 중간 제출 직전에 finished 체크를 넣었다, 이거 추가 안 했으면 이미 사이클 체크된 곳
 *      으로 넘어갔을 떄 discovey 체크되서 사이클로 판단 됬을 것이다.
 * 
 *      이후 다른사람의 풀이를 보고 stack 을 사용할 필요가 없음을 알았다
 *      discovery 에 번호만 메기면 dfs 로 탐지됬을 때 바로 업데이트 가능하다.
 *      (처음 아이디어에서는 bool 체크만 메겼음)
 * 
 *      더불어 사이클 키고 루프로 다시 카운팅하는 것도 비효율적이였다.
 *      <다시 제출했을 때 시간차는 미비했는데, io가 상당한 비중을 차지해서 그런듯하다>
 * 
 *      알고리즘만 훔쳐봐서 알고있지 이해는 상당히 낮은듯하다.
 * 
 *      제대로 아는 것이 중요하다...
 */

#define MAX_NODE 100000
#define INITIAL_VAL 0

int discovery_counter;
int cycle_node_amount;
int next_node_of[MAX_NODE + 1];
int discovery[MAX_NODE + 1];
bool finished[MAX_NODE + 1];

void dfs(int node)
{
    discovery[node] = ++discovery_counter;
    if (discovery[next_node_of[node]] == INITIAL_VAL) {
        dfs(next_node_of[node]);
    } else if (finished[next_node_of[node]] == false) {
        cycle_node_amount += discovery[node] - discovery[next_node_of[node]] + 1;
    }
    finished[node] = true;
}

void solution(int n)
{
    for (int i = 1; i <= n; ++i) {
        if (finished[i] == false) {
            dfs(i);
        }
    }
}

int main()
{
    int test_case;
    int n;

    scanf("%d", &test_case);
    for (int i = 0; i < test_case; ++i) {
        scanf("%d", &n);
        
        discovery_counter = INITIAL_VAL;
        cycle_node_amount = INITIAL_VAL;
        memset(discovery, 0, sizeof(int) * (n + 1));
        memset(finished, 0, sizeof(bool) * (n + 1));
        for (int j = 1; j <= n; ++j) {
            scanf("%d", next_node_of + j);
        }
        solution(n);
        printf("%d\n", n - cycle_node_amount);
    }
}