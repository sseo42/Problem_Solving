#include <iostream>
#include <utility>
#include <vector>

using namespace std;

/**
 * Problem
 *      컨베이어 길이 N과 이를 감싸는 2N 길이의 벨트가 주어진다.
 *      각 벨트 위치는 내구도를 가지고 있으며 내구도가 0인 위치가 k가 되면
 *      아래 시나리오가 종료되며 몇번 반복했었는지를 출력하자.
 *      시나리오:
 *          1. 벨트가 각 칸 위에 있는 로봇과 함꼐 한칸 회전한다.
 *          2. 로봇은 회전 방향으로 한칸 움직이며 이동하려는 칸의 내구도가 0 혹은 이미 로봇이 있다면 스킵한다.
 *          3. 로봇을 컨베이어 시작점에 위치시킨다. 이 행위는 해당 벨트 위치의 내구도를 1 감소시킨다.
 *          (시작점에서 벨트의 내구도가 0이라면 이를 스킵한다.)
 *          4. k 개수 판단 시점.
 * 
 * Ideas
 *      내구도가 0 인 지점의 갯수를 글로벌로 관리하여 루프를 탈출하여 정답을 출력하자.
 *      
 *      벨트를 vector로 관리하고 컨베이어 start 와 컨베이어 end 의 인덱스를 가지고 있으면 된다.
 *      1 번은 start 와 end 지점을 미루고
 *      2 번은 벨트를 end 지점 부터 순회하여 로봇을 이동하고, end 도달 로봇은 마커를 지운다.
 *      3 번은 start 지점에 로봇은 마크한다.
 * 
 *      start와 end를 움직일 함수, 2번 함수(조건절로 나눔), end 체크 함수, 3번 함수
 * 
 * Comments
 *      시간은 다소 오래 걸렸지만 나쁘지 않았다.
 *      class 내부에서 함수를 최대한 명확하게 쪼개기 위해 고민을 하여 시간은 많이 썼다.
 * 
 *      start 지점이 앞선다 해도 로봇의 이동은 두 지점 사이에서 일어나도록 적성했다는 점을
 *      일찍 깨닳았다면 시간이 좀 더 적게 소모됬을 것이다.
 * 
 *      zero를 count 하는 함수의 이름 뺴고는 만족한다. checkZero 대신 checkAndCount 라고 좀 더 획실히
 *      했으면 어떗을까.
 */

class ConveyBelt
{
private:
    typedef pair<bool, int> p;

    vector<p> belt;
    int belt_size;
    int convey_start;
    int convey_end;
    int convey_size;
    int zero_cnt;
    int zero_cnt_goal;
    int round_cnt;

    ConveyBelt();
    void rotateBelt(void);
    void moveRobots(void);
    void putRobot(void);

    void checkAndMoveRobot(int from, int to);
    void checkZero(int belt_idx);

public:
    ConveyBelt(int convey_size, int zero_cnt_goal);
    void setBeltDurability(vector<int> &belt);
    int getEndRound(void);
};

ConveyBelt::ConveyBelt(int convey_size, int zero_cnt_goal) : zero_cnt_goal(zero_cnt_goal)
{
    belt.resize(convey_size * 2);
    belt_size = convey_size * 2;
    convey_start = 0;
    convey_end = convey_size - 1;
    zero_cnt = 0;
    round_cnt = 1;
}

void ConveyBelt::setBeltDurability(vector<int> &src_belt)
{
    for (int i = 0; i < belt_size; ++i) {
        belt[i].second = src_belt[i];
    }
}

int ConveyBelt::getEndRound(void)
{
    while (1) {
        rotateBelt();
        moveRobots();
        putRobot();
        if (zero_cnt >= zero_cnt_goal)
            break;
        round_cnt++;
    }
    return round_cnt;
}

ConveyBelt::ConveyBelt() {}

void ConveyBelt::rotateBelt(void)
{
    convey_start--;
    if (convey_start < 0)
        convey_start = belt_size - 1;
    convey_end--;
    if (convey_end < 0)
        convey_end = belt_size - 1;
    belt[convey_end].first = false;
}

void ConveyBelt::moveRobots(void)
{
    if (convey_start > convey_end) {
        for (int i = convey_end - 1; i >= 0; --i)
            checkAndMoveRobot(i, i + 1);
        checkAndMoveRobot(belt_size - 1, 0);
        for (int i = belt_size - 2; i >= convey_start; --i)
            checkAndMoveRobot(i, i + 1);
    }
    else {
        for (int i = convey_end - 1; i >= convey_start; --i)
            checkAndMoveRobot(i, i + 1);
    }
    belt[convey_end].first = false;
}

void ConveyBelt::putRobot(void)
{
    if (belt[convey_start].second > 0)
    {
        belt[convey_start].first = true;
        belt[convey_start].second--;
        checkZero(convey_start);
    }
}

void ConveyBelt::checkAndMoveRobot(int from, int to)
{
    if (belt[from].first == true && belt[to].first == false && \
        belt[to].second > 0) {
        belt[from].first = false;
        belt[to].first = true;
        belt[to].second--;
        checkZero(to);
    }
}

void ConveyBelt::checkZero(int belt_idx)
{
    if (belt[belt_idx].second == 0)
        zero_cnt++;
}

int main(void)
{
    int n, k;
    vector<int> belt_durability;

    scanf("%d %d", &n, &k);

    ConveyBelt convey_belt(n, k);
    belt_durability.resize(n * 2);
    for (int i = 0; i < 2 * n; ++i) {
        scanf("%d", &(belt_durability[i]));
    }
    convey_belt.setBeltDurability(belt_durability);
    printf("%d\n", convey_belt.getEndRound());
    return 0;
}