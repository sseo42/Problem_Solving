#include <stdio.h>
#include <string.h>

/**
 * Problem
 *      문자열 매칭 문제,
 *      문자열 n 와 찾고자하는 패턴 m 가 주어진다.
 * 
 *      n, m의 길이는 [1, 100] 이고, 알파벳 대문자와 공백으로 이루어져있다.
 * 
 *      출력으로 n 에서 m 이 몇변 등장하는지 출력하고
 *      둘째줄에 m 이 나타나는 위치들을 공백으로 구분해 출력하자
 *      (n 의 i ~ i+m-1 과 m 이 일치한다면 i 를 출력하는 것이다)
 * 
 * Ideas
 *      KMP 구현.
 * 
 * Comments
 *      틀린 패턴.
 * 
 *      1. 입력 유형 미숙지.
 *      2. 테스트 print 안 지움.
 *      3. 입력 한계 미숙지.
 *      4. 배열 타입 잘못 선언
 *      5. 입력의 buf size 설정 미숙지.
 * 
 *      차분하게 문제 읽고, 생각하고 풀자
 */

#define MAX_STRING 1000000

int n, m;
char input_string[MAX_STRING + 2];
char word[MAX_STRING + 2];
int pattern[MAX_STRING + 1];

void fill_pattern()
{
    int j = 0;

    for (int i = 1; i < m; ++i) {
        while (j > 0 && word[i] != word[j]) {
            j = pattern[j - 1];
        }
        if (word[i] == word[j]) {
            pattern[i] = ++j;
        }
    }
}

int main()
{
    int j = 0;
    int cnt = 0;
    int idx[MAX_STRING + 1];

    fgets(input_string, MAX_STRING + 2, stdin);
    fgets(word, MAX_STRING + 2, stdin);
    n = (int)strlen(input_string);
    m = (int)strlen(word);
    if (input_string[n - 1] == '\n') {
        input_string[n - 1] = '\0';
        --n;
    }
    if (word[m - 1] == '\n') {
        word[m - 1] = '\0';
        --m;
    }
    fill_pattern();
    for (int i = 0; i < n; ++i) {
        while (j > 0 && input_string[i] != word[j]) {
            j = pattern[j - 1];
        }
        if (input_string[i] == word[j]) {
            ++j;
         }
        if (j == m) {
            idx[cnt] = i + 1 - m + 1;
            ++cnt;
            j = pattern[j - 1];
        }
    }
    printf("%d\n", cnt);
    for (int i = 0; i < cnt; ++i) {
        printf("%d ", idx[i]);
    }
}