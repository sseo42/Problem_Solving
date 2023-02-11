#include <stdio.h>

/**
 * Problem
 *      증가하는 부분 수열 문제
 *      1 <= n <= 1,000,000     : 수열의 크기
 * 
 *      수열의 각 수는 int 범위 이내이다.
 * 
 * Ideas
 *      쭉 읽으면서 자기보다 크면 뒤에 달고, 작으면 담긴것 중 이분탐색 돌려서 삽입. 자신의 앞에 있는 놈을 대체한다.
 * 
 * Comments
 *      굉장이 많이 틀렸다.
 *      처음 제출에서 LST 에서는 마지막에 부분수열임을 보장하지 않는다는 점을 모른체 제출했다.
 * 
 *      두번째에서는 사이즈가 커질 때마다 부분 수열을 갱신하게 했으며, 시간복잡도가 염려되어 세그먼트를 이용해 이전에 수정된 부분만 수정하기로 했다.
 *      하지만 여기서는 수정된 순서가 고려 되지 않고 업데이트되므로 순서를 보장하지 않는 값으로 갱신되버렸다.
 * 
 *      세번째에서는 세그먼트를 버리고 배열을 이용해 수정된 부분을 기록했다, 이어 사이즈가 커질 때 뒤로 순회하며 마킹된 경우 업데이트하고 수정되지 않은 인덱스일 떄 탈출했다.
 *      언듯 순서를 보장할 수 있겠다 생각했지만 사실 연속된 인덱스가 수정된 다음에 사이즈가 늘어나버리면 마찬가지로 순서가 꼬인다.
 * 
 *      네번쨰는 수정될 때 등장한 순서를 기록하도록 했다. 순서가 보장될 것이라 생각했지만 만약 사이즈가 늘기 전에 수정된 값이 또 수정되면서 등장한 순서가 업데이트 되버릴 경우
 *      해당 인덱스 하위로는 올바르게 업데이트되지 않게된다. (등장 순서가 역전됬다 판단되어 break 되므로)
 * 
 *      다른 사람 풀이를 보고 배열을 활용해 숫자가 반영된 인덱스를 적어주고 역순회하도록 수정했다. 이를 통해 중간에 이상한 값이 업데이트 되더라도 이를 무시하고 다음을 찾으러가므로
 *      하위 값이 업데이트 안되버리는 불상사를 막을 수 있다.
 */

#define MAX_LEN 1000000

int sub_sequence[MAX_LEN];

int binary_search(int left, int right, int val)
{
    int mid;

    while (left != right) {
        mid = (left + right) / 2;
        if (sub_sequence[mid] < val) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return right;
}

int main(void)
{
    int sequence[MAX_LEN];
    int sub_seq_index[MAX_LEN];
    int n;
    int sub_seq_len = 1;
    int idx, number;

    scanf("%d", &n);
    scanf("%d", sub_sequence);
    sequence[0] = sub_sequence[0];
    sub_seq_index[0] = 0;
    for (int i = 1; i < n; ++i) {
        scanf("%d", &number);
        sequence[i] = number;

        if (number > sub_sequence[sub_seq_len - 1]) {
            idx = sub_seq_len;
            ++sub_seq_len;
        } else {
            idx = binary_search(0, sub_seq_len - 1, number);
        }
        sub_sequence[idx] = number;
        sub_seq_index[i] = idx;

    }
    printf("%d\n", sub_seq_len);

    idx = sub_seq_len - 1;
    for (int i = n - 1; i >= 0; --i) {
        if (idx < 0) {
            break ;
        }
        if (sub_seq_index[i] == idx) {
            sub_sequence[idx] = sequence[i];
            --idx;
        }
    }
    for (int i = 0; i < sub_seq_len; ++i) {
        printf("%d ", sub_sequence[i]);
    }   
    printf("\n");
}