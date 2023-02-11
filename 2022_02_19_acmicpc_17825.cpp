#include <iostream>
#include <vector>
#include <stack>

using namespace std;

/**
 * Problem
 *      처음 시작시 4개의 말을 가진다.
 *      1~5 까지의 수가 적힌 오면체 주사위를 굴려 나온 값 만큼 하나의 말을 선택해 이동해야한다.
 *      시작점이 파란색일경우 파란 화살표를 그렇지 않을 경우 빨간 화살표를 따른다.
 *      말이 이동을 마치는 칸에 다른 말이 있다면 그 말은 고를 수 없다.
 * 
 *      말이 도착칸으로 이동 한다면 나온 수와 관계없이 이동을 마친다.
 *      말이 이동을 마칠 때 마다 칸에 적힌 점수를 얻는다.
 * 
 *      10번의 주사위 값 순서가 주어 질때 점수의 최댓값을 구하자.
 * 
 * Ideas
 *      한 주사위 당 4개의 말 중 하나를 선택해 움직임 -> 4^10 의 경우의 수가 나온다.
 *      백트래킹으로 전부 순회하여 값을 구하자.
 * 
 *      맵은 고정이므로 맵을 전역 array 로 들고 있자.
 *      array 에는 점수와 화살표가 가리키는 지점을 저장하며 이를 [2] 로하여 idx1은 빨강화살표
 *      idx2는 파란 화살표 지점을 가지자. (빨강 지점에는 idx1 = idx2 로 둔다)
 * 
 *      말은 이동 시 최초 idx2 로 타고 나머지는 idx1로 이동한다.
 * 
 *      말은 현재 및 위치 이력을 stack으로 들고 있으며 전체는 vector로 관리하자.
 *      또한 stack을 init 할 때 시작지점을 넣어주자. 도착지점은 뒤에 4개를 더들고 있자
 *      이를 통해서 도착지에 도달한 말의 백트래킹이 가능해진다.(주사위값 유실 방지)
 * 
 *      전역 변수로 다음을 두자
 *      section_spec :  value
 *                      red_arrow
 *                      blue_arrow
 * 
 *      section_spec board[];
 * 
 *      vector<stack<int> >     unit_location_hist;
 *                      
 *      필요한 함수는 다음과 같다.
 *      장기 말의 이동 : forward_unit(int unit_id, int move_point)
 *      장기 말 되돌리기 : backward_unit(int unit_id, int move_point)
 * 
 *      초기 말 위치 초기화 : init_location_hist()
 * 
 *      솔루션 : get_highest_score(int turn)
 * 
 * Comments
 *      이동한 위치에 말이 있을 경우 이동 불가능한 경우를 빼먹고 코드를
 *      작성해서 디버깅의 시간이 생겨버렸다. 또한 보드를 작성함에 있어서 위치 인덱스를
 *      잘못 적어 시간이 소모되었다.
 * 
 *      + 테스트케이스 인풋을 잘못입력하여 추가적인 디버깅 시간이 발생했다.
 *      코드를 방어적으로 작성하여 스스로의 잘못을 빠르게 찾을 수 있도록하자.
 */

#define UNIT_AMOUNT 5
#define END_SECTION 32
#define END_TURN 10

typedef struct s_section_spec section_spec;
struct s_section_spec
{
    int value;
    int red_arrow;
    int blue_arrow;
};

int dice_point[END_TURN];

section_spec board[] = {
                        {0, 1, 1}, {2, 2, 2}, {4, 3, 3}, {6, 4, 4}, {8, 5, 5},
                        {10, 6, 20}, {12, 7, 7}, {14, 8, 8}, {16, 9, 9}, {18, 10, 10},
                        
                        {20, 11, 23}, {22, 12, 12}, {24, 13, 13}, {26, 14, 14}, {28, 15, 15},
                        {30, 16, 25}, {32, 17, 17}, {34, 18, 18}, {36, 19, 19}, {38, 31, 31},
                        
                        {13, 21, 21}, {16, 22, 22}, {19, 28, 28}, {22, 24, 24}, {24, 28, 28},
                        {28, 26, 26}, {27, 27, 27}, {26, 28, 28}, {25, 29, 29}, {30, 30, 30},
                        
                        {35, 31, 31}, {40, 32, 32}, {0, 33, 33}, {0, 34, 34}, {0, 35, 35}, 
                        {0, 36, 36}, {0, 37, 37}
                       };

vector<stack<int> > unit_location_hist;

int get_dest_location(int unit_id, int move_point)
{
    int unit_location = unit_location_hist[unit_id].top();

    if (unit_location >= END_SECTION)
        return -1;
    unit_location = board[unit_location].blue_arrow;
    for (int i = 1; i < move_point; ++i) {
        unit_location = board[unit_location].red_arrow;
    }
    if (unit_location < END_SECTION) {
        for (int i = 0; i < UNIT_AMOUNT; ++i) {
            if (unit_location_hist[i].top() == unit_location)
                return -1;
        }
    }
    return unit_location;
}

int forward_unit(int unit_id, int location)
{
    unit_location_hist[unit_id].push(location);
    return board[location].value;
}

int backward_unit(int unit_id)
{
    int section_value = unit_location_hist[unit_id].top();

    unit_location_hist[unit_id].pop();
    return section_value;
}

void init_location_hist(void)
{
    unit_location_hist.resize(UNIT_AMOUNT);
    for (int i = 0; i < UNIT_AMOUNT; ++i) {
        unit_location_hist[i].push(0);
    }
}

int get_highest_score(int turn)
{
    int max_score = 0;

    if (turn >= END_TURN) {
        return max_score;
    }

    int score = 0;
    int move_point = dice_point[turn];
    int dest_location;
    
    for (int i = 0; i < UNIT_AMOUNT; ++i) {
        dest_location = get_dest_location(i, move_point);
        if (dest_location >= 0) {
            score = forward_unit(i, dest_location);
            score += get_highest_score(turn + 1);
            max_score = max(score, max_score);
            backward_unit(i);
        }
    }
    return max_score;
}

int main(void)
{
    for (int i = 0; i < END_TURN; ++i) {
        scanf("%d", &(dice_point[i]));
    }
    init_location_hist();

    printf("%d\n", get_highest_score(0));
    return 0;
}