#include <iostream>
#include <string>
#include <cmath>

using namespace std;

/**
 * Problem
 *      1 <= n <= 1,000,000,000
 * 
 *      이 주어진다 1 부터 시작해 n 까지 각 숫자[0,9]가 몇번 나오는지 출력하자.
 * 
 * Ideas
 *      a 자리의 모든 수에서 각 숫자는 몇번 등장하게 될까?
 * 
 *      a = 1 일 경우 [1,9] 가 1번씩 등장한다.
 * 
 *      a = 2 일 경우에는 10 ... 19, 20 ... 29 ,,, 90 ... 99 이다.
 * 
 *      10의 자리에서는 [1, 9] 까지 각 10번씩 등장하고, 1의 자리에서는 [0,9] 까지 각 9 번 등장한다.
 * 
 *      a = 3 일 경우에는 100 ... 109 ... 199 ... 999 이다.
 * 
 *      100의 자리에서는 [1, 9] 까지 각 100번씩 등장하고, 10의 자리에서는 [0, 9] 까지 10 * 9 번 등장한다. 1의 자리는 [0, 9] 9 * 10 등징한다.
 * 
 *      이를 통해 길이가 n 인 모든 수에서 각 숫자는 {0이 아닐 경우} 10^(n - 1) + 9 * 10^(n - 2) + ... 9 * 10 이다.
 * 
 *      따라서 임의의 수가 주어질 떄, 이 수 의 길이가 n 이고 각 자리의 수가 A<자리> 라 하자.
 * 
 *      a 자리      : [0, 9] * (A<a> - 1) (상위 자리수의 합 + 1 - 10^(상위 자리수 - 1))  &&  
 * 
 * 
 * [1, A<a> - 1] = 10^(n-1) / A<n> = 해당 자리수 및의 값 + 1
 * 
 *      n - 1 자리  : [0, A<n-1> - 1] = (A<n> - 1) * 10^(n-2)
 * 
 *      --------------------------------------------------------------------------------------------------------------
 *      타이트하게 생각하면 점화식 세우기에 머리가 아프다... 0 로 패딩한 상태로 세우고 0 카운트 해서 빼자.
 * 
 *      a 자리      : [0, 9] -> (상위 자리수의 합) * 10^(a-1) | [0,A<a-1>] -> 10^(a-1) | A<a> -> 하위 자리수 합 + 1
 * 
 *      그럼 전체 패딩된 0는 몇 개일까? -> 자리에 의존한다.
 * 
 *      n 개 * 1
 * 
 *      n - 1 개 * 9
 * 
 *      n - 2 개 * 90
 *      ...
 * 
 *      1 개 * 9 * 10^(n-2)
 * 
 * Comments
 *      식을 세울 떄 핵심을 보지 못해 겉돌았다, 이상하게 풀게되면 온갖 케이스를 다 따지게 되면서 머리만 복잡해지는 둣하다.
 * 
 *      문제 없이 마무리는 됬지만, 식을 세울 때 표현이 서투른 거 같다 스스로도 헷갈려서 추가적인 시간 소모가 발생했다.
 */

long long number_counter[10];


int main()
{
    int number_str_len;
    string number_str;

    cin >> number_str;
    number_str_len = number_str.length();
    for (int i = 0; i < number_str_len; ++i) {
        int current_number = stoi(number_str.substr(i, 1));

        if (i > 0) {
            int counter = stoi(number_str.substr(0, i)) * pow(10, number_str_len - i - 1);

            for (int counter_pos = 0; counter_pos <= 9; ++counter_pos) {
                number_counter[counter_pos] += counter;
            }
        }
        
        for (int counter_pos = 0; counter_pos < current_number; ++counter_pos) {
            number_counter[counter_pos] += pow(10, number_str_len - i - 1);
        }
        number_counter[current_number] += 1;
        if (i < number_str_len - 1) {
            number_counter[current_number] += stoi(number_str.substr(i + 1, number_str_len - i));
        }
    }

    number_counter[0] -= number_str_len;
    for (int i = 1; i < number_str_len; ++i) {
        number_counter[0] -= (number_str_len - i) * 9 * pow(10, i - 1);
    }

    for (int i = 0; i <= 9; ++i) {
        cout << number_counter[i] << ' ';
    }
    cout << '\n';
}