#include <iostream>
#include <cmath>

using namespace std;

/**
 * Problem
 *      a와 b가 주어진다.(a < b)
 *      a < number <= b 사이의 수 중 약수의 개수가 홀수인 수의 개수(n이라 하자)를 구하고
 *      이를 n / (b - a) 기약분수 형태로 출력하라.
 * 
 * Ideas
 *      2 x 3 x 5 = 30 -> 1, 2, 3, 5, 6, 10, 15, 30
 *      6 x 6 = 36 -> 1, 2, 3, 4, 6, 9, 12, 18, 36
 *      약수를 작은 수 부터 나열 할 때 자신의 제곱으로 해당 수를 만드는 값이 아닌
 *      다른 값들은 매칭되는 값(곱하여 해당 수를 구성하는 값)을 가지므로
 *      제곱수만이 홀수개의 약수를 가진다.
 *      0 <= a < b <= 2^60 이므로 long long 을 사용하고
 *      a 와 b의 제곱근을 구해 값의 범위를 구한 다음 최대 공약수를 구해 기약분수 꼴을 취하자.
 * 
 * 
 * Comments
 *      출력 양식과 범위와 값의 크기만 조심하면 크게 문제될 게 없는 문제이다.
 */

long long gcd(long long a, long long b)
{
    long long c;

    while (b != 0) {
        c = a % b;
        a = b;
        b = c;
    }
    return a;
}

int main(void)
{
    long long a, b;
    long long numerator, denominator, gcd_val;

    scanf("%lld %lld", &a, &b);
    numerator = (long long)floor(sqrt(b)) - (long long)ceil(sqrt(a + 1)) + 1;
    denominator = b - a;
    if (numerator == 0) {
        printf("0\n");
    }
    else {
        gcd_val = gcd(denominator, numerator);
        printf("%lld/%lld\n", numerator / gcd_val, denominator / gcd_val);
    }
    return 0;
}