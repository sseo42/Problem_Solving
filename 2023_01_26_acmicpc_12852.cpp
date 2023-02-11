#include <iostream>
#include <utility>
#include <queue>

using namespace std;

/**
 * Problem
 *      자연수 n[1, 10^6] 이 주어진다. 이를
 * 
 *      3으로 나눠지는 경우 3으로 나눈다.
 *      2로 나눠지는 경우 2로 나눈다.
 *      1을 뺀다
 * 
 *      3가지 연산을 통해 1로 만들 수 있는 최소 횟수를 구하고 출력,
 * 
 *      둘째줄에는 방법에 포함되있는 수를 공백으로 구분해 출력하자.
 * 
 * Ideas
 *      1에서 출발해 해당 수를 만들 수 있는 가장 빠른 길을 구하자.
 *      사용한 횟수, 현재 수를 queue 에 저장하고 n을 만족할 때 까지 구한다. 어떤 수이든 소인수분해할 떄
 *      각 수를 +1, *2, *3 으로 표현하는데 비용이 얼마 안 들을 둣?
 * 
 *      배열을 통해 이전 번호를 기억하도록하자, batch first 로 돌릴 것이기에 배열의 값이 갱신됬다면 shortcut임이 보장된다.
 *      경로는 이전 번호를 역추적해 출력하면 된다.
 * 
 * Comments
 *      queue를 이용해 탐색하는 것 까지는 좋았지만 쭉 연산되어 배열을 범위를 넘어갈 때 짜르는 부분을 간과했다.
 *      처음 수정했을 떄 넘어간다라는 것을 인지하고 나서 배열의 크기만 늘려주는 건 왜 그런건지...?
 * 
 *      항상 배열안으로 보장되는지 체크하자
 */

#define MAX_NUMBER 1000000

int previous[MAX_NUMBER * 3];
queue<pair<int, int> > num_q;

void print_trace(int n)
{
    while (n != 1) {
        printf("%d ", n);
        n = previous[n];
    }
    printf("1\n");
}

int main()
{
    int n;
    int count, num;

    scanf("%d", &n);
    num_q.push({0, 1});

    while (num_q.empty() == false) {
        count = num_q.front().first;
        num = num_q.front().second;
        num_q.pop();

        if (num > MAX_NUMBER) {
            continue;
        }
        if (num == n) {
            break;
        }
        if (previous[num + 1] == 0) {
            previous[num + 1] = num;
            num_q.push({count + 1, num + 1});
        }
        if (previous[num * 2] == 0) {
            previous[num * 2] = num;
            num_q.push({count + 1, num * 2});
        }
        if (previous[num * 3] == 0) {
            previous[num * 3] = num;
            num_q.push({count + 1, num * 3});
        }
    }
    printf("%d\n", count);
    print_trace(n);
    return 0;
}