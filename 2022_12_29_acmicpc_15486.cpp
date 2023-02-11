#include <iostream>

using namespace std;

/**
 * Problem
 *      1 <= N <= 1,500,000 이 주어진다. N은 업무를 수행할 수 있는 일수 이다.
 *      이후 N 개의 업무가 주어진다. 업무는 cost 와 payment 로 구성되있고 
 *      cost 는 업무에 걸리는 일수 payment 를 결과로 얻는 보상이다.
 * 
 *      1 <= cost <= 50
 *      1 <= payment <= 1000
 * 
 *      N 일 동안 얻을 수 있는 최대 payment 를 구하자.
 * 
 * Ideas
 *      1,500,050 짜리 배열을 구성하자.
 * 
 *      1일에 적혀있는 기대값보다 0일에 적힌 기대값이 더 크다면 복사한다.
 *      0일까지 업무 payment + 1일의 기대값을 [1 + cost - 1](a) 의 값과 비교해 큰값으로 업데이트 한다.
 * 
 *      a 에서 최댓값이 1일 포함하지 않는 것이라면 이는 뒤에서 업데이트 하는 것으로 덮어써질 것이다.
 *      또한 a일의 합이 a + 1 일에 적혀있는 것보다 이득이라면 a + 1 일에서 복사해 갈 것이므로 정보가 유지된다.
 * 
 *      + int 로 계산해도 괜춘함.
 * 
 * Comments
 *      낫베드, 하지만 뒤에서 접근하면 payment를 적는 일이 필요없어진다.
 */

#define MAX_DAY 1500000
#define MAX_COST 50

int dp[MAX_DAY + MAX_COST];

int main(void)
{
    int n = 0;
    int cost, payment = 0;

    scanf("%d", &n);
    for (int i = 1 ; i <= n; i++) {
        scanf("%d %d", &cost, &payment);

        if (dp[i] < dp[i - 1]) {
            dp[i] = dp[i - 1];
        }
        if (dp[i - 1] + payment > dp[i + cost - 1]) {
            dp[i + cost - 1] = dp[i - 1] + payment;
        }
    }
    printf("%d\n", dp[n]);
    return 0;
}