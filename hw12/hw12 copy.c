// EE3980 HW12 Travelling Salesperson Problem
// 106012017, 林冠廷
// 2021/06/12

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

/* Structure declaration */
struct node 
{
    int cost; // current path cost
    int level; // current level in state tree
    int v; // city number
    int **d; // current reduced table
    struct node *next; // link the linked list that extracts least-cost node
    struct node *prev;
    int **path; // current traversing path
};
struct list
{
    struct node *head;
    struct node *tail;
};
/* function declaration */
double GetTime(void);   // get current time from linux system 
int Reduce(int **d); 
struct node* newNode(int **d, int **path, int R, int level, int i, int j);
void pad_impossible(int **d, int i, int j); 
struct node* LCBB(int **d, int **path);
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
    int **path;
    // read the first line to get N
    scanf("%d", &N);
    // memory allocation 
    name = (char**)malloc(N * sizeof(char*));
    d = (int**)malloc(N * sizeof(int*));
    path = (int**)malloc(N * sizeof(int*));

    for (i = 0; i < N; i++) {
        name[i] = (char*)malloc(string_len * sizeof(char));
        d[i] = (int*)malloc(N * sizeof(int));
        path[i] = (int*)malloc(2 * sizeof(int));
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < 2; j++) {
            path[i][j] = 0;
        }
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
    // run LCBB
    t = GetTime();
    A = LCBB(d, path);
    t = GetTime() - t;

    total_cost = A->cost;
    A->path[N - 1][0] = A->v;
    A->path[N - 1][1] = 0;  // back to 0
    printf("Minimum distance route:\n");
    for (i = 0; i < N; i++) {
        printf("  %s -> %s\n", name[A->path[i][0]], name[A->path[i][1]]);
    }
    printf("Total travelling distacne: %d\n", total_cost);

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
struct node* newNode(int **d, int **path, int R, int level, int i, int j)
{
    int k, l; // loop index
    struct node* newNode = malloc(sizeof(struct node));
    int r;

    // initialize 
    
    newNode->level = level;
    newNode->v = j;
    newNode->d = (int**)malloc(N * sizeof(int*));
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->path = (int**)malloc(N * sizeof(int*));

    for (k = 0; k < N; k++) {
        newNode->d[k] = (int*)malloc(N * sizeof(int));
    }
    
    for (k = 0; k < N; k++) {
        newNode->path[k] = (int*)malloc(2 * sizeof(int));
    }
    // copy path for previous node 
    for (k = 0; k < level - 1; k++) {
        for (l = 0; l < 2; l++) {
            newNode->path[k][l] = path[k][l];
        }
    }
    // initialization: copy previous step d
    for (k = 0; k < N; k++) {
        for (l = 0; l < N; l++) {
            newNode->d[k][l] = d[k][l];
        }
    }
    // record new path <i, j>
    if (level > 0) {
        newNode->path[level - 1][0] = i;
        newNode->path[level - 1][1] = j;
        // calculate current lower bound from:  
        // previous cost R and new reduction r
        pad_impossible(newNode->d, i, j);
        r = Reduce(newNode->d);            
        newNode->cost = R + d[i][j] + r;
    }
    else { // if is root node
        newNode->cost = R;
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
    d[j][i] = -1;  // reverse link is not available 
    d[j][0] = -1;  // current node j cannot directly back to starting city
}

struct node* LCBB(int **d, int **path)
{
    struct node *root, *child, *E, *temp, *min_node;
    int level;
    int i, j;   // edge from node i to node j
    struct list *L = (struct list*)malloc(sizeof(struct list));
    int min;
    int R;
    int lb = inf; 
    // initialization: create root node 
    R = Reduce(d);
    root = newNode(d, path, R, 0, -1, 0);
    // initialization for linked list L
    L->head = NULL;   
    L->tail = NULL;
    // set E as the root node to enter while loop 
    E = root;
    while(E) {
        i = E->v;
        // if find answer, the entire traversing path
        if (E->level == N - 1) {
            return E;
        }
        // select new edge <i, j>
        for (j = 1; j < N; j++) {  
            // for every possible node other than the first city
            if (E->d[i][j] >= 0) {
                child = newNode(E->d, E->path, E->cost, E->level + 1, i, j);
                // insert child node to L
                // (1) if L is emtpy
                if (L->head == NULL && L->tail == NULL) {
                    L->head = child;
                    L->tail = child;
                }
                // (2) if L already exist some nodes
                else { 
                    L->tail->next = child;
                    child->prev = L->tail;
                    L->tail = child; 
                }
            }
        }
        // free node E since it already has child, 
        // the information in E is unnecessary
        free(E);
        // Find least-cost node in L
        temp = L->head;  // start from the head of L
        min_node = NULL;
        min = inf;
        while (temp) {
            if (temp->cost < min) {
                min = temp->cost;
                min_node = temp;                
            }
            temp = temp->next;  // update temp to next node
        }
        // Linked list deletion: remove current min_node
        if (min_node->next == NULL) { // min_node is the last 
            min_node->prev->next = NULL;
            L->tail = min_node->prev;
        }
        else {
            min_node->prev->next = min_node->next;  
            min_node->next->prev = min_node->prev;
        }
        // update new E if L is not empty
        E = min_node;
    }
}
