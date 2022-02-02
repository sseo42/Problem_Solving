#include <iostream>
#include <vector>
#include <map>

using namespace std;

/**
 * Problem
 *      크기가 N x N인 격자에 M개의 파이어볼이 있다.
 *      각 파이어 볼은 질량 m, 방향 d, 속력 s 가 있다.
 *      방향은 다음과 같다.
 *
 *      7   0   1
 *      6       2
 *      5   4   3
 *
 *      step 1에서 모든 파이어볼은 d방향으로 s 만큼 이동한다.
 *      * 격자의 맨끝은 서로 연결되어 있다.
 * 
 *      이동이 끝난 후 2개 이상의 파이어볼이 있는 칸에서는 step2를 수행한다.
 * 
 *      step2.
 *      모든 파이어볼을 합친다.
 *      파이어볼은 4개로 나눠진다.
 *      질량은 floor(sumOfMass / 5) 이다. *질량이 0인 파이어볼은 소멸된다.
 *      속력은 floor(sumOfVelocity / NumberOfFireball)
 *      파이어볼들의 뱡향이 모두 even or odd 이면 방향은 0, 2, 4, 6
 *      그렇지 않다면 1, 3, 5, 7 이다.
 * 
 *      step을 k번 수행 후에 남아있는 파이어볼의 질량 합을 구하자.
 * 
 *      첫줄에 N, M, K가 주어진다.
 *      둘째줄에 파이어볼에 대한 정보가  r, c, m, s, d 로 주어진다.
 *      시작시 같은 위치에 파이어볼을 주어지지는 않는다.
 * 
 * Ideas
 *      총 8방향에 대한 dx, dy를 정의하자.
 *      파이어볼은 M크기의 vector로 관리하고, 이를 순회해 이동명령을 수행하자.
 *      s 는 1000 보다 작으므로 이동시 N * 1000의 바이어스를 이용해 나머지를 구해 이동시키자.
 *      파이어볼 vector는 r, c, m, s, d 를 저장하는 struct로 구성하자.
 *      맵에는 파이어볼의 인덱스들을 저장할 수 있게 하자. 이를 통해 size가 2 이상일 때 step2
 *      를 수행하면 된다.
 * 
 *      이동 및 board에 마크 함수, 개수 체크 함수, 질량 update, 속력 update, 방향 update
 *      파이어볼은 삭제/추가 될 수 있으므로 map 구조를 써서 관리하는게 깔끔하다.
 *      vector로 관리하자면 빈공간을 메꿀 로직이 필요함...
 * 
 * Comments
 *      좀 더 나은 구성이 있을 수 있으나,
 *      파이어볼을 map 자료구조로 관리하고 맵에 key를 물려놓는 것은 나쁘지 않았다.
 *      하지만 key를 이용해 어떻게 파이어볼들을 제어할지에 대한 고려가 부족해 헤메는 시간이 있었다.
 * 
 *      또한 struct 와 map 의 이름, 인자의 이름을 유사하게 선언하여 스스로를 헷갈리게 만들었다.
 *      이는 런타임에러를 받은 원인이 되었다.
 * 
 *      또한 스코프에 대한 생각을 깊게 하지 않아 수정 중 초기화를 잘못하는 일이 있었다.
 * 
 *      스코프와 역할을 정확한 구분을 하는 사고를 길러야겠다.
 */

int dr[] = {-1, -1, 0, 1, 1, 1, 0, -1};
int dc[] = {0, 1, 1, 1, 0, -1, -1, -1};

int new_direction_case[2][4] = {{1, 3, 5, 7}, {0, 2, 4, 6}};

typedef struct  FireBall
{
    int r;
    int c;
    int mass;
    int direction;
    int speed;
}               FireBall_t;

vector<vector<vector<int> > > board;
map<int, FireBall_t> fireballs;
int n, m, k;

void createBoard(void) {
    board.resize(n);
    for (int i = 0; i < n; ++i) {
        board[i].resize(n);
        for (int j = 0; j < n; ++j) {
            board[i][j].resize(0);
        }
    }
}

void clearBoard(void)
{
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[0].size(); ++j) {
            board[i][j].resize(0);
        }
    }
}

void updateBoard(void)
{
    clearBoard();
    map<int, FireBall_t>::iterator iter = fireballs.begin();
    for (; iter != fireballs.end(); ++iter) {
        board[iter->second.r][iter->second.c].push_back(iter->first);
    }
}

void moveFireball(FireBall_t &fireball)
{
    static int BIAS = 1000;

    fireball.r = (BIAS * n + fireball.r + dr[fireball.direction] * fireball.speed) % n;
    fireball.c = (BIAS * n + fireball.c + dc[fireball.direction] * fireball.speed) % n;
}

int getNewMass(vector<int> &old_fireballs)
{
    map<int, FireBall_t>::iterator target;
    int total_mass = 0;

    for (int i = 0; i < old_fireballs.size(); ++i) {
        target = fireballs.find(old_fireballs[i]);
        total_mass += target->second.mass;
    }
    return (total_mass / 5);
}

int getNewSpeed(vector<int> &old_fireballs)
{
    map<int, FireBall_t>::iterator target;
    int total_speed = 0;
    int total_num = old_fireballs.size();

    for (int i = 0; i < total_num; ++i) {
        target = fireballs.find(old_fireballs[i]);
        total_speed += target->second.speed;
    }
    return (total_speed / total_num);
}

bool isAllEvenOrOddDirection(vector<int> &old_fireballs)
{
    map<int, FireBall_t>::iterator target;
    
    int first_is_odd = (fireballs.find(old_fireballs[0])->second.direction % 2);

    for (int i = 1; i < old_fireballs.size(); ++i) {
        target = fireballs.find(old_fireballs[i]);
        if (target->second.direction % 2 != first_is_odd)
            return false;
    }
    return true;
}

void updateNewFireball(int row, int col, vector<int> &old_fireballs)
{
    static int NEW_FIREBALL_CNT = 4;
    int total_num = old_fireballs.size();
    int new_mass = getNewMass(old_fireballs);
    int new_speed = getNewSpeed(old_fireballs);
    int is_all_even_or_odd_direction = isAllEvenOrOddDirection(old_fireballs);
    map<int, FireBall_t>::iterator old_fireball;

    if (new_mass > 0) {
        if (total_num < NEW_FIREBALL_CNT) {
            for (int i = 0; i < total_num; ++i) {
                old_fireball = fireballs.find(old_fireballs[i]);
                old_fireball->second.mass = new_mass;
                old_fireball->second.speed = new_speed;
                old_fireball->second.direction = new_direction_case[is_all_even_or_odd_direction][i];
            }
            int last_idx = (--fireballs.end())->first;
            for (int i = total_num; i < NEW_FIREBALL_CNT; ++i) {
                FireBall_t new_fireball;

                new_fireball.r = row;
                new_fireball.c = col;
                new_fireball.mass = new_mass;
                new_fireball.speed = new_speed;
                new_fireball.direction = new_direction_case[is_all_even_or_odd_direction][i];
                fireballs.insert({++last_idx, new_fireball});
            }
        }
        else if (total_num > NEW_FIREBALL_CNT) {
            for (int i = 0; i < NEW_FIREBALL_CNT; ++i) {
                old_fireball = fireballs.find(old_fireballs[i]);
                old_fireball->second.mass = new_mass;
                old_fireball->second.speed = new_speed;
                old_fireball->second.direction = new_direction_case[is_all_even_or_odd_direction][i];
            }
            for (int i = NEW_FIREBALL_CNT; i < total_num; ++i) {
                fireballs.erase(old_fireballs[i]);
            }
        }
        else {
            for (int i = 0; i < total_num; ++i) {
                old_fireball = fireballs.find(old_fireballs[i]);
                old_fireball->second.mass = new_mass;
                old_fireball->second.speed = new_speed;
                old_fireball->second.direction = new_direction_case[is_all_even_or_odd_direction][i];
            }
        }
    }
    else {
        for (int i = 0; i < old_fireballs.size(); ++i) {
            fireballs.erase(old_fireballs[i]);
        }
    }
}

void checkNumberAndTrigger(void (*func)(int row, int col, vector<int> &old_fireballs))
{
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j].size() > 1) {
                func(i, j, board[i][j]);
            }
        }
    }
}

int main(void)
{
    int answer = 0;

    scanf("%d %d %d", &n, &m, &k);
    for (int i = 0; i < m; ++i) {
        FireBall_t new_fireball;

        scanf("%d %d %d %d %d", &(new_fireball.r), &(new_fireball.c), \
                &(new_fireball.mass), &(new_fireball.speed), &(new_fireball.direction));
        new_fireball.r--;
        new_fireball.c--;
        fireballs.insert({i, new_fireball});
    }
    createBoard();
    for (int i = 0; i < k; ++i) {
        for (map<int, FireBall_t>::iterator iter = fireballs.begin(); \
                iter != fireballs.end(); ++iter) {
            moveFireball(iter->second);
        }
        updateBoard();
        checkNumberAndTrigger(updateNewFireball);
    }
    for (map<int, FireBall_t>::iterator iter = fireballs.begin(); \
            iter != fireballs.end(); ++iter) {
        answer += iter->second.mass;
    }
    printf("%d\n", answer);
    return 0;
}