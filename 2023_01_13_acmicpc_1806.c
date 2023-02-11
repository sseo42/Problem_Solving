#include <stdio.h>

/**
 * Problem
 *      1 <= n <= 100,000       : 수열의 길이
 *      0 <= s <= 100,000,000   : 목표합
 * 
 *      s 를 만족하는 부분수열합 중 가장 짧은 길이를 출력하자.
 * 
 *      각 값은 10,000 이하의 자연수 이다.
 * 
 * Ideas
 *      투포인터로 쓱 돈다 넘으면 꼬리에서 줄이고 만족못하면 앞을 늘린다.
 *      만족하는 경우에서 업데이트 친다.
 * 
 * Comments
 *      특이사항 없었다.
 * 
 *      중간ㅇ에 last_idx < i 조건을 뺄까했지만 이거 빼면 s = 0 일 때 인덱스 역전 났을 수 있다.
 */

#define MAX_LENGTH 100000

int main()
{
    int val_array[MAX_LENGTH];
    int n, s, val;
    int val_sum = 0;
    int last_idx = 0;
    int min_length = MAX_LENGTH + 1;

    scanf("%d %d", &n, &s);
    for (int i = 0; i < n; ++i) {
        scanf("%d", &val);

        val_sum += val;
        if (val_sum >= s) {
            while (last_idx < i && val_sum - val_array[last_idx] >= s) {
                val_sum -= val_array[last_idx];
                last_idx++;
            }
            if (i - last_idx < min_length) {
                min_length = i - last_idx + 1;
            }
        }
        val_array[i] = val;
    }
    if (min_length == MAX_LENGTH + 1) {
        printf("0\n");
    } else {
        printf("%d\n", min_length);
    }
}