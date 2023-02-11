#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      1 <= n <= 100,000   : 이진트리의 정점 수
 * 
 *      이진 트리의 각 정점에는 [1,n] 이 중복 없이 메겨져있다.
 * 
 *      n 이 후, 두줄에 걸쳐 각 n개의 이진트리의 post-order 와 in-order 가 주어진다.
 * 
 *      이진 트리의 pre-order 를 구하자.
 * 
 * Ideas
 *      in-order 는 a root b
 *      post-order 는 a b root
 * 
 *      두 개 모든 most-left 가 leaf-node 라면 가장 처음 찍힌다.
 * 
 *      이후 in-order 에서는 root 가 찍힌다. post-order 에서는 b right 를 다 찍은 뒤 root 를 찍으므로
 *      그 사이 있는 값들은 오른쪽 서브트리를 구성한다 할 수 있다.
 * 
 *      만약 most-left 가 leaf-node 가 아니라는 것은 오른쪽 자식이 있다는 것이므로 in-order 첫 값이
 *      post-order 에 나올 때 까지의 값이 오른쪽 서브트리를 구성하는 것을 알 수 있다.
 * 
 *      이를 통해 root-node 까지 구성가능하다.
 * 
 *      오른쪽 서브트리는 post-order 에서 root 바로 직전이 서브트리의 root 이다.
 * 
 *      이 값이 in-order 에서 나타날 때 ....
 * 
 *      -------------------------------------------------------------------------------------------
 * 
 *      트리의 정점은 post-order 마지막에 나온다. 이를 in-order 에서 찾을 때 왼쪽 수가 left-sub-tree
 *      오른쪽 수가 rigjt-sub-tree 를 구성한다. 또 다시 post-order 에서 왼쪽의 root 가 마지막에 등장하므로
 *      이를 반복하면 다 찾을 수 있다.
 * 
 *      이분 탐색의 비용은 log n 이므로 n log n 으로 풀린다.
 * 
 * Comments
 *      문제의 핵심을 이해하지 못해 Idea 를 잡는데 시간이 걸렸다.
 * 
 *      이분탐색은 쓸 수 없다.... 그러려면 in-order 의 값들이 정렬되있어야하는데 그렇지 않잖...?
 *      알고리즘을 쓸 때 조건을 자꾸 망각하는 것 같다, 필요할 때만 쓱 쓰고 대수롭지 않게 여기기 때문인듯...
 * 
 *      그냥 인덱싱하면 됬었다, 시간도 더 빠르다.
 */

#define MAX_NODE 100000

int n;
int in_order[MAX_NODE + 1];
int in_order_idx[MAX_NODE + 1];
int post_order[MAX_NODE + 1];

void solution(int in_order_left, int in_order_right, int post_order_left, int post_order_right)
{
    if (in_order_left > in_order_right) {
        return ;
    }
    if (post_order_left == post_order_right) {
        printf("%d ", post_order[post_order_right]);
        return ;
    }
    int root_val = post_order[post_order_right];

    printf("%d ", root_val);

    int in_order_root = in_order_idx[root_val];
    solution(in_order_left, in_order_root - 1, post_order_left, post_order_left + (in_order_root - in_order_left - 1));
    solution(in_order_root + 1, in_order_right, post_order_right - (in_order_right - in_order_root), post_order_right - 1);
}

int main(void)
{
    scanf("%d", &n);

    for (int i = 1; i <= n; ++i) {
        scanf("%d", in_order + i);
        in_order_idx[in_order[i]] = i;
    }
    for (int i = 1; i <= n; ++i) {
        scanf("%d", post_order + i);
    }
    solution(1, n, 1, n);
}