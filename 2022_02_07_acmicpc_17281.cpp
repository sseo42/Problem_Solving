#include <iostream>
#include <vector>

using namespace std;

/**
 * Problem
 *      야구게임을 진행한다.
 *      첫 줄에 이닝 수 2 <= N <= 50 이 주어지고 
 *      이 후 N 줄에 걸쳐 이닝에서 선수가 얻을 결과가 주어진다.
 * 
 *      1 -> 안타
 *      2 -> 2루타
 *      3 -> 3루타
 *      4 -> 홈런
 *      0 -> 아웃
 * 
 *      총 9명의 선수가 있다. 한 이닝에는 아웃을 기록하는 타자가 적어도 한명 존재한다.
 * 
 *      가장 많은 득점을 하는 타순을 찾고 그 떄의 득점을 출력하자.
 * 
 * Ideas
 *      모든 경우의 수를 탐색한다면, 9! -> 1초 안에 탐색 가능하다.
 *      선수별 이닝 결과를 2차원 배열에 담아놓고, 타순을 브루트하게 탐색한다.
 *      타순이 정해졌다면, 
 * 
 *      for 이닝
 *          strike = 0
 *          while (strike < 3)
 *              2차원배열[현재이닝][타순[idx]] 
 *              idx++ 및 조정
 * 
 *          idx 저장
 * 
 *      와 같은 방삭으로 접근하자.
 * 
 * Comments
 *      함수명을 정할 때 중복되는 부분이 있어서 시간이 걸렸다.
 *      처리하는 함수와 그 내부 부분함수들의 이름을 적절하게 설정해야겠다.
 */

#define NUM_OF_PLAYERS 9

enum BASEBALL
{
    THIRD_BASE = 0,
    SECOND_BASE,
    FIRST_BASE,
    HOME_BASE
};

int innings;
bool base[4 * 2];
vector<vector<int> > performance;
int used[NUM_OF_PLAYERS];
int player_lineup[NUM_OF_PLAYERS];

void clearBase(void)
{
    for (int i = 0; i < HOME_BASE; ++i)
        base[i] = false;
}

int getScore(int hit)
{
    int score = 0;

    base[HOME_BASE] = true;
    for (int i = 0; i < hit; ++i)
        if (base[i] == true)
            score++;
    for (int i = 0; i <= HOME_BASE; ++i)
        base[i] = base[i + hit];
    return score;
}

int getTotalScore(void)
{
    int total_score = 0;
    int player_turn = 0;

    for (int i = 0; i < innings; ++i) {
        int strike = 0;
        int hit;

        clearBase();
        while (strike < 3) {
            hit = performance[i][player_lineup[player_turn]];
            if (hit)
                total_score += getScore(hit);
            else
                strike++;
            player_turn++;
            if (player_turn >= NUM_OF_PLAYERS)
                player_turn = 0;
        }
    }
    return total_score;
}

int solution(int order)
{
    int ans = 0;
    int score;

    if (order >= NUM_OF_PLAYERS)
        return getTotalScore();
    if (order == 3)
        return solution(order + 1);
    for (int i = 0; i < NUM_OF_PLAYERS; ++i) {
        if (!used[i]) {
            player_lineup[order] = i;
            used[i] = true;
            score = solution(order + 1);
            if (ans < score)
                ans = score;
            used[i] = false;
        }
    }
    return ans;
}

int main(void)
{
    scanf("%d", &innings);

    performance.resize(innings);
    for (int i = 0; i < innings; ++i) {
        performance[i].resize(NUM_OF_PLAYERS);
        for (int j = 0; j < NUM_OF_PLAYERS; ++j) {
            scanf("%d", &(performance[i][j]));
        }
    }
    player_lineup[3] = 0;
    used[0] = true;
    printf("%d\n", solution(0));
    return 0;
}