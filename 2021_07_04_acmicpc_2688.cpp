#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * Problem
 *      입력은 처음 테스트케이스의 개수 T 와 이어지는 T개의 숫자 n으로 이루어져있다.
 *      줄어들지 않는 n 자리 수의 개수를 구하자.
 * 
 * Ideas
 *      0~9 의 dp 를 구성한다음 제일 앞부분부터 갱신해 나간다.
 *      예로써 5의 값은 이전 dp 의 0~5 구간의 합이다.
 * 
 *      필요한 변수는 다음과 같을 것이다.
 *      unsigned long long[10], n, T
 * 
 *      값은 long long 을 넘지 않을 것인가?
 *      일단 unsigned long long 으로 가져가자.
 *      
 *      2차원 dp 테이블을 이용해 저장한다면 T 의 반복을 줄일 수 있다!
 * 
 *      구현 시작.
 * 
 * Comments
 *      dp 테이블에서 인덱스의 의미를 조정하는 부분에서 시간을 잡아먹었다.
 *      조금더 코드가 늘어나더라도 통일된 작성방식을 가진다면 좋을 것이다.
 * 
 *      반복적인 계산을 하는 문제에서는 매번 계산을 하지 않고 이전의 결과를 써먹을
 *      수 있는지 미리 체크해보는 것이 좋겠다.
 */

vector<vector<unsigned long long> > dp;
vector<int> test_case;

void solution(int n)
{
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= 10; ++j)
            dp[i][j] = dp[i][j - 1] + dp[i - 1][j];
}

int main(void)
{
    int T, max_val = -1;

    scanf("%d", &T);
    test_case.resize(T);
    for (int i = 0; i < T; ++i)
    {
        scanf("%d", &(test_case[i]));
        if (max_val < test_case[i])
            max_val = test_case[i];
    }
    dp.resize(max_val + 1);
    for (int i = 0; i <= max_val; ++i)
        dp[i].resize(11);
    for (int i = 1; i <= 10; ++i)
        dp[0][i] = 1;
    solution(max_val);
    for (int i = 0; i < T; ++i)
        printf("%llu\n", dp[test_case[i]][10]);
    return 0;
}