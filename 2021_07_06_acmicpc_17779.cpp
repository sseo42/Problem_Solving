#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * Problem
 *      2차원 배열을 5개의 구역으로 나눈다.
 *      구역 내의 원소의 합의 차가 최소가 되도록 하자.
 *      구역은 4 의 포인트로 구분된다.
 *      A: (r, c)
 *      B: (r - p1, c + p1)
 *      C: (r - p1 + p2, c + p1 + p2)
 *      D: (r + p2, c + p2)
 * 
 *      A-B-C-D 의 포인트를 포함하는 사각형,
 *      해당 영역을 제외한,
 *      (row < r) and (column <= c + p1)
 *      (row <= r - p1 + p2) and (column > c + p1)
 *      (row > r - p1 + p2) and (column >= c + p2)
 *      (row >= r) and (column < c + p2)
 * 
 *      배열의 크기 N 은 20 이하
 *      원소의 값은 1 이상 100 이하이다.
 * 
 * Ideas
 *      모든 경우를 따져도 시간이 넘치지 않을까?
 *      임의의 한점 (r, c) 를 잡았을 때, p1 + p2 는 20을 넘지 않는다.
 *      따라서 20 * 20 * 10 * 10 이하의 경우의 수가 존재한다.
 *      각 경우 마다 모든 배열의 원소를 탐색한다면
 *      20 * 20 * 10 * 10 * 400 이하, 1 초 안에 가능하다.
 * 
 *      프리픽스섬으로 가능할 것 같다.
 *      -> 복잡해짐 포기
 * 
 *      필요한 자료구조는 다음과 같을 것이다.
 * 
 *      배열을 저장할 arr, 다이아몬드 구간을 기록할 visited.
 * 
 *      구현 시작.
 * 
 * Comments
 *      구간 순회에서 포함지점과 구간 설정에서 시간을 많이 소모했고,
 *      구간을 잘못 잡아 이를 디버깅하는데 시간을 소모했다.
 *      정형화된 구간순회의 포맷을 설정하는 것이 바람직할 것 같다.
 */

vector<vector<int> > arr;
vector<vector<bool> > visited;
int N;
int total_val;

int get_val(int r, int c, int p1, int p2)
{
    int max_val, min_val;
    int section1 = 0, section2 = 0, section3 = 0, section4 = 0, section5 = 0;

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            visited[i][j] = false;

    for (int i = 1; i <= p1; ++i)
    {
        visited[r - i][c + i] = true;
        visited[r - p1 + p2 + i][c + p1 + p2 - i] = true;
    }
    for (int i = 1; i <= p2; ++i)
    {
        visited[r - p1 + i][c + p1 + i] = true;
        visited[r + p2 - i][c + p2 - i] = true;
    }

    for (int i = 0; i < r; ++i)
    {
        for (int j = 0; j <= c + p1; ++j)
        {
            if (visited[i][j] == true)
                break ;
            section1 += arr[i][j];
        }
    }
    for (int i = N - 1; i > p1 + c; --i)
    {
        for (int j = 0; j <= r - p1 + p2; ++j)
        {
            if (visited[j][i] == true)
                break ;
            section2 += arr[j][i];
        }
    }
    for (int i = N - 1; i > r + p2 - p1; --i)
    {
        for (int j = N - 1; j >= c + p2; --j)
        {
            if (visited[i][j] == true)
                break ;
            section3 += arr[i][j];
        }
    }
    for (int i = 0; i < c + p2; ++i)
    {
        for (int j = N - 1; j >= r; --j)
        {
            if (visited[j][i] == true)
                break ;
            section4 += arr[j][i];
        }
    }
    section5 = total_val - section1 - section2 - section3 - section4;
    max_val = max(section1, max(section2, max(section3, max(section4, section5))));
    min_val = min(section1, min(section2, min(section3, min(section4, section5))));

    return max_val - min_val;
}

int solution()
{
    int val, min_val = 987654321;

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            for (int p1 = 1; p1 < N; ++p1)
            {
                for (int p2 = 1; p2 < N; ++p2)
                {
                    if (i - p1 < 0 || i + p2 >= N || j + p1 + p2 >= N)
                        continue ;
                    val = get_val(i, j, p1, p2);
                    if (val < min_val)
                        min_val = val;
                }
            }
        }
    }
    return min_val;
}

int main(void)
{
    scanf("%d", &N);
    arr.resize(N);
    visited.resize(N);
    for (int i = 0; i < N; ++i)
    {
        arr[i].resize(N);
        visited[i].resize(N);
        for (int j = 0; j < N; ++j)
        {
            scanf("%d", &(arr[i][j]));
            total_val += arr[i][j];
        }
    }
    printf("%d\n", solution());
}