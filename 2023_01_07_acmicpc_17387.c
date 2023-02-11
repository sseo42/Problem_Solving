#include <stdio.h>

/**
 * Problem
 *      선분 교차 문제
 * 
 *      -1,000,000 <= coord <= 1,000,000
 * 
 *      (x1, y1, x2, y2)
 *      (x3, y3, x4, y4)
 * 
 *      가 주어진다.
 * 
 * Ideas
 *      선분 교차, 벡터곱이 0 이면 같은 직선 안에 있는 것이다.
 * 
 *      (2, 0) x (-1, 0) = 0
 * 
 *      (2, 0) x (2, 0) = 0
 * 
 * Comments
 *      제대로 집중을 못하는 것 같다. 잡생각이 많은 듯...
 *      1. 곱연산을 쓰면서 범위를 고려하지 않았다. 두번이나 간과함...
 *      2. 타이핑 미스
 * 
 *      태도를 바꿀 필요가 좀 있다.
 */

int cross_product(int v_x1, int v_y1, int v_x2, int v_y2)
{
    long long res = (long long)v_x1 * v_y2 - (long long)v_x2 * v_y1;

    if (res < 0)
    {
        return -1;
    }
    else if (res > 0)
    {
        return 1;
    }
    return 0;
}

int is_in_the_zone(int x1, int y1, int x2, int y2, int a, int b)
{
    int tmp;

    if (x1 > x2) {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y1 > y2) {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    return ((x1 <= a && a <= x2) && (y1 <= b && b <= y2));
}

int main(void)
{
    int x1, y1, x2, y2;
    int x3, y3, x4, y4;
    int cross_res1, cross_res2, cross_res3, cross_res4;

    scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
    scanf("%d %d %d %d", &x3, &y3, &x4, &y4);

    cross_res1 = cross_product(x2 - x1, y2 - y1, x3 - x2, y3 - y2);
    cross_res2 = cross_product(x2 - x1, y2 - y1, x4 - x2, y4 - y2);
    cross_res3 = cross_product(x4 - x3, y4 - y3, x1 - x4, y1 - y4);
    cross_res4 = cross_product(x4 - x3, y4 - y3, x2 - x4, y2 - y4);

    if ((cross_res1 * cross_res2 < 0 && cross_res3 * cross_res4 < 0) ||
        (cross_res1 == 0 && is_in_the_zone(x1, y1, x2, y2, x3, y3)) ||
        (cross_res2 == 0 && is_in_the_zone(x1, y1, x2, y2, x4, y4)) ||
        (cross_res3 == 0 && is_in_the_zone(x3, y3, x4, y4, x1, y1)) ||
        (cross_res4 == 0 && is_in_the_zone(x3, y3, x4, y4, x2, y2)))
    {
        printf("1\n");
        return 0;
    }
    printf("0\n");
}