#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <cassert>

using namespace std;

/**
 * Problem
 *      공의 갯수 N (1 <= N <= 200,000)이 주어지고, 이어 공마다 색과 크기가 주어진다.
 *      각 공은 자신보다 크기가 작고 색이다른 공을 사로잡을수 있으며 해당 공의 크기만큼 점수를 얻는다.
 *      사로잡은 수 자신의 색이나 크기가 변하지는 않는다.
 *      각 공이 사로잡을 수 있는 모든 공들의 크기의 합을 출력하자.
 *      컬러 C, 사이즈 S (1 <= C <= N, 1 <= S <= 2,000), 같은 크기 혹은 같은 컬러의 공이 존재할 수 있다.
 *
 * Ideas
 *      해당 문제는 Nlog(N) 이하의 시간복잡도를 요구한다.
 *      공이 특정한 공 A 를 사로잡을 수 있다면 A와 색이 같고 크기가 작은 모든 공들을 사로잡을 수 있다.
 *      공은 자신보다 작은 모든 공에서 자신보다 작고 색이 같은 공들을 뺸 만큼 사로잡을 수 있다.
 *
 *      입력을 받으며 공들에 인덱스를 달아 색깔별로 구분하여 크기와 함께 vector 에 삽입한다.
 *      또한 크기별로 vector를 구성하고 이에 인덱스를 삽입한다.
 *
 *      전역으로 다음을 가져간다.
 *      vector<vector<int> >    ball_by_size
 *      vector<vector<p> >      ball_by_color
 *      vector<int>             small_ball_sum;
 *      vector<int>             small_ball_with_same_color_sum;
 *
 *      필요한 함수는 다음과 같다.
 *      update_small_ball_sum()
 *      update_small_ball_with_same_color_sum()
 *
 * Comments
 *      구슬의 갯수를 세는 것이 아닌 크기의 합을 구하는 문제이다. 문제를 잘못 이해함으로 인해서 시간 소요가
 *      발생했다. 로직에서는 크게 문제를 겪지 않았지만 투포인터를 이용해서 계산을 할 때 헷갈려서 시간을 소요했다.
 *      정확한 개념을 잡아야한다.
 */

#define MAX_SIZE 2000

typedef pair<int, int> p;

int ball_amount;

vector<vector<int> > ball_by_size(MAX_SIZE + 1);
vector<vector<p> > ball_by_color;
vector<int> small_ball_sum;
vector<int> small_ball_with_same_color_sum;

void update_small_ball_sum(void)
{
    int cumulative_sum = 0;

    for (int i = 1; i <= MAX_SIZE; ++i) {
        for (int j = 0; j < ball_by_size[i].size(); ++j) {
            small_ball_sum[ball_by_size[i][j]] = cumulative_sum;
        }
        cumulative_sum += i * ball_by_size[i].size();
    }
}

void update_small_ball_with_same_color_sum(void)
{
    int amount;
    int cumulative_sum;
    int pointer1, pointer2;

    for (int i = 1; i <= ball_amount; ++i) {
        amount = ball_by_color[i].size();
        cumulative_sum = 0;
        if (amount > 0) {
            sort(ball_by_color[i].begin(), ball_by_color[i].end());

            pointer1 = 0;
            while (pointer1 < amount) {
                for (pointer2 = pointer1; pointer2 < amount && \
                     ball_by_color[i][pointer2].first == ball_by_color[i][pointer1].first; ++pointer2)
                    small_ball_with_same_color_sum[ball_by_color[i][pointer2].second] = cumulative_sum;

                if (pointer2 == amount)
                    break ;
                cumulative_sum += (pointer2 - pointer1) * ball_by_color[i][pointer1].first;
                pointer1 = pointer2;
            }
        }
    }
}

int main(void)
{
    int color, size;

    scanf("%d", &ball_amount);
    ball_by_color.resize(ball_amount + 1);
    small_ball_sum.resize(ball_amount + 1);
    small_ball_with_same_color_sum.resize(ball_amount + 1);

    for (int i = 1; i <= ball_amount; ++i) {
        scanf("%d %d", &color, &size);
        ball_by_size[size].push_back(i);
        ball_by_color[color].push_back({size, i});
    }
    update_small_ball_sum();
    update_small_ball_with_same_color_sum();
    for (int i = 1; i <= ball_amount; ++i) {
        printf("%d\n", small_ball_sum[i] - small_ball_with_same_color_sum[i]);
    }
    return 0;
}