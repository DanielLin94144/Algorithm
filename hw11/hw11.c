// EE3980 HW11 0/1 Knapsack Problem
// 106012017, 林冠廷
// 2021/06/05

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* function declaration */
double GetTime(void);   // get current time from linux system 

/* global variable declaration */
int R = 1;  // repeatation

/* main function */
int main(void)
{
    double t;   // CPU running time 
    int i;  // loop index
    int W;  // weight 
    int P;  // profit
    int N;  // number of item 
    int M;  // maximum loading weight 
    int *w; 
    int *p;
    char **name;

    scanf("%d %d", &N, &M);
    // memory allocation
    w = (int*)malloc(N * sizeof(int));
    p = (int*)malloc(N * sizeof(int));
    name = (char**)malloc(N * sizeof(char*));
    for (i = 0; i < N; i++) {
        name[i] = (char*)malloc(3 * sizeof(char));  // 2 char + 1 null char
    }
    // readlines 
    for (i = 0; i < N; i++) {
        scanf("%s %d %d", name[i], &w[i], &p[i]);
    }

    t = GetTime();
    for (i = 0; i < R; i++) {
        
    }
    t = (GetTime() - t) / R;

    printf("Pick items:\n")
    
    printf("N = %d, M = %d\n", N, M);
    // printf("Weight = %d, Profits = %d\n", W, P);
    printf("CPU time: %g sec\n", t);
    return 0;
}

/* function implementation */
// Getime: current time from linux
double GetTime(void)
{           
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}