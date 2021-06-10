#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      3 ~ 50 인 n, m 과 6 이하의 k 가 주어진다.
 *      n x m 의 배열이 주어진다.
 *      회전변환은 r, c 를 중점으로 상하좌우 k 이내를 사각형으로 회전한다.
 *      회전변환을 임의의 순서대로 1번씩 모두 사용할 때 최소값(각 행의 합 중 최소값)을 구하라.
 * 
 * Ideas
 *      k가 6 이하이므로 2^6 = 64, 모든 경우에 대해 수행한다면 회전변환(50 * 50) * 64 * 합계산(50 * 50) 내로 해결 가능.(시간제한?)
 *      재귀함수로써 구성한다.
 *      로테이션은 함수로 분리한다.
 *      합계산도 함수로 분리한다.
 * 
 *      다음과 같은 자로구조가 필요할 것이다.
 *      배열을 담을 벡터.
 *      회전변환정보를 기록할 벡터.
 *      방문체크를 할 int 형(비트마스킹)
 *      
 *      구현 시작.
 * 
 *      6! * ((50 * 50) + (50 * 50)) 임. 시간계산 잘못함.
 *      배열을 복사해 진행해야함. 백트레킹은 시간이 비효율적임. 720 * 2500 감당 가능.
 * 
 * Comments
 *      첫트 실패. 로테이션 함수에서 tmp 를 사용하지 않고 배열 위치로 접근함. 집중할 것!
 *      시간계산복잡도 계산이 미숙함.
 *      백트래킹으로 해도 좋았을 것이다. 결국 복사과정은 같은 시간이고, 오히려 메모리를 좀 더 타이트하게 쓸 수 있었을 것이다.
 */

typedef pair<int, pair<int, int> > p3;

int n, m, k;
vector<p3> rotation;
int answer = 987654321;

void rotate(vector<vector<int> > &arr, int s, int r, int c)
{
    if (s > 0)
    {
        int tmp = arr[r - s][c - s];
        for (int i = r - s; i < r + s; ++i)
            arr[i][c - s] = arr[i + 1][c - s];
        for (int i = c - s; i < c + s; ++i)
            arr[r + s][i] = arr[r + s][i + 1];
        for (int i = r + s; i > r - s; --i)
            arr[i][c + s] = arr[i - 1][c + s];
        for (int i = c + s; i > c - s; --i)
            arr[r - s][i] = arr[r - s][i - 1];
        arr[r - s][c - s + 1] = tmp;
        rotate(arr, s - 1, r, c);
    }
}

int get_minval(vector<vector<int> > &arr)
{
    int out = 987654321;
    int tmp;

    for (int i = 0; i < n; ++i)
    {
        tmp =  0;
        for (int j = 0; j < m; ++j)
            tmp += arr[i][j];
        if (tmp < out)
            out = tmp;
    }
    return out;
}

void solution(vector<vector<int> > &arr, int mask)
{
    if (mask == 0)
    {
        int tmp = get_minval(arr);
        if (tmp < answer)
            answer = tmp;
    }
    else
    {
        vector<vector<int> > new_arr;

        new_arr.resize(n);
        for (int i = 0; i < n; ++i)
            new_arr[i].resize(m);
        for (int i = 0; i < k; ++i)
        {
            if ((1<<i) & mask)
            {
                for (int ii = 0; ii < n; ++ii)
                    for (int jj = 0; jj < m; ++jj)
                        new_arr[ii][jj] = arr[ii][jj];
                rotate(new_arr, rotation[i].first, rotation[i].second.first, rotation[i].second.second);
                solution(new_arr, mask - (1<<i));
            }
        }
    }
}

int main(void)
{
    int r, c, s;
    vector<vector<int> > arr;

    scanf("%d %d %d", &n, &m, &k);
    arr.resize(n);
    for (int i = 0; i < n; ++i)
    {
        arr[i].resize(m);
        for (int j = 0; j < m; ++j)
            scanf("%d", &(arr[i][j]));
    }
    rotation.resize(k);
    for (int i = 0; i < k; ++i)
    {
        scanf("%d %d %d", &r, &c, &s);
        rotation[i].first = s;
        rotation[i].second.first = r - 1;
        rotation[i].second.second = c - 1;
    }
    solution(arr, (1<<k) - 1);
    printf("%d\n", answer);
    return 0;
}