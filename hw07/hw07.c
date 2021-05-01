// EE3980 HW07 Grouping Friends
// 106012017, 林冠廷
// 2021/05/01

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>

/* global variable declaration */
struct graph *G; // main graph would be stored in G
struct graph *GT; // transpose of graph G
int Vlen, Elen; // the number of vertices and edges
int len = 13; // there are some 4-words Chinese name
char** T;  // Chinese name lookup table
int* visited;   // visited array (not visited: 0/ visited: 1)
int* slist;  // list containing topological sorting result 
int* R;  // root of forest set
int head_idx;  // record the head index from the end of list to the beginning

/* Structure declaration */
struct node
{
    int vertex;                             // Chinese name idx
    struct node* next;                      // point to the next node
};

struct graph
{
    int numVertices;                        // total number of vertices
    struct node** adjList;                  // 2D node pointer for vertices
};

/* function declaration */
double GetTime(void);   // get current time from linux system 
struct node* createNode(int v);     // create node dynamically                
struct graph* createGraph(int numV);        // create graph dynamically                
// add directed edge <v1,v2> to adjacency list in graph G
void addEdge(struct graph* G, int v1, int v2);
void readGraph(void);       // read the input data convert to graph
// read Chinese name for each individual to Chinese name lookup table T
void ref_table(void);       
int str2int(char* str); // convert Chinese name to integer (person ID)
char* int2str(int idx); // convert integer (person ID) to Chinese name
int SCC(struct graph* G, struct graph* GT); // strongly connected component
void topsort_call(struct graph* G);  // topsort for forest condition
void topsort(struct graph* G, int v);  // topsort starting from vertex v
int DFS_call(struct graph* G);  // DFS call for forest condition 
void DFS(int v, int group_idx);  // DFS travse and record subgroup index to R

/* main function */
int main(void)
{
    double t;   // for CPU time measurement
    int numGroup;   // number of subgroups
    int i, j;      // loop index

    readGraph();    // read the data and construct graph 
    t = GetTime();
    // Grouping Friends: Strongly connected component
    numGroup = SCC(G, GT);
    t = GetTime() - t;  // running time for one run SCC
    
    // print out the information 
    printf("N = %d M = %d Subgroup = %d CPU time = %e\n", 
                Vlen, Elen, numGroup, t);
    printf("Number of subgroups: %d\n", numGroup);
    for (i = 1; i <= numGroup; i++) {
        printf("  Subgroup %d: ", i);
        for (j = 0; j < Vlen; j++) {
            if (i == R[j]) { // for members with the same group number
                printf("%s ", int2str(j));
            }
        }
        printf("\n");
    }

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

// createNode: allocate memory for new node, setting up the vertex key 
// and initializing the next pointer to NULL
struct node* createNode(int v)
{
    // allocate memory in node structure
    struct node* newNode = malloc(sizeof(struct node));

    newNode->vertex = v;    // store key value in "vertex" member
    newNode->next = NULL;   // initialize to NULL
    return newNode;
}

// createGraph: allocate memory for adjacency list and create graph 
struct graph* createGraph(int numV)
{
    int i;      // loop index
    struct graph* graph = malloc(sizeof(struct graph));

    graph->numVertices = numV;
    // allocate memory for the head pointer of adjacency list
    graph->adjList = malloc((numV) * sizeof(struct node));

    // initialize adajacency list pointer to NULL
    for (i = 0; i < numV; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

// // addEdge: add edge <v1, v2> to adjacency list in graph
void addEdge(struct graph* G, int v1, int v2)
{
    // v1 links to v2
    struct node* newNode = createNode(v2); // allocate memory for new node v2

    newNode->next = G->adjList[v1]; // new node point to adjList
    G->adjList[v1] = newNode; // update adjList
}

// readGraph: In the readGraph function, first I call ref_table() to 
// build reference table T[1:n], and then I create both regular graph G 
// and transpose graph GT by createGraph() and addedge(). 
void readGraph(void)
{
    int i; 
    char V1[len], V2[len];  // nodes on edge (v1, v2)
    int v1, v2;

    // read the first line in input file
    scanf("%d%d", &Vlen, &Elen);
    // allocate memory for table T
    T = (char**)malloc(Vlen * sizeof(char*));
    for (i = 0; i < Vlen; i++) {
        T[i] = (char*)malloc(len * sizeof(char));
    }
    // read Chinese name for each individual
    ref_table();
    // create both G and GT
    G = createGraph(Vlen);
    GT = createGraph(Vlen);

    for (i = 0; i < Elen; i++) {  // repeat |E| times
        scanf("%s -> %s", V1, V2); // read v1 and v2
        // convert string to int 
        v1 = str2int(V1);
        v2 = str2int(V2);
        addEdge(G, v1, v2); // build edges on G
        addEdge(GT, v2, v1); // build inverse edges on GT
    }
}

// ref_table:  
// read Chinese name for each individual to Chinese name lookup table T
void ref_table(void)
{
    int i;      // loop index

    for (i = 0; i < Vlen; i++) {
        scanf("%s", T[i]);
    }
}

// str2int: find corresponding integer ID for given Chinese name by T
int str2int(char* str)
{
    int i;  // loop index

    for (i = 0; i < Vlen; i++ ) {
        if (strcmp(str, T[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// int2str: find Chinese name by idx by T
char* int2str(int idx)
{
    return T[idx];
}

// SCC: strongly connected component algorithm to find friend subgroups
// Input: G graph for communication list, GT graph G with transpose edge, 
// Vlen number of vertex = n, visited[1:n] n vertices visited or not,
// slist[1:n] topologically sorted list.
// Output: numGroup total number of subgroups, R[1:n] root of disjoint forests.
int SCC(struct graph* G, struct graph* GT)
{
    int numGroup;   

    head_idx = Vlen - 1;
    // allocate memory for global array "visited", "slist" and "R"
    visited = (int*)malloc(Vlen * sizeof(int));
    slist = (int*)malloc(Vlen * sizeof(int));
    R = (int*)malloc(Vlen * sizeof(int));

    topsort_call(G); // topological sorted list of vertex in G
    numGroup = DFS_call(GT); // find SCC in GT 

    return numGroup;
}

// topsort_call:
// Input: G graph for communication list, Vlen number of vertex = n, 
// visited[1:n] n vertices visited or not 
// Output: slist[1:n] topologically sorted list. 
void topsort_call(struct graph* G)
{
    int i;  // loop index

    // initialize "visisted" to 0 (not visited)
    for (i = 0; i < Vlen; i++) {
        visited[i] = 0;
    }
    for (i = 0; i < Vlen; i++) {
        if (visited[i] == 0) {
            topsort(G, i);
        }
    }
}

// topsort: 
// Input: G graph for communication list,  v starting vertex for topology sort,
// head_idx records head index when topology sort, visited[1:n] 
// Output: slist[1:n] topologically sorted list
void topsort(struct graph* G, int v)
{
    struct node* temp;

    if (head_idx >= 0) {
        visited[v] = 1;
        temp = G->adjList[v];

        while (temp != NULL) {
            if (visited[temp->vertex] == 0) { // neighbor vertex not visited 
                topsort(G, temp->vertex);
            }
            temp = temp->next; // update temp to next adjacent vertex
        }
        slist[head_idx] = v;  // add current vertex v to head of slist
        head_idx--;  // update head idx
    }
}

// DFS_call: 
// Input: GT graph for communication list,  visited[1:n],
// slist[1:n] topologically sorted list, Vlen number of vertices.
// Output: group_idx – 1=numGroup number of subgroups, 
// R[1:n] root of disjoint forests
int DFS_call(struct graph* GT)
{
    int i;  // loop index
    int group_idx = 1;

    // initialize visited and R to 0
    for (i = 0; i < Vlen; i++) {
        visited[i] = 0;
        R[i] = 0;
    }
    for (i = 0; i < Vlen; i++) {
        // select starting vertex according to the order of slist
        if (visited[slist[i]] == 0) {
            DFS(slist[i], group_idx);
            group_idx++;    // update group_idx, next subgroup
        }
    }
    return group_idx - 1; 
}

// DFS:
// Input: GT graph for communication list,  visited[1:n],
// Vlen number of vertices, group_idx .
// Output: R[1:n] root of disjoint forests
void DFS(int v, int group_idx)
{
    struct node* temp = GT->adjList[v];
    
    visited[v] = 1;  // update to visited
    while (temp != NULL) {
        if (visited[temp->vertex] == 0) { // neighbor vertex not visited 
            DFS(temp->vertex, group_idx);
        }
        temp = temp->next; // update temp to next adjacent vertex
    }
    R[v] = group_idx; // record subgroup idx for vertex v
}