#include <iostream>
#include <cstring>

using namespace std;

/**
 * Problem
 *      최대 10줄에 거쳐서 [1,1000000] 길이의 문자열 s가 주어진다.
 *      마지막 줄은 마침표가 온다.
 * 
 *      s = a ^ n (부분문자열 a 가 n 번 반복되는 경우) 를 만족하는 최대 n
 *      을 찾은 뒤 이를 출력하자.
 * 
 * Ideas
 *      욱진이와 같이 얘기했던 문제 KMP를 사용해 구성할 때 마지막 인덱스 pi 값이
 *      접두사와 접미사가 겹치는 최대 길이이므로 총길이에서 이를 빼면 a의 최소 길이를 알 수 있다.
 * 
 * Comments
 *      다음 부분을 주의해서 수정해야겠다.
 * 
 *      KMP 의 마지막은 접두, 접미의 반복이다.
 *      문자열이 반복이라면 이는 처음과 마지막 사이의 반복을 포함할 것이다. 따라서 전체 len 에서 뺌으로
 *      반복되는 문자열을 알수 있다. 하지만 이는 후보일 뿐, 실제 반복문자열인지는 확인이 필요하다.
 *      후보의 반복으로 전체를 꽉 채울수 있는 지롤 봐야한다, 만약 중간이 떳다면 후보는 전체의 과반을 구성
 *      할 것이기에 (b + a) > a, 꽉 못채우고 반복이 아닌 대칭형이라면 자투리가 남아 못채운다.
 * 
 *      1. 엣지 케이스에 대해 항상 생각하자. 해법을 찾는 과정은 힘들지만, 찾은 뒤에도 엣지케이스는 신경 써줘야한다.
 * 
 *      2. 케이스와 결과에 대해 주의하자, 가볍게 생각하고 추정하면 머피의 법칙이 깃들기 마련이다.
 * 
 *      3. 초기화를 신경쓰자, 언듯 초기화가 영향을 안 미친다 생각할 수 있지만, 절묘하게 영향을 끼질수 있다.
 */

#define MAX_STRING 1000000

int pattern[MAX_STRING + 1];
string input_str;

void fill_pattern()
{
    int j = 0;

    for (size_t i = 1; i < input_str.length(); ++i) {
        while (j > 0 && input_str[i] != input_str[j]) {
            j = pattern[j - 1];
        }
        if (input_str[i] == input_str[j]) {
            pattern[i] = ++j;
        }
    }
}

int get_min_n()
{
    int joint_area = pattern[input_str.length() - 1];

    if (input_str.length() % (input_str.length() - joint_area)) {
        return 1;
    }
    return input_str.length() / (input_str.length() - joint_area);
}

int main()
{
    cin >> input_str;
    while (input_str != ".") {
        memset(pattern, 0, sizeof(int) * input_str.length());
        fill_pattern();
        printf("%d\n", get_min_n());
        input_str.clear();
        cin >> input_str;
    }
}