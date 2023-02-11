#include <iostream>

using namespace std;

/**
 * Problem
 *      0 < a < b < d <= 10,000 이 주어진다.
 * 
 *      최초 짚신벌레 1마리가 존재하며, 이 벌레는 a 일 째 부터 증식한다.
 *      b 일 쨰 부터는 증식하지 않고, d 일에는 없어진다.
 * 
 *      1 < N < 1,000,000 일에 존재하는 짚신벌레의 수를 1,000 으로 나눠 출력하자.
 * 
 * Ideas
 *      N 일 동안 10,000 배열을 뒤지면서 업데이트하면 시간이 오래걸린다.
 * 
 *      날짜 별로 벌레가 태어난 수를 기록할 배열과 - born
 *               번식 가능한 개체 수를 기록할 배열 - reproducible
 * 
 *      reproducible[i] = reproducible[i - 1] + born[i - a] - born[i - b]
 *      born[i] = reproducible[i]
 * 
 *      ans = (reproducible[N] + born[i - d] + ... + born[N] ) / 1000
 * 
 * Comments
 *      1000 나머지 적용함으로 인해서 음수 나와버릴 가능성이 있다는 것을 간과했다.
 *      여기서 시간 좀 썻다.
 * 
 *      reproduce 가능한 녀석들이 결국 증식하는 것이므로 born 배열을 대체할 수 있다.
 *      여기서 약간의 조정이 필요한데, 시간이 꽤나 걸렸다.
 *      첫번쨰 벌레가 증식하기 시작하는 날부터는 문제가 없지만 첫날 벌레의 born 을 어떻게 처리할지 고민했었다.
 * 
 *      결국 벌레의 상태는 아기 - 성인 - 노인 으로 나타낼수 있으며, reproducible 표현이 옳바르다면 첫 벌레의 born 이 미치는 영향은 d 인덱스를 넘어가면서
 *      전혀 없다는 점, a 까지는 무조건 1(첫 벌레가 번식을 못하므로) 이라는 점을 고려하면 a-b / b-d 구간만 처리하면 된다는 것을 빨리 알 수 있었을 것이다.
 * 
 *      생각을 좀 더 체계적으로 하면 좋을 둣..?
 */

#define MAX_LIFE 10000
#define MAX_DAYS 1000000

int reproducible[MAX_DAYS + MAX_LIFE + 1];

int *reproducible_bug = reproducible + MAX_LIFE;

int a, b, d, n;

void calculate_amount(void)
{
    reproducible_bug[a] = 1;
    reproducible_bug[b] = -1;
    for (int i = a + 1; i <= n; i++) {
        reproducible_bug[i] += (1000 + reproducible_bug[i - 1] + reproducible_bug[i - a] - reproducible_bug[i - b]) % 1000;
    }
}

int main(void)
{   
    int ans = 0;

    scanf("%d %d %d %d", &a, &b, &d, &n);

    if (n < a) {
        printf("1\n");
        return 0;
    } else if (n >= b && n < d) {
        ans = 1;
    }
    calculate_amount();
    ans += reproducible_bug[n];

    for (int i = n - a + 1; i <= n; i++) {
        ans += reproducible_bug[i];
    }
    for (int i = n - d + 1; i <= n - b; i++) {
        ans += reproducible_bug[i];
    }
    printf("%d\n", ans % 1000);
}