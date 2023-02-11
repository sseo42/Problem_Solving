#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      2 <= v <= 100,000       : 정점의 수
 *      
 *      V 줄에 걸쳐서 간선의 정보가 주어진다.
 * 
 *      a b1 b2 c1 c2 ..... z1 z1 ... -1 의 형식으로 주며, a 노드에서 x1 노드까지 링크의 거리가 x2 임을 의미한다.
 *      마지막에는 -1 이 주어진다. 거리는 10,000 이하의 자연수이다.
 * 
 *      트리의 지름을 구하자.
 * 
 * Ideas
 *      트리의 지름, 간단한 유형이다.
 * 
 *      밑에서 부터 자신으로부터 가장 긴 거리를 저장하고, 조회마다 가장 긴거리 2 개를 합해서 답을 업데이트 시도한다.
 * 
 * Comments
 *      실수가 굉장히 많았다.
 * 
 *      1. 제출 전 visited 체크를 안 해서 양방향 무한 루프로 터짐.
 * 
 *      2. 문제를 제대로 안 읽은 듯... 노드는 순서대로 주어지지 않을 수 있는데, 테케만 봐서 그런지 무시해버렸다.
 * 
 *      3. 최대값을 비교해서 크면 업데이트 했는데, 이거로는 최대값 2개 구하는게 아니다....
 * 
 *      이전 1967 을 풀었을 떄는 1, 2 함정이 없었긴했지만 3번을 잘못한 것은 반성해야될 부분이다...
 */

int n;
vector<vector<pair<int, int> > > links;
vector<int> max_distance;
vector<bool> visited;

int distance_of_tree(int node)
{
    int max_distance_of_sub_tree = 0;
    int prime_distance = 0;
    int secondary_distance = 0;

    visited[node] = true;
    for (vector<pair<int, int> >::iterator child = links[node].begin(); child != links[node].end(); ++child) {
        if (visited[child->first] == false) {
            int distance_of_sub_tree = distance_of_tree(child->first);
            int current_distance = max_distance[child->first] + child->second;
        
            if (distance_of_sub_tree > max_distance_of_sub_tree) {
                max_distance_of_sub_tree = distance_of_sub_tree;
            }
            
            if (current_distance > secondary_distance) {
                if (current_distance < prime_distance) {
                    secondary_distance = current_distance;
                } else {
                    secondary_distance = prime_distance;
                    prime_distance = current_distance;
                }
            }
        }
    }
    max_distance[node] = prime_distance;
    return (max_distance_of_sub_tree > prime_distance + secondary_distance) ? max_distance_of_sub_tree : (prime_distance + secondary_distance);
}

int main()
{
    int from_node, to_node, dist;

    scanf("%d", &n);

    links.resize(n + 1);
    max_distance.resize(n + 1);
    visited.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &from_node);

        scanf("%d", &to_node);
        while (to_node != -1) {
            scanf("%d", &dist);

            links[from_node].push_back({to_node, dist});
            scanf("%d", &to_node);
        }
    }
    printf("%d\n", distance_of_tree(1));
}