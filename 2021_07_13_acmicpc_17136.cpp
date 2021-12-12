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
 *      큰 색종이부터 시작하여 부르트하게 탐색하자.
 *      탐색 사이즈, 남은 갯수, 지금까지 사용한 색종이 갯수를 원소로 하는 check함수를 두고
 *      최대 정사각형을 마크하는 배열을 함수의 지역변수로 가져간다.
 * 
 *      큰 사이즈부터 순회하여 함수를 재귀적으로 돌리고 색종이가 들어갈 자리가 
 *      확보된다면 입력 배열의 값을 0으로 둔다. 그 후 남은 갯수가 있다면 같은 탐색 사이즈로 돌리고,
 *      남은 갯수가 없다면 더 적은 사이즈를 원소로 돌린다.
 *      재귀적 탐색이 종료 된다면 해당 값을 1로 돌리고
 *      다른 들어갈 자리가 있는지 탐색하여 반복한다.
 * 
 *      사이즈가 2인 경우 탐색의 마지막에 1의 갯수가 5개 이하인지 체크하여 정답을 갱신한다.
 * 
 *      정답의 초기값은 IMPOSSIBLE로 하여 최소 갱신을 한다.
 * 
 *      구현 시작
 * 
 * Comments
 * 
 *      각 각의 색종이마다 전체 배열을 탐색하게 되면 시간이 초과된다.
 *      모든 1을 덮기 위해서는 각 배열 원소에 1~5중 하나가 덮어야한다.
 *      따라서 1번 전체배열을 백트래킹으로 순회하며 1~5 각 각의 케이스를 부루트하게 실험해보면된다.
 */

# define IMPOSSIBLE 987654321

int square[] = {5, 5, 5, 5, 5};
int arr[10][10];
int answer = IMPOSSIBLE;

bool is_possible_square(int r, int c, int size)
{
    if (r + size > 10 || c + size > 10)
        return false;
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (arr[r + i][c + j] == 0)
                return false;
        }
    }
    return true;
}

void change_arr_elem(int r, int c, int size, int state)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            arr[r + i][c + j] = state;
        }
    }
}

void update_answer(int r, int c, int cnt)
{
    if (r >= 10)
        answer = min(answer, cnt);
    else if (c >= 10)
        update_answer(r + 1, 0, cnt);
    else if (arr[r][c] == 1)
    {
        for (int size = 1; size <= 5; ++size)
        {
            if (square[size - 1] > 0 && is_possible_square(r, c, size))
            {
                change_arr_elem(r, c, size, 0);
                square[size - 1]--;
                update_answer(r, c + 1, cnt + 1);
                change_arr_elem(r, c, size, 1);
                square[size - 1]++;
            }
        }
    }
    else
        update_answer(r, c + 1, cnt);
}

int main(void)
{
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            scanf("%d", arr[i] + j);
        }
    }
    update_answer(0, 0, 0);
    if (answer == IMPOSSIBLE)
        printf("-1\n");
    else
        printf("%d\n", answer);
    return 0;
}