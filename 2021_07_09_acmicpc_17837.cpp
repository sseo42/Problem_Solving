#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

/**
 * Problem
 *      N x N 2차원 배열에 K 개의 말이 있다.
 *      말은 번호 순서대로 이동하며 각 각 정해진 방향이 있다.
 *      말은 다른 말 위에 올라갈 수 있으며 4개 이상이 쌓이면 게임이 종료된다.
 * 
 *      이동하려는 칸의 색이
 *      흰색:   A 말과 그 위의 말을 이동한다.
 *      빨간색: A 말과 그 위 말을 이동하고 , A번 말과 그 위 쌓인 말의 순서를 반대로 한다.
 *      파란색: A 번 말의 이동방향을 거꾸로 하고 한칸이동한다. 만약 해당위치도 파란색이며 이동하지 않는다.
 * 
 *      게임이 종료되는 턴의 번호를 구하자. 값이 1000보다 크다면 -1을 반환한다.
 * 
 *      12 이하의 N, 10 이하의 K
 * 
 * Ideas
 *      시간복잡도는 문제가 안된다.
 *      구현이다.
 * 
 *      필요한 자료구조는 다음과 같을 것이다.
 * 
 *      배열을 저장할 arr
 *      말 배열 원소로 자신 위에 있는 말의 번호를 저장.
 *      말객체를 저장할 vector<{말, {loc}}>
 * 
 *      구현시작.
 *      예제 케이스를 통과 못함...
 * 
 *      파란색의 경우 새로이 이동하는 칸을 또 고려한다는 점을 인지하지 못함.
 * 
 * Comments
 *      문제를 제대로 숙지 하지 못한 점이 컸다. 파란색의 경우 새로이 방향을 설정했을 때 당연히 흰색처럼 움직일 것이라 지레짐작한점.
 * 
 *      또한 하나의 함수에서 완전함을 보장하지 못해 문제의 원인이 어딘지 확신하기가 어려웠다.
 *      말의 연결 상태를 끊고 이어주는 과정이 여러 함수에 걸쳐 있으므로 파악에 골치가 아프다.
 * 
 *      이는 잘못된 자료구조를 선택함에 크게 기인한다. 2차원 배열을 이용해 말의 위치를 컨트롤 하고 큐와 스택을 사용해 힌색과 빨간색의 이동을 구현했더라면
 * 
 *      시간과 공간복잡도는 약간 올라갈지라도 하나의 함수에 완전함을 부여할 수 있었을 것이다.
 */

#define TURN_LIMIT 1000

typedef pair<int, int> p;
typedef pair<p, p> p4;

int N, K;
vector<vector<int> > arr;
vector<p4> horse;  // up_elem, direction, r, c
int dr[] = {0,0,0,-1,1};
int dc[] = {0,1,-1,0,0};

void cut_bottom(int target)
{
    for (int j = 0; j < K; ++j)
    {
        if (j != target && horse[j].first.first == target)
        {
            horse[j].first.first = j;
            break ;
        }
    }
} // target 밑의 원소 끊기, target 과 같지 않으면서 target 을 가리키는 대상을 self로 돌린다.

void move(int target, int new_r, int new_c)
{
    for (int j = 0; j < K; ++j)
    {
        if (horse[j].second.first == new_r && 
            horse[j].second.second == new_c &&
            horse[j].first.first == j)
        {
            horse[j].first.first = target;
            break ;
        }
    } // 이동 지점의 원소 연결

    while (target != horse[target].first.first)
    {
        horse[target].second.first = new_r;
        horse[target].second.second = new_c;
        target = horse[target].first.first;
    }
    horse[target].second.first = new_r;
    horse[target].second.second = new_c;
} // 위에 있는 모든 말들의 좌표를 갱신

int reverse_horse(int target, int next)
{
    int out;

    if (target == horse[target].first.first)
    {
        horse[target].first.first = next;
        return target;
    }
    out = reverse_horse(horse[target].first.first, target);
    horse[target].first.first = next;
    return out;
} // 해당 말을 포함해 그 이상에 있는 말들의 연결 순서를 바꾸고 새로운 바닥 말을 리턴

int check_stack(int target)
{
    int out;

    if (target == horse[target].first.first)
        return 1;
    return check_stack(horse[target].first.first) + 1;
}

void check_and_move(int i)
{
    int tmp_r, tmp_c;

    tmp_r = horse[i].second.first + dr[horse[i].first.second];
    tmp_c = horse[i].second.second + dc[horse[i].first.second];
    if (tmp_r < 0 || tmp_r >= N || tmp_c < 0 || tmp_c >= N ||
        arr[tmp_r][tmp_c] == 2)
    {
        if (horse[i].first.second % 2 == 1)
            horse[i].first.second++;
        else
            horse[i].first.second--;
        tmp_r = horse[i].second.first + dr[horse[i].first.second];
        tmp_c = horse[i].second.second + dc[horse[i].first.second];
        if (tmp_r >= 0 && tmp_r < N && tmp_c >= 0 && tmp_c < N && arr[tmp_r][tmp_c] != 2)
        {
            check_and_move(i);
        }
    }
    else if (arr[tmp_r][tmp_c] == 1)
    {
        cut_bottom(i);
        int tmp = reverse_horse(i, i);

        move(tmp, tmp_r, tmp_c);
    }
    else
    {
        cut_bottom(i);
        move(i, tmp_r, tmp_c);
    }
}

int solution()
{
    int turn = 0;
    int tmp_r, tmp_c;

    while (turn < TURN_LIMIT)
    {
        turn++;
        for (int i = 0; i < K; ++i)
        {
            check_and_move(i);
            for (int j = 0; j < K; ++j)
            {
                if (check_stack(j) >= 4)
                    return turn;
            }
        }
    }
    return -1;
}

int main(void)
{
    
    scanf("%d %d", &N, &K);
    arr.resize(N);
    horse.resize(K);
    for (int i = 0; i < N; ++i)
    {
        arr[i].resize(N);
        for (int j = 0; j < N; ++j)
        {
            scanf("%d", &(arr[i][j]));
        }
    }
    int a, b, c;
    for (int i = 0; i < K; ++i)
    {
        scanf("%d %d %d", &a, &b, &c);
        horse[i] = {{i, c}, {a - 1, b - 1}};
    }
    printf("%d\n", solution());
    return 0;
}