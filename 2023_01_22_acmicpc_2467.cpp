#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      용액의 값이 정렬된 상태로 주어진다.
 *      이 중 더해 0에 가장 가까운 값을 얻게되는 값 2개를 출력하자.
 *      용액의 절대값은 [1,1000000000] 이다.
 * 
 *      2 <= n <= 100,000       : 용액의 수
 *      
 *      n 에 이어서 한줄에 걸쳐 용액의 값들이 주어진다.
 * 
 * Ideas
 *      이전에 풀었던 문제의 단순한 버젼.
 *      음의 값과 양의 값 두개를 나눠 저장하고 한쪽은 loop 로 나머지 반대 용액은 이분탐색으로 찾는다.
 *      한쪽에서 두개 모두 선택해야되는 경우가 있으므로 이를 고려하자.
 * 
 * Comments
 *      가볍게 놓쳤던 부분으로 정답은 용액 2개의 값인데 ans 를 따로 선언 안해서 뒤에 가서 추가했던 부분.
 *      그리고 기준이 되는 min_diff 값을 갱산하지 않아서 사실상 무조건 갱신한 부분.
 * 
 *      제출 이후 segfault 났다, 이전 유사문제 떄는 positive / negative 만 주어질 떄 바로 출력하고 종료했었다.
 * 
 *      이전 문제에서 한쪽이 없을 때만 한쪽을 선택하도록 작성해서 로직에 공백이 생겨 무조건 양쪽을 체크하도록 했다.
 *      그러다보니 positive 가 없을 때 bsearch 에서 idx 0를 돌려주고 접근하려해서 문제가 생겼다.
 * 
 *      케이스에 대한 고려가 부족했다. 잘못된 추정은 좋지 않다...
 */

vector<int> negative_liquid;
vector<int> positive_liquid;

int bsearch_in_positive_liquid(int val)
{
    int left = 0;
    int right = positive_liquid.size() - 1;

    while (left < right) {
        int mid = (left + right) / 2;

        if (positive_liquid[mid] < val) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int main()
{
    #define IMPOSSIBLE 2147483647

    int n, val;
    int min_diff = IMPOSSIBLE;
    int size_of_negative;
    int size_of_positive;
    int ans[2];

    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%d", &val);

        if (val < 0) {
            negative_liquid.push_back(val);
        } else {
            positive_liquid.push_back(val);
        }
    }

    size_of_negative = negative_liquid.size();
    size_of_positive = positive_liquid.size();
    if (size_of_negative > 1 && negative_liquid[size_of_negative - 2] + negative_liquid[size_of_negative - 1] > -min_diff) {
        ans[0] = negative_liquid[size_of_negative - 2];
        ans[1] = negative_liquid[size_of_negative - 1];
        min_diff = abs(ans[0] + ans[1]);
        if (size_of_positive == 0) {
            printf("%d %d\n", ans[0], ans[1]);
            return 0;
        }
    }
    if (size_of_positive > 1 && positive_liquid[0] + positive_liquid[1] < min_diff) {
        ans[0] = positive_liquid[0];
        ans[1] = positive_liquid[1];
        min_diff = abs(ans[0] + ans[1]);
    }
    for (int i = 0; i < size_of_negative; ++i) {
        int negative_liquid_val = negative_liquid[i];
        int idx_of_positive_liquid = bsearch_in_positive_liquid(-negative_liquid_val);

        if (idx_of_positive_liquid > 0 && abs(positive_liquid[idx_of_positive_liquid - 1] + negative_liquid_val) < abs(positive_liquid[idx_of_positive_liquid] + negative_liquid_val)) {
            --idx_of_positive_liquid;
        }
        
        if (abs(negative_liquid_val + positive_liquid[idx_of_positive_liquid]) < min_diff) {
            min_diff = abs(negative_liquid_val + positive_liquid[idx_of_positive_liquid]);
            ans[0] = negative_liquid_val;
            ans[1] = positive_liquid[idx_of_positive_liquid];
            min_diff = abs(ans[0] + ans[1]);
        }
    }
    printf("%d %d\n", ans[0], ans[1]);
}