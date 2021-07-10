#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>

using namespace std;

/**
 * Problem
 *      N x M 크기의 2차원 배열이 주어진다.
 *      (N x M)은 100 이하이다.
 *      배열은 1 과 0 으로 구성되 있으며, 1은 섬 0은 바다를 의미한다.
 * 
 *      0 위에 가로 혹은 세로의 길을 통해 모든 섬을 연결하는 최소 길이를 구하자.
 *      불가능한 경우 -1 을 출력한다.
 * 
 *      길은 서로 겹칠 수 있으며 A 와 B 를 이어주는 길이 C 에 접한다 해서 C 가
 *      연결되는 것은 아니다. 길의 최소 길이는 2 이다.
 * 
 * Ideas
 *      섬들을 djs 으로 구성하고 최소 비용 신장트리로 구하자.
 *      프림이냐 크루스칼이냐..
 *      한 섬에서 진행하며 체크하는 것보다 그래프를 구성하고 다루는 편이 더 깔끔할 것이다.
 *      시간복잡도는 동일하므로 크루스칼알고리즘을 적용해 풀자.
 * 
 *      섬의 집합을 구성할 map 자료구조. -> bfs를 통해 구성
 *      섬의 번호를 매길 전역변수
 *      섬간의 사이클을 체크할 djs.
 *      섬간의 거리를 기록할 2차원 배열.
 *      짧은 거리 순으로 나열될  priority_queue.
 * 
 *      구현 시작.
 * 
 *      map 자료구조가 필요없다 -> arr 바로 기록하면 됨.
 *      방문체크 없이 dfs를 돌음 -> 무한루프
 * 
 * Comments
 *      프림알고리즘과 크루스칼 알고리즘의 시간복잡도는 엄연히 다르다.
 *      문제와 혼용해 생각하지 말자.
 *      bfs로 구성하지 않고 간단하게 dfs 구성했다. 사이즈가 작다면 별도의 객체를 선언하지 말자.
 *      순회에서 방문체크를 잊지말것.
 * 
 *      구현에 시간이 오래 걸렸다. 처리해야할 과정을 명확히 나누고 함수화하자.
 *      생각을 재빠르게 정리하는 법과 사소한 부분을 정형화된 패턴으로 넘기는 것이 중요하다 생각된다.
 */

# define DISCOVER_DEFAULT 2
# define IMPOSSIBLE 987654321

typedef pair<int, int> p;
typedef pair<int, p> p3;

int N, M;
int discover = DISCOVER_DEFAULT;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, -1, 0, 1};
vector<vector<int> > arr;
vector<p> islands;
vector<vector<int> > dist;
vector<int> djs;
priority_queue<p3, vector<p3>, greater<p3> > pq;

void mark_island(int r, int c)
{
    arr[r][c] = discover;
    islands.push_back({r, c});
    for (int i = 0; i < 4; ++i)
    {
        if (r + dx[i] < 0 || r + dx[i] >= N || c + dy[i] < 0 || c + dy[i] >= M)
            continue ;
        if (arr[r + dx[i]][c + dy[i]] == 1)
            mark_island(r + dx[i], c + dy[i]);
    }
}

void find_other_island(int r, int c, int dr, int dc)
{
    int distance = 0;
    int my_island = arr[r][c];

    if (dr == 0 && dc == 0)
    {
        printf("wrong!");
    }
    while (1)
    {
        r += dr;
        c += dc;
        if (r < 0 || r >= N || c < 0 || c >= M || my_island == arr[r][c])
            break ;
        if (arr[r][c] > 0)
        {
            if (distance > 1)
            {
                if (my_island < arr[r][c] && distance < dist[my_island][arr[r][c]])
                    dist[my_island][arr[r][c]] = distance;
                else if (my_island > arr[r][c] && distance < dist[arr[r][c]][my_island])
                    dist[arr[r][c]][my_island] = distance;
            }
            break ;
        }
        distance++;
    }
}

void update_distance()
{
    int r, c;

    for (vector<p>::iterator it = islands.begin(); it != islands.end(); ++it)
    {
        r = it->first;
        c = it->second;
        for (int i = 0; i < 4; ++i)
        {
            find_other_island(r, c, dx[i], dy[i]);
        }
    }
}

int get_root(int a)
{
    if (djs[a] == a)
        return a;
    djs[a] = get_root(djs[a]);
    return djs[a];
}

int check_djs(int a, int b)
{
    int a_root = get_root(a), b_root = get_root(b);

    if (a_root == b_root)
        return 1;
    djs[a_root] = b_root;
    return 0;
}

int solution(void)
{
    int ret = 0;

    update_distance();
    for (int i = DISCOVER_DEFAULT; i < discover; ++i)
    {
        for (int j = i; j < discover; ++j)
        {
            if (dist[i][j] != IMPOSSIBLE)
                pq.push({dist[i][j], {i, j}});
        }
    }
    while (!pq.empty())
    {
        if (!check_djs(pq.top().second.first, pq.top().second.second))
            ret += pq.top().first;
        pq.pop();
    }
    for (int i = DISCOVER_DEFAULT; i < discover; ++i)
    {
        for (int j = i; j < discover; ++j)
        {
            if (!check_djs(i, j))
                return -1;
        }
    }
    return ret;
}

int main(void)
{
    scanf("%d %d", &N, &M);
    arr.resize(N);
    for (int i = 0; i < N; ++i)
    {
        arr[i].resize(M);
        for (int j = 0; j < M; ++j)
        {
            scanf("%d", &(arr[i][j]));
        }
    }
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < M; ++j)
        {
            if (arr[i][j] == 1)
            {
                mark_island(i, j);
                discover++;
            }
        }
    }
    dist.resize(discover);
    for (int i = 0; i < discover; ++i)
        dist[i].resize(discover, IMPOSSIBLE);
    djs.resize(discover);
    for (int i = 0; i < discover; ++i)
        djs[i] = i;
    printf("%d\n", solution());
    return 0;
}