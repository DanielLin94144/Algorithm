// EE3980 HW08 Minimum-Cost Spanning Tree
// 106012017, 林冠廷
// 2021/05/08

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* Structure declaration */
struct Edge
{
    int v1, v2;
    double weight;
};

/* global variable declaration */
double** w;
double* W;
int* near;
int* p;
int** T;
int Vlen, Elen; // the number of vertices and edges
int repeatation = 500; 
double inf = 1000000;
struct Edge* E;
bool run_Prim = false;

/* function declaration */
double GetTime(void);   // get current time from linux system 
void readGraph(void);       // read the input data convert to graph
double Prim(void); 
double Kruskal(void);
int Find(int i);
void Union(int i, int j);
void Heapify(struct Edge* E, int i, int n);

/* main function */
int main(void)
{
    double t; 
    int i; 
    double cost;
    int v1, v2;

    readGraph();
    t = GetTime();
    for (i = 0; i < repeatation; i++) {
        cost = Kruskal();   // can be replaced with calling Prim()
    }
    t = (GetTime() - t) / repeatation;

    printf("Minimum-cost spanning tree:\n");
    for (i = 1; i <= Vlen - 1; i++) {
        v1 = T[i][0]; 
        v2 = T[i][1];        
        // print the edge's vertex in increasing order 
        if (T[i][0] > T[i][1]) { 
            v1 = T[i][1]; 
            v2 = T[i][0];
        }
        printf("  %d: <%d %d> %.2lf\n", i, v1, v2, W[i]);
    }
    printf("|V| = %d |E| = %d\n", Vlen, Elen);
    printf("Minimum cost: %.2lf\n", cost);
    printf("CPU time: %e seconds\n", t);

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

// readGraph: 
void readGraph(void)
{
    int i, j; 
    int v1, v2;
    double weight;

    // // read the first line in input file
    scanf("%d%d", &Vlen, &Elen); 
    // allocate memory
    T = (int**)malloc((Vlen + 1) * sizeof(int*));
    for (i = 0; i <= Vlen; i++) {
        T[i] = (int*)malloc((2) * sizeof(int));  // 1 2
    }
    near = (int*)malloc((Vlen + 1) * sizeof(int));
    p = (int*)malloc((Vlen + 1) * sizeof(int));
    // Heap index start from 1~Elen
    E = (struct Edge*)malloc((Elen + 1) * sizeof(struct Edge)); 
    W = (double*)malloc((Vlen) * sizeof(double)); 

    if (run_Prim) {
        // allocate memory for w
        w = (double**)malloc((Vlen + 1) * sizeof(double*));
        for (i = 0; i <= Vlen; i++) {
            w[i] = (double*)malloc((Vlen + 1) * sizeof(double));
        }
        // initialize w to inf
        for (i = 1; i <= Vlen; i++) {
            for (j = 1; j <= Vlen; j++) {
                w[i][j] = inf;
            }
        }
    }
    for (i = 1; i <= Elen; i++) {  // repeat |E| times
        scanf("%d %d %lf", &v1, &v2, &weight); // read v1 and v2
        E[i].v1 = v1;
        E[i].v2 = v2;
        E[i].weight = weight;
        if (run_Prim) {
            w[v1][v2] = weight;
            w[v2][v1] = weight;
        }
    }
}

double Prim(void)
{
    int i, j;  // loop index
    int k, l;  // (k, l)
    int local_idx;
    double mincost = inf;
    double local_min;
    
    // find minimum edge
    for (i = 1; i <= Vlen; i++) {
        for (j = 1; j <= Vlen; j++) {
            if (w[i][j] < mincost) {
                k = i;
                l = j;
                mincost = w[i][j];
            }
        }
    }
    // record first edge to minimum spanning tree T
    T[1][0] = k;
    T[1][1] = l;
    for (i = 1; i <= Vlen; i++) {
        if (w[i][l] < w[i][k]) {
            near[i] = l;
        }
        else {
            near[i] = k;
        }
    }
    near[k] = 0;
    near[l] = 0;
    for (i = 2; i <= Vlen - 1; i++) {   // add remaining n-2 edges to tree
        local_min = inf;
        for (j = 1; j <= Vlen; j++) {   // find j 
            if (near[j] != 0 && w[j][near[j]] < local_min) {
                local_min = w[j][near[j]];
                local_idx = j;
            }
        }
        j = local_idx;
        T[i][0] = j;
        T[i][1] = near[j];
        mincost = mincost + w[j][near[j]];    // update mincost
        near[j] = 0;
        for (k = 1; k <= Vlen; k++) {  // update near for unselected vertices
            if (near[k] != 0 && w[k][j] < w[near[k]][k]) {
                near[k] = j;
            }
        }
    }
    return mincost;
}

double Kruskal(void)
{
    int i; 
    double mincost = 0;
    int v1, v2, s1, s2; 
    double min_weight;
    int n = Elen; // edge heap capacity 
    struct Edge temp;
    
    for (i = Elen / 2; i >= 1; i--) {
        Heapify(E, i, Elen);
    }
    for (i = 1; i <= Vlen; i++) { // initialize p[1:n] to -1 (disjoint set)
        p[i] = -1;
    }
    i = 1;
    while (i <= Vlen - 1 && n >= 1) { // heap is not empty
        min_weight = E[1].weight; 
        v1 = E[1].v1;  v2 = E[1].v2;
        // swapping first and last 
        temp = E[1];    E[1] = E[n];    E[n] = temp;
        n--;   
        Heapify(E, 1, n);
        s1 = Find(v1);
        s2 = Find(v2);
        if (s1 != s2) {     // if two set is not the same (no cycle)
            T[i][0] = v1;   T[i][1] = v2; 
            W[i] = min_weight;
            mincost = mincost + min_weight;
            Union(s1, s2);  // union set disjoint set
            i++;
        }
        
    }
    if (i != Vlen) {
        printf("No spanning tree\n");
    }
    return mincost;
}

// Heapify: i is the element that needs to be rearrange 
// n = total number of elements
void Heapify(struct Edge* E, int i, int n)
{
    struct Edge temp = E[i];
    int j = 2 * i;  // lchild
    bool done = false; 
    
    while (j <= n && done != true) {
        if (j + 1 <= n) {
            if (E[j].weight > E[j + 1].weight) {  // choose the smaller child
                j = j + 1;
            }
        }
        if (E[j].weight < temp.weight) {
            E[j / 2] = E[j]; 
            j = 2 * j; 
        }
        else {
            done = true;
        }
    }
    E[j / 2] = temp;
}



// Given an element, Find the root of the set that element i is in.
int Find(int i)
{
    while (p[i] > 0) { // if p[i] = -1, then i is root, terminate while loop
        i = p[i];
    }
    return i;
}

// Union: weighted union
void Union(int i, int j)
{    
    int temp = p[i] + p[j];

    if (p[i] > p[j]) {  // i has fewer elements, set j as new parent 
        p[i] = j;
        p[j] = temp;
    }
    else {
        p[j] = i;
        p[i] = temp;
    }
}
