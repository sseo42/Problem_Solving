#include <iostream>
#include <cstring>

using namespace std;

/**
 * Problem
 *      테스트케이스의 수 t 가 주어진다.
 *      1 <= n <= 10,000        : 구역의 수 / 2
 *      1 <= w <= 10,000        : 소대의 부대원 수
 * 
 *      이어 두 줄에 거쳐 n 개의 값이주어진다.
 * 
 *      각 값은 10,000 보다 작거나 같으며, 각 구역의 적 수를 나타낸다.
 * 
 *      각 줄의 값은 이어져 있는 두 링버퍼에서 각 한줄의 구역 순서대로의 적의 수를 의미한다.
 * 
 *      한 구역에 배치되는 부대원은 해당 구역을 커버하며, 인접한 구역 중 하나를 추가로 커버할 수 있다.
 * 
 *      커버하는 모든 구역의 적 수 보다 배치된 부대원의 수가 같거나 많아야한다.
 * 
 *      배치해야되는 소대의 최소 수를 구하자.
 * 
 * Ideas
 *      링을 끊어서 두줄의 배열로 만들 때 끝과 처음이
 * 
 *      a a         a b         a a         a b
 *      b b         c c         b c         c d
 *      (start1)    (start2)    (start3)    (start4)
 * 
 *      네 가지 케이스만 고려하면 링에서의 모든 케이스에 대해 커버할 수 있다.
 * 
 *      x   x   x
 *      x   x   x       ---(a)
 * 
 *      x   x   x   x
 *      x   x   x   x   ---(b)
 * 
 *      
 * 
 *      에서의 최솟값을 알고 있다면 다음은
 * 
 *      x   x   x   a   a       x   x   x   x   a       x   x   x   a   a       x   x   x   x   a
 *      x   x   x   b   b       x   x   x   b   b       x   x   x   x   b       x   x   x   x   b
 * 
 *      (case1)                 (case2)                 (case3)                 (case4)
 * 
 *      의 경우가 있다.
 * 
 *      x   x   x   x
 *      x   x   x   x   x   은 case2, case4 비교로부터 얻을 수 있다 (upper case)
 * 
 *      x   x   x   x   x
 *      x   x   x   x       은 case3, case4 비교로부터 얻을 수 있다 (lower case)
 * 
 *      마직막까지 수행 후
 * 
 *      n 을 fully 하게 채운 것으로 정답으로한다.
 * 
 *      a   a
 *      b   b       를 커버할 조건을 만족한다면, n - 1을 fully 하게 채운것으로 정답을 업데이트 시도한다.
 *      (start1)
 * 
 *      a   b
 *      c   c       를 커버할 조건을 만족한다면, n 의 (upper case) 로 시도
 *      (start2)
 *      
 *      a   a
 *      b   c       를 만족한다면, n의 (lower case)
 *      (start3)
 * 
 * Comments
 *      세부적인 사항에서 간과한 부분이 있다
 * 
 *      첫 인덱스에 위아래로 한개씩 집어넣고 돌리면 위,아래 혹은 두개 모든 걸친 경우가 구분이 안된다
 *      따라서 마지막에 연결을 할 떄 3개가 묶이는 불상사가 발생할 수 있었다.
 *      생각의 깊이가 짧았던것 같다
 * 
 *      DP 를 통해 전개될 때 이전 정보가 사라진다는 사실을 기억할 필요가 있다
 *      또한 최소/최대 연산을 통해 이어질 때 IMPOSSIBLE 을 통해 전개를 막을 수 있다는 것을 명확히 기억한다면
 *      생각에 필요한 자원을 줄일 수 있을 것이다
 */

#define MAX_LEN 10000
#define IMPOSSIBLE 100000

int n, w;
int district[MAX_LEN + 1][2];
int cover_case[MAX_LEN + 1][3];

void fill_cover_case(int idx)
{
    //full cover
    if (district[idx][0] + district[idx][1] <= w) {
        cover_case[idx][0] = cover_case[idx - 1][0] + 1;
    }
    else {
        cover_case[idx][0] = cover_case[idx - 1][0] + 2;
    }
    //upper cover
    cover_case[idx][1] = cover_case[idx - 1][0] + 1;
    //lower cover
    cover_case[idx][2] = cover_case[idx - 1][0] + 1;

    if ((district[idx - 1][0] + district[idx][0] <= w) && (district[idx - 1][1] + district[idx][1] <= w)) {
        cover_case[idx][0] = min(cover_case[idx][0], cover_case[idx - 2][0] + 2);
    }
    if (district[idx - 1][0] + district[idx][0] <= w) {
        cover_case[idx][0] = min(cover_case[idx][0], cover_case[idx - 1][2] + 2);
        cover_case[idx][1] = min(cover_case[idx][1], cover_case[idx - 1][2] + 1);
    }
    if (district[idx - 1][1] + district[idx][1] <= w) {
        cover_case[idx][0] = min(cover_case[idx][0], cover_case[idx - 1][1] + 2);
        cover_case[idx][2] = min(cover_case[idx][2], cover_case[idx - 1][1] + 1);
    }
}

void read_and_initialize_all(void)
{
    scanf("%d %d", &n, &w);

    for (int i = 1; i <= n; ++i) {
        scanf("%d", &(district[i][0]));
    }
    for (int i = 1; i <= n; ++i) {
        scanf("%d", &(district[i][1]));
    }
    memset(cover_case, 0, sizeof(int) * (MAX_LEN + 1) * 3);
}

int solution(void)
{
    int ans = IMPOSSIBLE;

    read_and_initialize_all();

    cover_case[1][1] = 1;
    cover_case[1][2] = 1;
    if (district[1][0] + district[1][1] <= w) {
        cover_case[1][0] = 1;
    }
    else {
        cover_case[1][0] = 2;
    }
    for (int i = 2; i <= n; ++i) {
        fill_cover_case(i);
    }
    ans = cover_case[n][0];

    if (n <= 1) {
        return ans;
    }

    cover_case[0][0] = IMPOSSIBLE;
    cover_case[1][0] = 2;
    if (district[n][0] + district[1][0] <= w && district[n][1] + district[1][1] <= w) {
        cover_case[1][1] = IMPOSSIBLE;
        cover_case[1][2] = IMPOSSIBLE;
        for (int i = 2; i <= n; ++i) {
            fill_cover_case(i);
        }
        ans = min(ans, cover_case[n - 1][0]);
    }
    if (district[n][0] + district[1][0] <= w) {
        cover_case[1][1] = 1;
        cover_case[1][2] = IMPOSSIBLE;
        for (int i = 2; i <= n; ++i) {
            fill_cover_case(i);
        }
        ans = min(ans, cover_case[n][2]);
    }
    if (district[n][1] + district[1][1] <= w) {
        cover_case[1][1] = IMPOSSIBLE;
        cover_case[1][2] = 1;
        for (int i = 2; i <= n; ++i) {
            fill_cover_case(i);
        }
        ans = min(ans, cover_case[n][1]);
    }
    return ans;
}

int main(void)
{
    int t;

    scanf("%d", &t);
    for (int i = 0; i < t; ++i) {
        printf("%d\n", solution());
    }
}