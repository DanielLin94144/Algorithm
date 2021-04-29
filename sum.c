#include <stdio.h> 

int Sum(int *A, int i, int n)
{
    if (i == n) {
        return A[n];
    }
    else {
        return A[i] + Sum(A, i + 1, n);
    }
}

int main(void)
{
    int A[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int s = Sum(A, 0, 9);
    printf("%d\n", s);

}