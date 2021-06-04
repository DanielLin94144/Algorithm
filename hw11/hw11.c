// EE3980 HW11 0/1 Knapsack Problem
// 106012017, 林冠廷
// 2021/06/05

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* function declaration */
double GetTime(void);   // get current time from linux system 
void BKnap(int k, int cp, int cw);
double Bound(int cp, int cw, int k);
void HeapSort(double *p_div_w, int n);  
void Heapify(double *p_div_w, int i, int n);

/* global variable declaration */
int R = 1000;  // the number of repeatation
int *w; // w[1:n] weight list of n items
int *p; // p[1:n] profit list of n items
double *p_div_w; // the CP value for sorting other list
char **name;    // name[1:n][1:3] each row is name item
int *x; // global solution of including items or not (0: not; 1: include)
int *cx; // current solution of including items or not (0: not; 1: include)
int fp = 0;  // final profit, initialze to 0
int fw = 0;  // final weight, initialze to 0
int N;  // number of item 
int M;  // maximum loading weight 

/* main function */
int main(void)
{
    double t;   // CPU running time 
    double t_sort; 
    int i;  // loop index
    int W = 0;  // weight 
    int P = 0;  // profit

    // read the first line from data file
    scanf("%d %d", &N, &M);
    // memory allocation
    w = (int*)malloc(N * sizeof(int));
    p = (int*)malloc(N * sizeof(int));
    p_div_w = (double*)malloc(N * sizeof(double));
    name = (char**)malloc(N * sizeof(char*));
    x = (int*)malloc(N * sizeof(int));
    cx = (int*)malloc(N * sizeof(int));
    for (i = 0; i < N; i++) {
        name[i] = (char*)malloc(3 * sizeof(char));  // 2 char + 1 null char
    }
    // readlines from input file
    for (i = 0; i < N; i++) {
        scanf("%s %d %d", name[i], &w[i], &p[i]);
        p_div_w[i] = (double)p[i] / (double)w[i];
    }
    // preprocess list to non-increasing order by HeapSort()
    t_sort = GetTime();
    HeapSort(p_div_w, N);  
    t_sort = GetTime() - t_sort;
    // run experiments
    t = GetTime();
    for (i = 0; i < R; i++) {
        fp = 0;
        fw = 0;
        BKnap(0, 0, 0);
    }
    t = (GetTime() - t) / R;
    // print out the result 
    printf("Pick items:\n");
    P = 0;
    W = 0;
    for (i = 0; i < N; i++) {
        if (x[i] == 1) {
            W += w[i];
            P += p[i];
            printf("  %s %d %d\n", name[i], w[i], p[i]);
        }
    }
    printf("N = %d, M = %d\n", N, M);
    printf("Weight = %d, Profits = %d\n", W, P);
    printf("CPU time: %g sec\n", t + t_sort);
    
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

void BKnap(int k, int cp, int cw)
{
    int i; // loop index 
    
    // add kth item and set x[]
    if (cw + w[k] <= M) { // add item but not exceed M
        cx[k] = 1;
        if (k < N - 1) { // traverse next
            BKnap(k + 1, cp + p[k], cw + w[k]);  
        }
        // traverse to the end of state tree
        else if (k == N - 1 && cp + p[k] > fp) {  
            fp = cp + p[k];
            fw = cw + w[k];
            // record solution to x 
            for (i = 0; i < N; i++) {
                x[i] = cx[i];
            }
        }
    }
    // not add kth item but use Bound function to check if keep traverse
    if (Bound(cp, cw, k) >= fp) {
        cx[k] = 0;  // do not add kth item
        if (k < N - 1) {
            BKnap(k + 1, cp, cw);
        }
        else if (k == N - 1 && cp > fp) {
            fp = cp;
            fw = cw;
            // record solution to x 
            for (i = 0; i < N; i++) {
                x[i] = cx[i];
            }
        }
    }
}
// Bound: estimate the maximum profit given current k, profit and weight
double Bound(int cp, int cw, int k) 
{
    // initialize maximum profit and maximum weight to cp and cw
    int mp = cp;
    int mw = cw;
    int i; // loop index 

    for (i = k + 1; i < N; i++) {
        mw += w[i];
        if (mw <= M) {
            mp += p[i];
        }
        else {
            return mp + (1 - (mw - M) / w[i]) * p[i];
        }
    }
    return mp;
}

// HeapSort: 

void HeapSort(double *p_div_w, int n)  
{
    int i;      // loop index 
    double t1;      // temp variable
    int t2; 
    char *t3;

    // initialize A[1:n] to min heap
    for (i = (n - 1) / 2; i >= 0; i--) {
        // start from the deepest right parents, the index is n / 2
        // i is the root of subtree that needs to heapify       
        Heapify(p_div_w, i, n);
    }
    // extract min at A[0] and swap the end to A[0]
    for (i = n - 1; i > 0; i--) {
        t1 = p_div_w[i]; p_div_w[i] = p_div_w[0]; p_div_w[0] = t1;
        t2 = w[i]; w[i] = w[0]; w[0] = t2;
        t2 = p[i]; p[i] = p[0]; p[0] = t2;
        t3 = name[i]; name[i] = name[0]; name[0] = t3;
        Heapify(p_div_w, 0, i);  // maximun is already in A[n]
    }
}

// Heapify: i is the element that needs to be rearrange 
// to suit the maxheap property
// NOTE: in C implementation, array starts from 0 to n-1
// so the child index is 2 * i + 1 and i is 0 needs to be handle
void Heapify(double *p_div_w, int i, int n)
{
    int j;    // A[j] is the lchild
    double t1;  // temp buffer for p_div_w[i]
    int t2;     // temp buffer for w[i]
    int t3;     // temp buffer for p[i]
    char *t4;   // temp buffer for name[i]
    bool done;    // boolean expression to determine whether heapify is done

    // temporary save element i since A[i] would change in heapify process
    t1 = p_div_w[i];
    t2 = w[i];
    t3 = p[i];
    t4 = name[i];
    // when i is 0, its left child j is 1
    if (i == 0) {
        j = 1;
    }
    else {
        j = 2 * i + 1;  // lchild
    }
    done = false;
    // to min-heap
    while (j < n && done != true) {
        // check which path to go down 
        if (j < n - 1 && p_div_w[j] > p_div_w[j + 1]){  
            // if rchild is smaller
            j = j + 1;  // follow rchild to travse down
        }
        if (t1 < p_div_w[j]) {   // A[i] is already the min
            done = true;
        }
        else {  // continue going down
            p_div_w[(j - 1) / 2] = p_div_w[j];
            w[(j - 1) / 2] = w[j];
            p[(j - 1) / 2] = p[j];
            name[(j - 1) / 2] = name[j];
            j = j * 2 + 1;  // update child index
        }
    }
    // temp finds right position
    p_div_w[(j - 1) / 2] = t1;   
    w[(j - 1) / 2] = t2; 
    p[(j - 1) / 2] = t3; 
    name[(j - 1) / 2] = t4; 
}