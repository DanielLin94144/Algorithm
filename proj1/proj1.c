// EE3980 Project 1. Coin Picking Game
// 106012017, 林冠廷
// 2021/05/29

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* global variable declaration */
int m = 1000000;        // number of games played 
int n = 10;             // number of coins 
// -1: choose low; 1: choose right
int **d;
int **d_choice;
bool done_init; 
bool done_call;
/* function declaration */
int CoinLeft(int A[], int low, int high); // selecting left coin
int CoinRandom(int A[], int low, int high); // random selection
int CoinGreedy(int A[], int low, int high); // greedy approach
int CoinDP(int A[], int low, int high); // dynamic programming
int DP_TD(int A[], int low, int high); 
int max(int v1, int v2);
int min(int v1, int v2); 
double compete(int FX1(), int FX2(), int A[], int c[]);

/* main function */ 
int main(void)
{
    int c[4] = {1, 5, 10, 50};
    int A[10];
    int i;
    int j; 
    double win1_ratio; 
    
    srand(time(0));
    // Question 1
    printf("Question 1\n");
    win1_ratio = compete(CoinLeft, CoinLeft, A, c);
    printf("  CoinLeft vs. CoinLeft: %g\n", win1_ratio);
    win1_ratio = compete(CoinLeft, CoinRandom, A, c);
    printf("  CoinLeft vs. CoinRandom: %g\n", win1_ratio);
    win1_ratio = compete(CoinRandom, CoinLeft, A, c);
    printf("  CoinRandom vs. CoinLeft: %g\n", win1_ratio);

    // Question 2
    printf("Question 2\n");
    // player 1: CoinLeft
    win1_ratio = compete(CoinLeft, CoinLeft, A, c);
    printf("  CoinLeft vs. CoinLeft: %g\n", win1_ratio);
    win1_ratio = compete(CoinLeft, CoinRandom, A, c);
    printf("  CoinLeft vs. CoinRandom: %g\n", win1_ratio);
    win1_ratio = compete(CoinLeft, CoinGreedy, A, c);
    printf("  CoinLeft vs. CoinGreedy: %g\n", win1_ratio);
    win1_ratio = compete(CoinLeft, CoinDP, A, c);
    printf("  CoinLeft vs. CoinDP: %g\n", win1_ratio);
    // player 1: CoinRandom
    win1_ratio = compete(CoinRandom, CoinLeft, A, c);
    printf("  CoinRandom vs. CoinLeft: %g\n", win1_ratio);
    win1_ratio = compete(CoinRandom, CoinRandom, A, c);
    printf("  CoinRandom vs. CoinRandom: %g\n", win1_ratio);
    win1_ratio = compete(CoinRandom, CoinGreedy, A, c);
    printf("  CoinRandom vs. CoinGreedy: %g\n", win1_ratio);
    win1_ratio = compete(CoinRandom, CoinDP, A, c);
    printf("  CoinRandom vs. CoinDP: %g\n", win1_ratio);
    // player 1: CoinGreedy
    win1_ratio = compete(CoinGreedy, CoinLeft, A, c);
    printf("  CoinGreedy vs. CoinLeft: %g\n", win1_ratio);
    win1_ratio = compete(CoinGreedy, CoinRandom, A, c);
    printf("  CoinGreedy vs. CoinRandom: %g\n", win1_ratio);
    win1_ratio = compete(CoinGreedy, CoinGreedy, A, c);
    printf("  CoinGreedy vs. CoinGreedy: %g\n", win1_ratio);
    win1_ratio = compete(CoinGreedy, CoinDP, A, c);
    printf("  CoinGreedy vs. CoinDP: %g\n", win1_ratio);
    // player 1: CoinDP
    win1_ratio = compete(CoinDP, CoinLeft, A, c);
    printf("  CoinDP vs. CoinLeft: %g\n", win1_ratio);
    win1_ratio = compete(CoinDP, CoinRandom, A, c);
    printf("  CoinDP vs. CoinRandom: %g\n", win1_ratio);
    win1_ratio = compete(CoinDP, CoinGreedy, A, c);
    printf("  CoinDP vs. CoinGreedy: %g\n", win1_ratio);
    win1_ratio = compete(CoinDP, CoinDP, A, c);
    printf("  CoinDP vs. CoinDP: %g\n", win1_ratio);

    return 0; 
}

/* function implementation */ 
// output low or high indicating picking left or right
int CoinLeft(int A[], int low, int high)
{
    return low;
}

int CoinRandom(int A[], int low, int high)
{
    // rand() % 2 is either 0 or 1 
    if (rand() % 2 == 0) {
        return low; 
    }
    else {
        return high;
    }
}

int CoinGreedy(int A[], int low, int high)
{
    if (A[low] > A[high]) {
        return low;
    }
    else {  // include equal
        return high;
    }
}

int min(int v1, int v2)
{
    if (v1 < v2) {
        return v1;
    }
    else {
        return v2;
    }
}

int max(int v1, int v2)
{
    if (v1 > v2) {
        return v1;
    }
    else {
        return v2;
    }
}

int CoinDP(int A[], int low, int high)
{
    int i, j;
    int len;
    int choose_left, choose_right;
    int Max; 

    if (!done_init) {
        // memory allocation of table d and d_choice
        d = (int**)malloc(n * sizeof(int*));
        for (i = 0; i <= n; i++) {
            d[i] = (int*)malloc(n * sizeof(int));
        }
        d_choice = (int**)malloc(n * sizeof(int*));
        for (i = 0; i <= n; i++) {
            d_choice[i] = (int*)malloc(n * sizeof(int));
        }
        // initialization of table d and d_choice
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                d[i][j] = 0;
                d_choice[i][j] = 0;
            }
        }
        done_init = true;
    }
    // call DP_TD(A, low, high); or use below bottom-up approach (DP_BU) 
    if (!done_call) {
        // DP_TD(A, low, high);
        // using bottom-up DP (DP_BU)
        for (i = 0; i < n; i++) {
            d[i][i] = A[i];
            j = i + 1;
            Max = max(A[i], A[j]);
            if (Max == A[i]) {
                d_choice[i][j] = -1;  // record to choose left
            }
            else {
                d_choice[i][j] = 1;  // record to choose right 
            }
            d[i][j] = Max;
        }
        for (len = 2; len < n; len++) {
            i = 0; 
            for (j = len; j < n; j++) {
                choose_left = A[i] + min(d[i + 2][j], d[i + 1][j - 1]);
                choose_right = A[j] + min(d[i][j - 2], d[i + 1][j - 1]);
                Max = max(choose_left, choose_right);     
                if (Max == choose_left) {
                    d_choice[i][j] = -1;  // record to choose left 
                }
                else {
                    d_choice[i][j] = 1;  // record to choose right 
                }
                d[i][j] = Max; // record ans
                i++;
            }
        }
        done_call = true;
    }
    // return the picking coin decision by d_choice
    if (d_choice[low][high] == -1) {
        return low;
    }
    else if (d_choice[low][high] == 1) {
        return high;
    }
}

int DP_TD(int A[], int low, int high)
{
    int choose_left;
    int choose_right;
    int Max; 

    // if already recorded in table d, return to avoid redundent function call
    if (d[low][high] > 0) {
        return d[low][high];
    }
    if (high == low) {  // one coin left
        d_choice[low][high] = 1;  
        // record to choose right, but it is actually useless 
        d[low][high] = A[low];
        return A[low];
    }
    if (high - 1 == low) {  // remain two coins
        Max = max(A[low], A[high]);
        if (Max == A[low]) {
            d_choice[low][high] = -1;  // record to choose left
        }
        else {
            d_choice[low][high] = 1;  // record to choose right 
        }
        d[low][high] = Max;
        return Max;
    }
    choose_left = A[low] + min(DP_TD(A, low + 2, high), 
                                DP_TD(A, low + 1, high - 1));
    choose_right = A[high] + min(DP_TD(A, low, high - 2), 
                                DP_TD(A, low + 1, high - 1));
    Max = max(choose_left, choose_right);     
    if (Max == choose_left) {
        d_choice[low][high] = -1;  // record to choose left 
    }
    else {
        d_choice[low][high] = 1;  // record to choose right 
    }
    d[low][high] = Max; // record ans
    return Max;
}

double compete(int FX1(), int FX2(), int A[], int c[])
{
    int win1 = 0;  // player 1 win times
    int win2 = 0;  // player 2 win times
    int g;  // loop index 
    int sum1, sum2;
    int low; 
    int high;
    int r;  // round index
    int k1, k2;  // current choice for player 1 and 2
    int i, j;  // loop index 

    for (g = 1; g <= m; g++) {
        for (i = 0; i < n; i++) {   // initialize A
            A[i] = c[rand() % 4];   // generate random coins to A
        }
        sum1 = 0; 
        sum2 = 0; 
        low = 0; 
        high = n - 1;
        done_init = false;
        done_call = false;
        for (r = 1; r <= 5; r++) {
            // player 1 choose coin first 
            k1 = FX1(A, low, high);
            if (k1 == low) {
                sum1 += A[low];
                low++;
            }
            else {
                sum1 += A[high];
                high--;
            }
            // player 2 choose coin later  
            k2 = FX2(A, low, high);
            if (k2 == low) {
                sum2 += A[low];
                low++;
            }
            else {
                sum2 += A[high];
                high--;
            }
        }
        // finish one game 
        if (sum1 > sum2) {
            win1++;
        }
        else if (sum1 < sum2) {
            win2++;
        }
        // do not count for even game 
    }
    return (double) win1 / ((double) win1 + (double) win2);
}

