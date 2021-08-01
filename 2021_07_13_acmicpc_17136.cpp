#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * Problem
 *      10 x 10 의 2차원 배열이 주어진다.
 *      배열은 0 과 1 로 구성되 있으며, 0 은 덮히지 않고 1은 덮혀야한다.
 * 
 *      재료로 (1 x 1) ~ (5 x 5)의 종이가 각 5 개씩 있다.
 *      덮을 때 경계를 벗어나거나 재료가 겹쳐서는 안된다.
 * 
 *      만족하는 최소의 색종이 개 수를 출력하자. 불가능하다면 -1을 출력한다.
 * 
 * Ideas
 *      부루트 하게 최대 정사각형을 구한 다음 큰 재료부터 가능한 모든 위치를 탐색
 *      해가며 채운 뒤 다음 재료를 테스트하는 방삭은 어떤가?
 * 
 *      주어진 배열을 탐색하는 시간은 100이다.
 * 
 *      따라서 각 재료당 100 즉 100 ^ 5 시간이 넘친다.
 * 
 *      아니다 (5 x 5 경우의 수) (4 x 4 경우의 수) (3 x 3 경우의 수) ... X 100
 *      + (5 x 5 경우) ... (2 x 2 경우) X 100
 *      + ...
 *      + (5 x 5 경우) X 100
 * 
 *      아니다 만약 (5 x 5) 색종이가 하나라도 들어간다면 (4 x 4) 색종이가 들어갈 공간은
 *      더욱 줄어들어 (4 x 4 경우의 수는 5 x 5 색종이가 하나도 안들어갈 때 보다 더 작아질 것이다)
 *      같은 이유로 (5 x 5 경우의 수)는 항상 (4 x 4 경우의 수) 보다 작다.
 * 
 *      따라서 (5 x 5) ~ (3 x 3) 이 한번도 안 들어갈 때가 가장 많은 경우의 수가 생기는 데 
 *      이 를 25C5 + 25C4 + ... + 25C1 이라 어림 잡을 수 있다(2 x 2 의 최대 경우의 수).
 * 
 *      마찬가지로 (3 x 3 의 최대 경우의 수)는 9C5 + ... + 9C1
 * 
 *      따라서 곱으로 어림 잡아도 1 초 안에 처리가 가능하다. 실제로는 상위 색종이에서 선택을 하면 할 수록
 *      하위 경우의 수가 급격하게 줄어드므로 더 작을 것이다.
 * 
 *      필요한 자료구조는 다음과 같을 것이다.
 * 
 *      원본을 저장할 2차원 전역변수 arr
 *      백트레킹할 2차원 전역변수 visted
 *      visited 체크를 위해 쓰일 2차원 지역변수 possible_sq
 *      
 *      솔루션 함수와 파라미터를 색종이 크기, 남은 갯 수로 하는 재귀 함수로 나누자.
 * 
 *      구현 시작.
 * 
 */

# define ARR_SIZE 10
# define IMPOSSIBLE 987654321

int arr[ARR_SIZE + 1][ARR_SIZE + 1];
bool visited[ARR_SIZE + 1][ARR_SIZE + 1];

int fill_sq(int sq_size, int cnt)
{
    int out = IMPOSSIBLE;
    int possible_sq[ARR_SIZE + 1][ARR_SIZE + 1];
    int left_up, left, up;

    for (int i = 1; i <= ARR_SIZE; ++i)
    {
        for (int j = 1; j <= ARR_SIZE; ++j)
        {
            if (arr[i][j] && visited[i][j] == false)
            {
                left_up = arr[i - 1][j - 1];
                left = arr[i][j - 1];
                up = arr[i - 1][j];

                if (left_up == left && left_up == up)
                    arr[i][j] = left_up + 1;
                else
                    arr[i][j] = max(left_up, max(left, up));
            }
        }
    }
    
}

int solution()
{

}

int main(void)
{
    for (int i = 1; i <= ARR_SIZE; ++i)
    {
        for (int j = 1; j <= ARR_SIZE; ++j)
        {
            scanf("%d", &(arr[i][j]));
        }
    }
    printf("%d\n", solution());
    return 0;
}