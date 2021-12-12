#include <iostream>
#include <utility>
#include <vector>

using namespace std;

/**
 * Problem
 *      행렬의 개수 N(1<= N <= 500)가 주어지고 둘째 줄부터 N개의 줄에 걸쳐
 *      행렬의 크기 r과 c가 주어진다(1 <= r,c <= 500)
 *      행렬 A가 5 x 3, 행렬 B가 3 x 7 이라 할 때 두 행렬의 곱연산에 필요한
 *      비용은 5 x 3 x 7 이다. 주어진 N개의 행렬을 전부 곱연산을 할 때 연산
 *      순서를 적절히 하여 최소의 총 연산 값을 구하자.
 * 
 * Ideas
 *      연산순서와 상관없이 최종적으로 얻어지는 행렬의 크기는 확정되있다.
 *      (첫 행렬의 r, 마지막 행렬의 c) 따라서 주어진 행렬이 A B C D 라 할 때
 *      최종적으로 얻어지는 행렬에서 B와 C 사이를 끊어줄 때 A'r x B'c(C'r) x D'c 이라
 *      할 때 모든 고리를 끊어주는 최소 비용을 구하는 것과 같다.
 *      만약 B와 C 사이를 끊었다면 얻어지는 A x B, 그리고 C x D 두 행렬을 끊어주기 위해
 *      B'c 그리고 C'r 모두 다시 연산에 들어갈 것이므로 항상 제일 작은 지점을 끊는 포인트로
 *      잡는다면 부르트하게 정답을 구할 수 있다.
 * 
 *      세그먼트 구성을 통해 구간별로 가장 작은 벨류를 저장한다.
 *      구간의 left_value와 right_value를 받아서 최소 연산값을 리턴하는 함수가 있다고 할 때
 *      구간 추적을 통해 최소값을 구할 수 있다. 이는 log꼴의 연산을 필요로 하고 구성을 하는데에는
 *      n * 2만을 필요로 하므로 최소값을 특정할 때 마다 전체 구간을 돌아보는 n^2꼴에 비해 시간복잡도가 낮을 것이다.
 * 
 *      세그먼트 구성은 크기가 2의 배수 꼴로 구성할 것이므로 주어진 크기 범위를 벗어나는 부분은 IMPOSSIBLE
 *      을 큰 값으로 두어 잘못된 value를 올리는 일이 없도록 하자
 *      --------------------------------------------------------------------------------------
 *      전제가 잘못됬다. B'c C'r 모두 다시 연산에 들어가는 것은 맞지만 이것이 최소값을 보장한다 하기에는 근거
 *      가 없다. 따라서 2차원 dp테이블을 통해 가능한 모든 경우를 살펴봐야한다.
 *      최종적으로 얻어지는 행렬의 크기는 고정이므로 마지막 연산전의 두 행렬 A,B를 구성하는 각각 최소값을 알고 있다면
 *      마지막 연산비용을 더해 비교함으로서 최종 연산 후의 최소값을 구할 수 있다.
 * 
 *      같은 논리를 아래로 적용가능하므로 dp테이블 구성을 통해 총연산의 최소값을 구할 수 있다.
 *      구현에는 인덱스마다 행렬의 r,c를 페어담고 있을 배열하나, 최소값을 담고 있을 dp 테이블 2개의 구조가 필요하다.
 *      값은 INT_MAX보다 같거나 작으므로 dp테이블은 INT_MAX로 초기화 한다.
 * 
 *      구현 시작
 * 
 * Comments
 *      부족한 근거로 그리디하게 접근하려 했다, 충분한 근거가 없다면 dp를 생각해보자
 */

#define IMPOSSIBLE 2147483647
typedef pair<int, int> p;

int n;
vector<p> matrix_size_array;
vector<vector<int> > dp_table;

int get_minval(int start, int end)
{
    if (dp_table[start][end] != IMPOSSIBLE)
        return dp_table[start][end];
    
    int minval = IMPOSSIBLE;
    int partial_minval;
    for (int i = start; i < end; ++i)
    {
        partial_minval = get_minval(start, i) + get_minval(i + 1, end) + \
            matrix_size_array[start].first * matrix_size_array[i].second * matrix_size_array[end].second;
        minval = (partial_minval < minval) ? partial_minval : minval;
    }
    dp_table[start][end] = minval;
    return minval;
}

int main(void)
{
    scanf("%d", &n);
    matrix_size_array.resize(n);
    dp_table.resize(n);

    int row, col;
    for (int i = 0; i < n; ++i)
    {
        scanf("%d %d", &row, &col);
        matrix_size_array[i].first = row;
        matrix_size_array[i].second = col;
        dp_table[i].resize(n, IMPOSSIBLE);
        dp_table[i][i] = 0;
    }

    printf("%d\n", get_minval(0, n - 1));
    return 0;
}