#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <map>

using namespace std;

/**
 * Problem
 *      r, c, k 가 주어진다. 배열 A[r][c] == k 가 되는 최소시간 t를 출력하자.
 *      행의 개수 >= 열의 개수 일 때, R 연산을 수행한다.
 *      그렇지 않을 때, C 연산을 수행한다.
 * 
 *      연산은 해당 행 혹은 열에 숫자가 몇개 있는지 계산하여 다시 집어넣는 식이다.
 *      갯수가 오름차순이 되도록하며 갯 수가 같은 때 해당 값이 오름차순이 되도록한다.
 *      연산결과 가장 긴 행 혹은 열에 맞춰 나머지 행 혹은 열에 0 을 채워 넣는다.
 * 
 *      배열의 사이즈가 100보다 커질 경우 처음 100개를 제외한 나머지는 버린다.
 *      시간 t 가 100에 도달해도 A[r][c] == k 를 만족하지 못한다면 -1 을 출력한다.
 * 
 * Ideas
 *      t를 증가시키며 행/열의 갯수를 따져 연산을 실행하고 체크한다.
 *      만족하면 리턴 t가 100을 달성할시 루프를 탈출하며 -1리턴한다.
 *      연산을 위해 pair를 vector에 저장하고 sort 하는 과정을 거친다.
 * 
 *      필요한 자료구조는 다음과 같을 것이다.
 *      행의 크기와 열의 크기를 기록한 변수 2개.
 *      100x100 사이즈의 배열 기록판
 *      숫자와 등장횟수를 저장할 map.
 *      map으로부터 등장횟수와 숫자를 꺼내 기록할 백터
 *      t를 저장할 변수.
 * 
 *      구현 시작.
 * 
 *      테케 실패.
 *      초기화 위치가 잘못됨.
 *      0 다음에 숫자가 나올 수 있음 0가 나오면 끝이 아님.
 *      col_size, row_size 갱신 스코프가 잘못됨.
 *      2칸씩 뛰는 for 루프에서 인덱스 접근이 잘못됨.
 * 
 * Comments
 *      구현에 시간이 오래걸림.
 *      자료구조에 대한 미숙한 이해와 루프를 돌면서 인덱스접근을 헷갈린 부분이 크다. 명확하게 머리에 담아야겠다.
 * 
 *      에러케이스가 많았음.
 *      스코프를 잘못 잡고 들어가는 문제가 가장 컸다.
 *      작업순서에 대해 명확한 정리가 안된채 구현을 들어갔기 때문, 변수 혹은 구조를 어느 스코프 동안 사용할지 체크하는 과정이 필요하다.
 */

typedef pair<int, int> p;

int r, c, k;
int row_size = 3, col_size = 3;
int arr[100][100];

int solution(void)
{
    map<int, int> mp;
    vector<p> v;
    map<int, int>::iterator mp_it;
    int tmp;
    int t = 0;

    while (t <= 100)
    {
        if (arr[r][c] == k)
            return t;
        t++;
        tmp = 0;
        if (row_size >= col_size)
        {
            for (int i = 0; i < row_size; ++i)
            {
                mp.clear();
                v.clear();
                for (int j = 0; j < col_size; ++j)
                {
                    if (arr[i][j] == 0)
                        continue ;
                    else if (mp.find(arr[i][j]) != mp.end())
                        mp[arr[i][j]]++;
                    else
                        mp.insert({arr[i][j], 1});
                }
                for (mp_it = mp.begin(); mp_it != mp.end(); ++mp_it)
                {
                    v.push_back({mp_it->second, mp_it->first});
                }
                sort(v.begin(), v.end());
                int j = 0;
                for (; j < 50 && j < v.size(); ++j)
                {
                    arr[i][j * 2] = v[j].second;
                    arr[i][j * 2 + 1] = v[j].first;
                }
                if (j * 2 > tmp)
                    tmp = j * 2;
                for (; j < 50; ++j)
                {
                    arr[i][j * 2] = 0;
                    arr[i][j * 2 + 1] = 0;
                }
            }
            col_size = tmp;
        }
        else
        {
            for (int i = 0; i < col_size; ++i)
            {
                mp.clear();
                v.clear();
                for (int j = 0; j < row_size; ++j)
                {
                    if (arr[j][i] == 0)
                        continue ;
                    else if (mp.find(arr[j][i]) != mp.end())
                        mp[arr[j][i]]++;
                    else
                        mp.insert({arr[j][i], 1});
                }
                for (mp_it = mp.begin(); mp_it != mp.end(); ++mp_it)
                {
                    v.push_back({mp_it->second, mp_it->first});
                }
                sort(v.begin(), v.end());
                int j = 0;
                for (; j < 50 && j < v.size(); ++j)
                {
                    arr[j * 2][i] = v[j].second;
                    arr[j * 2 + 1][i] = v[j].first;
                }
                if (j * 2 > tmp)
                    tmp = j * 2;
                for (; j < 50; ++j)
                {
                    arr[j * 2][i] = 0;
                    arr[j * 2 + 1][i] = 0;
                }
            }
            row_size = tmp;
        }
    }
    return -1;
}

int main(void)
{
    scanf("%d %d %d", &r, &c, &k);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            scanf("%d", &(arr[i][j]));
        }
    }
    r--; c--;
    printf("%d\n", solution());
    return 0;
}