#include <iostream>

using namespace std;

/**
 * Problem
 *      서로 다른 정수로 이루어진 집합이 주어진다.
 *      이 집합의 순열을 합쳐 하나의 큰 정수를 만들떄 K 로 나눠 떨어지는 경우의 수를 구하자.
 * 
 *      집합의 수 n[1, 15]이 주어지며, 이어 n 줄에 걸쳐 각 수가 주어진다. 각 수의 길이는 50 이하이다.
 *      이후 k[1,100] 이 주어진다.
 * 
 *      경우의 수는 p/q 꼴의 기약분수로 출력하며 정답이 0 인 경우 0/1, 1인 경우는 1/1 로 출력한다.
 * 
 * Ideas
 *      문제가 정확히 이해되지 않으므로 예시 1번을 보자
 *      3 2 1   3 1 2   2 1 3   2 3 1   1 2 3   1 3 2
 * 
 *      2로 나눠지는 경우는 2/6.
 * 
 *      힌트로 dp 와 비트마스킹임을 보았다.
 * 
 *      n 번 수까지의 집합에서 순열을 구하는 것은 n-1 번째 까지의 집합에서의 순열에서 각 자리에 n 번쨰수를
 *      넣는 것으로 찾을 수있다.
 * 
 *      나머지의 값과 집합에서 선택된 곳들을 배열로 해서 그 수를 저장한다면?
 * 
 *      예로 1011 을 구할 때 어디를 참조해야되는가? (1000 & 0011) + (0010 & 1001) + (0001 & 1010) 하면된다.
 *      하나의 수를 여러자리에 끼워 넣는게 아니라 하나의 수씩 앞으로 놓고 나머지를 가져다 쓰면 된다...!
 * 
 *      이렇게 하면 앞뒤의 조합을 신경쓰지 않아도 된다. 하나의 수를 뒤에 놓자. 이게 나머지 계산하기 더 쉽다.
 * 
 *      현재 수의 길이에 따라 앞의 나머지에 곱할게 달라지므로 수마다 길이를 저장할 배열을 마련하자.
 * 
 *      시간복잡도는 15 * 100 * 2^15 이다.
 * 
 * Comments
 *      문제에 대한 접근은 맞았다. 하지만 구현에 있어서 문제가 있었다.
 * 
 *      해당 문제를 풀기위해서는 dp 와 gcd, 큰수의 remainder 연산이 필요하다.
 * 
 *      처음 풀이에서 value 를 받을 떄 해당 값의 remainder 를 구해 dp 에 저장하고 해당 수의 나머지 remainder 는
 *      0로 두었다.
 *      (여기서 큰 수의 remainder 를 구하기 위해 long long 3개를 struct 로 묶어 high / mid / low 파트별 재연산을 했다)      [A]
 *      
 *      이 후, 모든 수를 사용했을 때를 구하려했다. 만약 dp 에 값이 갱신되지 않았다면                                         [B]
 *      loop 돌며 xor 를 통해 한개를 제거한뒤 재귀적 업데이트를 시도한 뒤
 * 
 *      xor 한 값의 모든 remainder[0,99] 를 돌며 현재의 string 과 더해 다시 remainder 를 구했다.                            [C]
 *      (최초에는 원하는 remainder 를 집어주고자 했으나 xor 한 녀석이 어떤 remainder 를 가질지 알 수 가 없어, 
 *       remainder 를 돌도록 수정했다)
 * 
 *      이후 모든 수를 썻을 때 remainder 를 합쳐 분모를 구성하고 gcd 를 통해 나눠 출력했다.
 * 
 *      이를 작성하면서 ^ 를 * 로 잘못쓰는 typo 랑 low 를 계산할 때 number.low 가 아닌 number.mid 를 사용한 것으로 인해 
 *      디버깅에 상당한 시간이 소요됬다. 또한 (1<<n) - 1 이 아닌 1<<n - 1 등으로 작성하여 이를 찾는데 시간이 걸렸다.            [D]
 * 
 *      제출 후 OutOfBounds 를 맞았는데 MAX_CASE 를 (1<<MAX_N) - 1 으로 두고 이를 그대로 dp 배열 크기로 사용해 문제였다.
 *      
 *      이를 수정한 뒤 OutOfRange 를 받았는데, [A] 의 연산에서 18/16/16 을 생각했으나 [C] 에서 string 으로 더하면 high 가 20    [E]
 *      까지 올라갈 수 있으므로 lld 범위를 벗어날 수 있었다.
 * 
 *      이를 수정한 뒤 틀렸습니다를 받았다. 당시에는 원인을 못찾아 다른사람 코드를 참조해 [A] 를 뜯어고쳤는데
 *      (이 과정에서 typo 로 또 고생) 다시 확인해보니 추가한 field 의 초기화가 없어 그랬던 것으로 추정된다.
 * 
 *      여튼 수정 후 시간초과를 받았다. [C] 에서의 비효율 떄문, 시간복잡도가 2^15 * 100 * 15 * 100 이 되버린다..
 *      마찬가지로 다른사람 코드를 참조해 [C] 를 수정했다.
 * 
 *      다시 틀렸습니다를 받았다, 수정한 로직에서 p10 이라는 배열의 구성하는데 이를 10^x 의 각 x 마다 k로의 remainder 를 저장
 *      한다 생각하여 MAX_LEN 인 50 미만 까지만 구성했다.
 *      하지만 p10이 의미하는 바는 10^(x-1) => 0의 갯수이였다.
 * 
 *      해당 문제는 큰수에 대한 연산과 bit를 이용한 dp 에 대한 이해를 요구했다.
 *      나머지 연산에 대해 이해부족으로 인해 [A] 로 구성했으며, 비트마스킹DP 에 대한 이해 부족은 [C] 구성을 낳았다.
 *      또한 연산 우선 순위에 대한 착오는 [D] 를 유발했다.
 * 
 *      해당 문제 다시 풀어보면서 큰 수의 나머지 연산, 비트마스킹DP 에 대한 연습을 해야겠다.
 *      
 */

#define INIT_VAL -1
#define MAX_N 15
#define MAX_LEN 50
#define MAX_REMAINDERS 99
#define MAX_CASE (1 << MAX_N)

int n, k;
long long dp[MAX_CASE][MAX_REMAINDERS + 1];
int p10[MAX_LEN +  1];
int remainders[MAX_N];
string number_string[MAX_N];

void init_dp()
{
    for (int i = 0; i < (1<<n); ++i) {
        for (int j = 0; j < k; ++j) {
            dp[i][j] = INIT_VAL;
        }
    }
}

int get_big_num_remainders(string num_string)
{
    int result = 0;

    for (int i = 0; i < num_string.length(); ++i) {
        result = (result * 10 + (int)num_string[i] - '0') % k;
    }
    return result;
}

long long calculate_remainder_count(int bit_idx, int mod)
{
    long long &result = dp[bit_idx][mod];

    if (bit_idx == (1<<n) - 1) {
        return (mod == 0);
    }
    if (dp[bit_idx][mod] != INIT_VAL) {
        return dp[bit_idx][mod];
    }
    result = 0;
    for (int i = 0; i < n; ++i) {
        int current_bit = 1<<i;

        if (bit_idx & current_bit) {
            continue;
        }
        result += calculate_remainder_count(bit_idx | current_bit, (mod * p10[number_string[i].length()] + remainders[i]) % k);   
    }
    return result;
}

long long factorial(int a)
{
    long long ret = 1;

    for (int i = 2; i <= a; ++i) {
        ret *= i;
    }
    return ret;
}

long long gcd(long long a, long long b)
{
    long long tmp = a;

    if (a < b) {
        a = b;
        b = tmp;
    }
    while (b != 0) {
        tmp = a % b;
        a = b;
        b = tmp;
    }
    return a;
}

int main()
{
    long long gcd_val;
    long long nominator;
    long long denominator = 0;

    cin >> n;
    for (int i = 0; i < n; ++i) {
        cin >> number_string[i];
    }
    cin >> k;

    init_dp();
    p10[0] = 1 % k;
    for (int i = 1; i <= MAX_LEN; ++i) {
        p10[i] = p10[i - 1] * 10 % k;
    }
    for (int i = 0; i < n; ++i) {
        remainders[i] = get_big_num_remainders(number_string[i]);
    }
    nominator = calculate_remainder_count(0, 0);
    if (nominator == 0) {
        cout << "0/1\n";
        return 0;
    }
    denominator = factorial(n);
    gcd_val = gcd(denominator, nominator);
    cout << nominator / gcd_val << '/' << denominator / gcd_val << '\n';
}