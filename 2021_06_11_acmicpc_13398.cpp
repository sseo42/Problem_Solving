#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * Problem
 *      100000 보다 작거나 같은 n 이 주어진다.
 *      수열은 n 길이이고 절대값이 1000보다 작다.
 *      수열의 연속된 수의 합 중 가장 큰 값을 구하자.
 *      최소 1개 이상을 선택해야하며 연속된 수열 중 1개를 제거할 수 있다.
 * 
 * Ideas
 *      투포인터를 이용한 누적합 구성은 어떤가?
 *      하나의 배열에는 제거권을 사용한 상태(dp_used), 하나의 배열에는 제거권을 사용하지 않은 상태(dp)에서의 값을 누적해 나간다.
 *      dp_used 는 이전의 dp_used 에 현재의 값을 더한것 과 이전의 dp 값 을 비교해 큰수를 넣는다.
 *      dp 는 이전 dp 값에 현재 값을 더한다.
 *      두 경우 모두 갱신할 값이 음수가 되면 0으로 박아넣고 계속진행한다.
 * 
 *      투포인터가 아님 그냥 누적합.
 *      모든 수가 음수일 때를 조심해야겠다.
 * 
 *      필요한 자료구조는 다음과 같을 것이다.
 *      dp_used, dp -> int 1차원 벡터
 *      갱신할 answer
 * 
 *      구현 시작.
 * 
 * Comments
 *      큰 문제는 없없다. 모든 수가 음수을 때를 동작상에서 처리되는지 고민하느라 시간을 좀 썼다.
 *      tmp 를 미리 파악하고 스코프를 맞췄으면 더 좋았을 것이다.
 */

int n;
vector<int> arr, dp, dp_used;

int solution(void)
{
    int answer = -987654321;

    for (int i = 1; i <= n; ++i)
        if (arr[i] > answer)
            answer = arr[i];
    if (answer <= 0)
        return answer;
    dp.resize(n + 1);
    dp_used.resize(n + 1);
    for (int i = 1; i <= n; ++i)
    {
        dp[i] = max(0, dp[i - 1] + arr[i]);
        dp_used[i] = max(dp_used[i - 1] + arr[i], dp[i - 1]);

        int tmp = max(dp[i], dp_used[i]);
        if (tmp > answer)
            answer = tmp;
    }
    return answer;
}

int main(void)
{
    scanf("%d", &n);
    arr.resize(n + 1);
    for (int i = 1; i <= n; ++i)
        scanf("%d", &(arr[i]));
    printf("%d\n", solution());
    return 0;
}