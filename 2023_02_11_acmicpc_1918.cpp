#include <iostream>
#include <stack>

using namespace std;

/**
 * Problem
 *      알파벳과 '+' '-' '*' '/' '(' ')' 으로 이루어진 중위표기식이 주어진다.
 *      이를 후위표기식으로 바꿔 출력하자. 입력의 길이는 100을 넘지 않는다.
 * 
 * Ideas
 *      string 을 stack 에 저장하며 나아가자
 * 
 *      괄호는 재귀적으로 접근하여 string 을 받아오자.
 *      그럼 단어와 '+' '-' '*' '/' 로만 이루어진 문제가된다.
 * 
 *      처음은 {단어, ""} 로 push, 다음부터는 기호 + 단어로 읽어 {단어, "+"} 과 같이 push
 *      만약 단어부에 "(" 가 온다면 재귀를 통해 단어 받아오자.
 *      기호부가 '*' 이라면 앞 stack 단어를 꺼내서 자신앞에 붙이고 
 *      -> 뒤에서 부터 봐야한다.
 * 
 * Comments
 *      같은 우선순위라면 앞에가 우선 순위를 가진다 따라서 앞에서부터 봐야한다.
 *      string 에 "" 더하는 것과 null(char) 를 더하는 것은 다르다...
 *      ""(nullptr) 를 더하는 것은 사이즈의 변화가 없지만, null(char)를 더하는 것은 사이즈가 1증가하며
 *      string 이 관리하는 버퍼의 공간을 잡아먹는다.
 * 
 *      cout 의 경우 string 의 모든 버퍼를 찍기에 null이 들어간 위치 뒤의 값도 찍히지만(중간 null을 포함해),
 *      printf 로 찍을 경우 null 까지 찍기때문에 뒤에는 쓰레기 값이된다.
 */

pair<string, int> read_number(int idx);

string input_string;

pair<string, int> convert_to_postfix(int idx)
{
    stack<pair<string, string> > stk;
    stack<pair<string, string> > stk_rev;
    string res = "";
    pair<string, int> number = read_number(idx);

    idx = number.second;
    stk.push({number.first, ""});
    while (idx < input_string.size() && input_string[idx] != ')') {
        char op[2] = {'\0', '\0'};
        
        op[0] = input_string[idx];
        ++idx;
        number = read_number(idx);
        
        if (op[0] == '/' || op[0] == '*') {
            stk.top().first = stk.top().first + number.first + string(op);
        } else if (op[0] == '+' || op[0] == '-') {
            stk.push({number.first, string(op)});
        } else {
            exit(1);
        }
        idx = number.second;
    }
    while (stk.empty() == false) {
        stk_rev.push(stk.top());
        stk.pop();
    }
    while (stk_rev.empty() == false) {
        res = res + stk_rev.top().first + stk_rev.top().second;
        stk_rev.pop();
    }
    return make_pair(res, idx);
}

pair<string, int> read_number(int idx)
{
    string res = "";

    if (input_string[idx] == '(') {
        pair<string, int> pharen = convert_to_postfix(idx + 1);
        res = pharen.first;
        if (input_string[pharen.second] != ')') {
            exit(1);
        }
        ++pharen.second;
        return pharen;
    }
    while (idx < input_string.size() && input_string[idx] >= 'A' && input_string[idx] <= 'Z') {
        res = input_string[idx] + res;
        ++idx;
    }
    return make_pair(res, idx);
}

int main()
{
    cin >> input_string;

    pair<string, int> ans = convert_to_postfix(0);
    printf("%s\n", ans.first.c_str());
}