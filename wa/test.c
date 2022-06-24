#include <stdio.h>

// int g(int *c, int *p)
// {
//     return 1;
// }
// int f(int a, int b)
// {
//     int c[3], d, e;
//     d = a + 1;
//     e = g(c, &b);
//     return e + c[1] + b;
// }

int f1(int *arr, int n)
{
    int sum;
    int i;
    for (i = 0; i < n; ++i)
    {
        sum += arr[i];
    }
    return sum;
}

int main()
{
    // f(1, 2);

    int d[10][10];
    for (int k = 0; k < 10; k++)
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                if (d[i][j] > d[i][k] + d[k][j])
                    d[i][j] = d[i][k] + d[k][j];

    return 0;
}