#include <iostream>
#include <algorithm>

using namespace std;

/**
 * Problem
 *      3 <= n <= 5,000      : 용액의 수
 * 
 *      용액의 수와 이어 각 용액의 값이 주어진다.
 * 
 *      0 은 주어지지 않으며 [-1,000,000,000, 1,000,000,000] 이다.
 * 
 *      3가지 용액을 선택하 그 합이 0에 가장 가깝게되는 값을 구하고 이를 출력하자.
 * 
 * Ideas
 *      루프 두번에 이분탐색을 섞으면 된다.
 *       5000 * 5000 / 2 = 25,000,000 / 2
 * 
 *      이분 탐색 -> log(5000) ...
 * 
 *      산성과 알칼리를 나눠서 생각해야겠다, 정렬해서 음수나 양수가 없는 경우 바로 출력하고,
 * 
 *      알칼리 / 산성 각각 포인터 두고 이분탐색은 전체 용액으로.
 * 
 *      수가 크므로 long long 으로 풀자.
 * 
 * Comments
 *      다행히 제출에서 틀리지는 않았지만 중간중간 예외를 발견하고 수정한 사항들이 많았다.
 * 
 *      일단 출력 자체를 잘못알고 있었고, 아예 없는게 아니라면 반드시 음수 양수 하나씩은 선택하는게 최소일 것이라
 *      착각했었다. 또한 중복된 값을 고를 수 있는 케이스를 고려하지 못했다.
 * 
 *      보고 들은것은 있어서 대략적인 솔루션에 대한 감은 빠르게 잡았지만 구체적인 사고능력이 부족한 것 같다.
 *      추상적인 접근을 하는 것이 아니라 조금더 명확한 사고를 할 필요가 있다.
 */

#define MAX_AMOUNT 5000
#define MAX_DIFF 3000000000L

int n;
long long liquid[MAX_AMOUNT];

int bsearch(int start_idx, int last_idx, long long val)
{
    int left = start_idx;
    int right = last_idx;

    while (left < right) {
        int mid = (left + right) / 2;

        if (liquid[mid] < val) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int main()
{
    long long min_diff = MAX_DIFF;
    long long ans[3];

    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%lld", liquid + i);
    }
    sort(liquid, liquid + n);

    if (liquid[0] > 0) {
        printf("%lld %lld %lld", liquid[0], liquid[1], liquid[2]);
    } else if (liquid[n - 1] < 0) {
        printf("%lld %lld %lld", liquid[n - 3], liquid[n - 2], liquid[n - 1]);
    } else {
        int negative_idx = 0;
        int first_positive_idx = bsearch(0, n - 1, 0);
        int positive_idx = first_positive_idx;

        if (first_positive_idx + 2 < n) {
            min_diff = abs(liquid[first_positive_idx] + liquid[first_positive_idx + 1] + liquid[first_positive_idx + 2]);
            ans[0] = liquid[first_positive_idx];
            ans[1] = liquid[first_positive_idx + 1];
            ans[2] = liquid[first_positive_idx + 2];
        }
        if (first_positive_idx - 3 >= 0 && min_diff > abs(liquid[first_positive_idx - 3] + liquid[first_positive_idx - 2] + liquid[first_positive_idx - 1])) {
            min_diff = abs(liquid[first_positive_idx - 3] + liquid[first_positive_idx - 2] + liquid[first_positive_idx - 1]);
            ans[0] = liquid[first_positive_idx - 3];
            ans[1] = liquid[first_positive_idx - 2];
            ans[2] = liquid[first_positive_idx - 1];
        }

        for (int i = 0; i < first_positive_idx; ++i) {
            for (int j = first_positive_idx; j < n; ++j) {
                long long a = liquid[i];
                long long b = liquid[j];

                if (i + 1 < j) {
                    int searched_idx = bsearch(i + 1, j - 1, -(a + b));
                    long long diff = abs(a + b + liquid[searched_idx]);

                    if (i + 1 < searched_idx && abs(a + b + liquid[searched_idx - 1]) < diff) {
                        diff = abs(a + b + liquid[searched_idx - 1]);
                        --searched_idx;
                    }
                    if (diff < min_diff) {
                        min_diff = diff;
                        ans[0] = a;
                        ans[1] = b;
                        ans[2] = liquid[searched_idx];
                    }
                }

            }
        }
        sort(ans, ans + 3);
        printf("%lld %lld %lld\n", ans[0], ans[1], ans[2]);
    }
}