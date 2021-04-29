// EE3980 HW03 Network Connectivity Problem
// 106012017 林冠廷
// 2021/03/27

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
/* Structure declaration */
struct node
{
    int vertex;                             // vertex number
    struct node* next;                      // point to the next node
};

struct graph
{
    int numVertices;                        // total number of vertices
    struct node** adjList;                  // 2D node pointer for vertices
};


/* global variable */

int *R; // root table 
int *p; // parent table
struct graph *G; // main graph would be stored in G
int Vlen, Elen; // the number of vertices and edges
int NS;  // the number of disjoint sets 
int Nrepeat = 100;  // number of repeatation to measure CPU time
/* function declaration */
struct node* createNode(int v);     // create node dynamically                
struct graph* createGraph(int numV);        // create graph dynamically                
// add edge (v1,v2) to adjacency list
void addEdge(struct graph* G, int v1, int v2);
void readGraph(void);       // read the input data convert to graph
int SetFind(int i);         // find the root of set for given node
void SetUnion(int i, int j);        // merge two sets 
void Connect1(void);        // Connect1 algorithm implementation
void WeightedUnion(int i, int j); // merge two sets depends on number of nodes
void Connect2(void);        // Connect2 algorithm implementation
// find the root of set for given node and flatten the path on i
int CollapsingFind(int i); 
void Connect3(void);        // Connect3 algorithm implementation
double GetTime(void);       // get local time in seconds

/* main function */
int main (void)
{
    double t0, t1, t2, t3;      // four time-recording points
    int i;      // loop index
    int NS1, NS2, NS3;   // three algorithms resulting number of disjoint sets

    scanf("%d%d", &Vlen, &Elen);   // read the first line of input data |V|,|E|
    printf("|V| = %d, |E| = %d\n", Vlen, Elen); // print |V| and |E|
    readGraph(); // read the input data to graph G

    R = (int*)malloc((Vlen + 1) * sizeof(int));  // root table 
    p = (int*)malloc((Vlen + 1) * sizeof(int));  // represent the disjoint set
                                                 // root is -1
    // measure Connect1 CPU time                                                  
    t0 = GetTime();
    for (i = 0; i < Nrepeat; i++) {
        Connect1();
    }
    NS1 = NS;
    t1 = GetTime();
    // measure Connect2 CPU time
    for (i = 0; i < Nrepeat; i++) {
        Connect2();
    }
    NS2 = NS;
    t2 = GetTime();
    // measure Connect3 CPU time
    for (i = 0; i < Nrepeat; i++) {
        Connect3();
    }
    NS3 = NS;
    t3 = GetTime();
    // print the results for three algorithms
    printf("Connect1 CPU time: %e, Disjoint sets: %d\n", \
            (t1 - t0) / Nrepeat, NS1);
    printf("Connect2 CPU time: %e, Disjoint sets: %d\n", \
            (t2 - t1) / Nrepeat, NS2);
    printf("Connect3 CPU time: %e, Disjoint sets: %d\n", \
            (t3 - t2) / Nrepeat, NS3);
    return 0;
}

/* function implementation */
// createNode function: allocate memory for new node, setting up the vertex key 
// and initializing the next pointer to NULL
struct node* createNode(int v)
{
    // use malloc() to allocate memory in node structure
    struct node* newNode = malloc(sizeof(struct node));
    newNode->vertex = v;    // store key value in "vertex" member
    newNode->next = NULL;   // initialize to NULL
    return newNode;
}
// createGraph function: 
struct graph* createGraph(int numV)
{
    struct graph* graph = malloc(sizeof(struct graph));
    graph->numVertices = numV;
    // allocate memory for the head pointer of adjacency list
    graph->adjList = malloc((numV + 1) * sizeof(struct node));

    // initialize adajacency list pointer to NULL
    int i;      // loop index
    for (i = 0; i < numV + 1; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}
void addEdge(struct graph* G, int v1, int v2)
{
    // undirected graph: v1 links to v2 and v2 links to v1
    struct node* newNode = createNode(v2); // allocate memory for new node v2
    newNode->next = G->adjList[v1]; // new node point to adjList
    G->adjList[v1] = newNode; // update adjList
}

void readGraph(void)
{
    G = createGraph(Vlen);
    int i; // loop index
    int v1, v2;  // nodes on edge (v1, v2)

    for (i = 0; i < Elen; i++) {  // repeat |E| times
        scanf("%d%d", &v1, &v2); // read v1 and v2
        addEdge(G, v1, v2);//using addEdge() function to build edges on adjList
    }
}

double GetTime(void)
{                       // get current time from linux
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

// Given an element, Find the root of the set that element i is in.
int SetFind(int i)
{
    while (p[i] > 0) { // if p[i] = -1, then i is root, terminate while loop
        i = p[i];
    }
    return i;
}

// Merge two sets with root i and j.
void SetUnion(int i, int j)
{
    p[i] = j; // set i as parent and j as child
}

//Connect1 uses simple SetFind and random SetUnion.
void Connect1(void)
{
    int i,v;
    int Si, Sj;
    // one element for each set, all vertices initialize to -1 
    // which means they are their own parent
    for (i = 1; i < Vlen + 1; i++) {
        p[i] = -1;
    }
    NS = Vlen; // NS initialize to |V|
    struct node* temp; // temp node to move on adjList
    // Connected vertices, edges stored in the adjacency list in G
    for (v = 1; v < Vlen + 1; v++) {
        temp = G->adjList[v];
        while (temp) { // run all the edges given certain vertex
            Si = SetFind(v);
            Sj = SetFind(temp->vertex);
            if (Si != Sj) { // two sets are different
                NS--; // update number of disjoint set
                SetUnion(Si, Sj);
            }
            temp = temp->next;
        }
    }
    // record root to R table
    for (v = 1; v < Vlen + 1; v++) {
        R[v] = SetFind(v);
    }
}

void WeightedUnion(int i, int j)
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
// Connect2 uses simple SetFind with WeightedUnion.
void Connect2(void)
{
    int i,v;
    int Si, Sj;
    // one element for each set, all vertices initialize to -1 
    // which means they are their own parent
    for (i = 1; i < Vlen + 1; i++) {
        p[i] = -1;
    }
    NS = Vlen; // NS initialize to |V|
    struct node* temp; // temp node to move on adjList
    // Connected vertices, edges stored in the adjacency list in G
    for (v = 1; v < Vlen + 1; v++) {
        temp = G->adjList[v];
        while (temp) { // run all the edges given certain vertex
            Si = SetFind(v);
            Sj = SetFind(temp->vertex);
            if (Si != Sj) { // two sets are different
                NS--; // update number of disjoint set
                WeightedUnion(Si, Sj);
            }
            temp = temp->next;
        }
    }
    // record root to R table
    for (v = 1; v < Vlen + 1; v++) {
        R[v] = SetFind(v);
    }
}
// not only find the root number, but also flatten the nodes 
// on its path to the root as the new children of root. 
int CollapsingFind(int i)
{
    int r, s;
    r = i;
    while (p[r] > 0) {
        r = p[r];       // trace up to find the parent
    }
    while (i != r) {    // make the tree flatten
        s = p[i];
        p[i] = r;
        i = s;
    }
    return r;
}

// Connect3 uses CollapsingFind and WeightedUnion.
void Connect3(void)
{
    int i,v;
    int Si, Sj;
    // one element for each set, all vertices initialize to -1 
    // which means they are their own parent
    for (i = 1; i < Vlen + 1; i++) {
        p[i] = -1;
    }
    NS = Vlen; // NS initialize to |V|
    struct node* temp; // temp node to move on adjList
    // Connected vertices, edges stored in the adjacency list in G
    for (v = 1; v < Vlen + 1; v++) {
        temp = G->adjList[v];
        while (temp) { // run all the edges given certain vertex
            Si = CollapsingFind(v);
            Sj = CollapsingFind(temp->vertex);
            if (Si != Sj) { // two sets are different
                NS--; // update number of disjoint set
                WeightedUnion(Si, Sj);
            }
            temp = temp->next;
        }
    }
    // record root to R table
    for (v = 1; v < Vlen + 1; v++) {
        R[v] = SetFind(v);
    }
}
