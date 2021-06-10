#include <iostream>
#include <utility>
#include <vector>
#include <queue>

using namespace std;

/**
 * Problem
 *      5 ~ 200 사이의 n 과 m 이 주어진다.
 *      치즈는 1로 공기는 0으로 기록된 2차원배열이 주어진다.
 *      치즈 덩어리 내부의 공기를 제외한 외부의 공기와 2면 이상 접촉할 경우 1시간 뒤 녹는다.
 *      모두 녹아 없어지는 시간을 구하자.
 * 
 * Ideas
 *      0,0 지점으로 부터 bfs를 통해 순회하며 접촉하는 치즈 타일을 마킹한다.
 *      만일 마킹되는 경우가 없으면 종료한다.
 *      1시간을 증가시키고 마킹 횟 수가 2회 이상인 치즈타일을 0으로 돌린다.
 * 
 *      시간을 출력한다.
 * 
 *      필요한 자료구조는 다움과 같을 것이다.
 *      1. 입력을 담을 배열
 *      2. bfs를 돌리는 동안 방문체크, 및 치즈마킹을 위한 2차원 배열
 *      3. 마킹이 없는 경우 탐지를 위한 flag
 *      4. 시간을 저장한 변수
 * 
 *      구현 시작.
 * 
 *      bfs 를 돌리기위한 queue가 필요하다!
 *      사방을 체크하기 위한 x, y 배열이 필요하다!
 * 
 *      segfault 발생.
 *      사방을 체크할 때 arr 규격내부인지 체크 안함.
 *      visited 초기 설정시 미스타이핑
 * 
 * Comments
 *      알고리즘의 실행을 위해 필요한 준비물들에 대해 좀 더 숙지해야겠다.
 *      2차원 vector의 초기 사이즈 설정시 주의해야겠다.
 *      범위 체크 할 시 벗어나는 부분을 항상 염두해어야겠다.
 * 
 *      bfs를 순환하면서 마킹을 했는가를 체크하는 부분이 비효율적이다.
 *      구현을 하며 이부분에 주저하여 시간을 소모했다.
 */

typedef pair<int, int> p;

int n, m;
vector< vector<int> > arr;
vector< vector<int> > visited;
int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

void clear_visit_mark(void)
{
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            visited[i][j] = 0;
        }
    }
}

int solve(void)
{
    queue<p> q;
    int r, c;
    int current_time = 0;
    bool mark_flag = true;

    while (1)
    {
        /* bfs */
        clear_visit_mark();
        q.push({0, 0});
        visited[0][0] = -1;
        while (!q.empty())
        {
            r = q.front().first;
            c = q.front().second;
            q.pop();
            for (int i = 0; i < 4; ++i)
            {
                if (r + dx[i] < 0 || r + dx[i] >= n || c + dy[i] < 0 || c + dy[i] >= m)
                {
                    continue ;
                }
                else if (arr[r + dx[i]][c + dy[i]] == 0 && visited[r + dx[i]][c + dy[i]] == 0)
                {
                    q.push({r + dx[i], c + dy[i]});
                    visited[r + dx[i]][c + dy[i]] = -1;
                }
                else if (arr[r + dx[i]][c + dy[i]] == 1)
                {
                    visited[r + dx[i]][c + dy[i]]++;
                }
            }
        }

        /* 치즈제거 */
        mark_flag = false;
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                if (visited[i][j] > 1)
                {
                    arr[i][j] = 0;
                    mark_flag = true;
                }
            }
        }

        /* 제거 대상 없을 시 탈출 */
        if (mark_flag == false)
            break ;

        current_time++;
    }
    return current_time;
}

int main(void)
{
    scanf("%d %d", &n, &m);
    arr.resize(n);
    visited.resize(n);
    for (int i = 0; i < n; ++i)
    {
        arr[i].resize(m);
        visited[i].resize(m);
        for (int j = 0; j < m; ++j)
        {
            scanf("%d", &(arr[i][j]));
        }
    }
    printf("%d\n", solve());
    return 0;
}