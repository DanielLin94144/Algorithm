// EE3980 HW12 Travelling Salesperson Problem
// 106012017, 林冠廷
// 2021/06/12

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* Structure declaration */
struct node 
{
    int cost, lb;
    int level;
    int v; // vertex number
    int **d;
    struct node *parent;
    struct node *next;
    struct node *prev;
};
struct list
{
    struct node *head;
    struct node *tail;
    int num;
};
/* function declaration */
double GetTime(void);   // get current time from linux system 
int Reduce(int **d); 
struct node* newNode(int **ori_d, struct node *E, int R, int level, int i, int j);
void pad_impossible(int **d, int i, int j); 
struct node* LCsearch(int **d);
/* global variable declaration */
int N;  // number of cities
int **d;  // 2D distance array
char **name;    // name of cities
int string_len = 17;    // max length of string: 16 + 1 (null char)
int inf = 100000;
/* main function */
int main(void)
{
    double t;   // CPU running time 
    int D;  // travelling distance
    int i, j;  // loop index
    int k, l;
    int temp;
    int R;
    int r;
    struct node *A;
    int **sol;
    int total_cost;

    // read the first line to get N
    scanf("%d", &N);
    // memory allocation 
    name = (char**)malloc(N * sizeof(char*));
    d = (int**)malloc(N * sizeof(int*));
    sol = (int**)malloc(N * sizeof(int*));
    for (i = 0; i < N; i++) {
        name[i] = (char*)malloc(string_len * sizeof(char));
        d[i] = (int*)malloc(N * sizeof(int));
        sol[i] = (int*)malloc(2 * sizeof(int));
    }
    
    // read name of cities
    for (i = 0; i < N; i++) {
        getchar();  // remove "\n"
        scanf("%[^\n]", name[i]);   
    }
    // read distance matrix    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            scanf("%d", &temp);
            if (temp == 0) {
                d[i][j] = -1;
            }
            else {
                d[i][j] = temp;
            }
        }
    }
    // run LSBB
    t = GetTime();
    A = LCsearch(d);
    t = GetTime() - t;
    // 
    total_cost = A->cost;
    sol[N - 1][0] = A->v;
    sol[N - 1][1] = 0;  // back to 0
    i = N - 2;
    while (A->parent) {
        // sol: from sol[i][0] to sol[i][1]
        sol[i][0] = A->parent->v;
        sol[i][1] = A->v;
        A = A->parent;
        i--;
    }
    printf("Minimum distance route:\n");
    for (i = 0; i < N; i++) {
        printf("  %s -> %s\n", name[sol[i][0]], name[sol[i][1]]);
    }
    printf("Total travelling distacne: %d\n", total_cost);
    printf("CPU time: %g s\n", t);

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

int Reduce(int **d) 
{
    int i, j; // loop index
    int R = 0; // total reduction 
    int min; // min number but > 0

    // row reduction 
    for (i = 0; i < N; i++) { 
        min = inf;
        // find min in row
        for (j = 0; j < N; j++) {
            if (d[i][j] >= 0) { // can link
                if (d[i][j] < min) {
                    min = d[i][j];
                }
            } 
        }
        // finish finding min 
        for (j = 0; j < N; j++) {
            if (d[i][j] > 0) {
                d[i][j] -= min;
            }
        }
        if (min == inf) {
            min = 0;
        }
        R += min;
    }
    // col reduction
    for (j = 0; j < N; j++) { 
        min = inf;
        // find min in row
        for (i = 0; i < N; i++) {
            if (d[i][j] >= 0) { // can link
                if (d[i][j] < min) {
                    min = d[i][j];
                }
            } 
        }
        // finish finding min 
        for (i = 0; i < N; i++) {
            if (d[i][j] > 0) { // can link
                d[i][j] -= min;
            }
        }
        if (min == inf) {
            min = 0;
        }
        R += min;
    }
    return R;
}

// from node i to node j
// R is previous lb
struct node* newNode(int **d, struct node *E, int R, int level, int i, int j)
{
    int k, l; // loop index
    struct node* newNode = malloc(sizeof(struct node));
    
    // initialize 
    newNode->cost = R;
    newNode->parent = E;
    newNode->level = level;
    newNode->v = j;
    newNode->d = (int**)malloc(N * sizeof(int*));
    newNode->next = NULL;
    newNode->prev = NULL;

    for (k = 0; k < N; k++) {
        newNode->d[k] = (int*)malloc(N * sizeof(int));
    }
    // initialization: copy previous step d
    for (k = 0; k < N; k++) {
        for (l = 0; l < N; l++) {
            newNode->d[k][l] = d[k][l];
        }
    }
    
    return newNode;
}

void pad_impossible(int **d, int i, int j)
{
    int k; // loop index

    // pad row
    for (k = 0; k < N; k++) {
        d[i][k] = -1;
    }
    // pad col
    for (k = 0; k < N; k++) {
        d[k][j] = -1;
    }
    d[j][i] = -1;
}

struct node* LCsearch(int **d)
{
    struct node *root, *child, *E, *temp, *min_node;
    int level;
    int i, j;   // edge from node i to node j
    struct list *L = (struct list*)malloc(sizeof(struct list));
    int min;
    int R;
    int r;

    // root node 
    R = Reduce(d);
    root = newNode(d, NULL, R, 0, -1, 0);
    L->head = NULL;
    L->tail = NULL;
    E = root;

    while(E) {
        i = E->v;
        // find answer
        if (E->level == N - 1) {
            return E;
        }
        // select new edge <i, j>
        for (j = 1; j < N; j++) {
            if (E->d[i][j] >= 0) {
                child = newNode(E->d, E, E->cost, E->level + 1, i, j);
                pad_impossible(child->d, i, j);
                r = Reduce(child->d);            
                child->cost = E->cost + E->d[i][j] + r;
                // initialize L
                if (L->head == NULL && L->tail == NULL) {
                    L->head = child;
                    L->tail = child;
                }
                // link head to tail
                // add one
                else { 
                    L->tail->next = child;
                    child->prev = L->tail;
                    L->tail = child; 
                }
            }
        }
        // Least
        temp = L->head;
        min_node = NULL;
        min = inf;
        while (temp) {
            if (temp->cost < min) {
                min = temp->cost;
                min_node = temp;
                
            }
            temp = temp->next;
        }
        
        // remove current min_node
        if (min_node->next == NULL) { // min_node is the last 
            min_node->prev->next = NULL;
            L->tail = min_node->prev;
        }
        else {
            min_node->prev->next = min_node->next;  
            min_node->next->prev = min_node->prev;
        }
        // update new E
        E = min_node;
    }
}