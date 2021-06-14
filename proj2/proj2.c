// EE3980 Project2 Encoding Text Files
// 106012017, 林冠廷
// 2021/06/14

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Structure declaration */
struct node
{
    char Ch_buff[4];
    char En_buff[2];
    int indicator; // English char >= 0; Chinese char < 0
    int count;
    struct node *prev, *next, *lchild, *rchild;
};

struct list
{
    struct node *head;
    struct node *tail;
};
/* function declaration */
struct node** readInput(void);
struct node* NewNode(int indicator, char new_char[]);
void Heapify(struct node **list, int i, int n);
struct node* HuffmanCoding(struct node **list, int n);
struct node* Least(struct node **list, int n);
void Insert(struct node **list, struct node *pt, int n);
void DFS_print(struct node *A, int* path, int code, int n);
/* global variable declaration */
double total_bits = 0.0;
int N_char;
int Numchar;
/* main function */
int main(void)
{
    int i, j;
    int bytes;
    int *path;
    struct node *A;
    struct node **list;
    double efficiency;

    list = readInput();
    for (i = (N_char - 1) / 2; i >= 0; i--) {
        // start from the deepest right parents, the index is n / 2
        // i is the root of subtree that needs to heapify       
        Heapify(list, i, N_char);
    }
    A = HuffmanCoding(list, N_char);
    path = (int*)malloc(N_char * sizeof(int));
    printf("The best coding:\n");
    DFS_print(A, path, 0, 0);
    bytes = total_bits / 8;
    // ceiling bytes
    if (8 * bytes < total_bits) {
        bytes++;
    }
    printf("Number of characters read: %d\n", Numchar);
    printf("The best coding needs %d bits, %d bytes\n", 
                            (int) total_bits, bytes);
    printf("Efficiency = %g %s\n", ((double) bytes) / Numchar * 100, "%");
    return 0;
}

/* function implementation */
// Getime: current time from linux
struct node** readInput(void)
{
    // max_len is 121 because the maximum Chineses sentence is about 
    // 37 character and add one byte for null character.
    int max_len = 40 * 3 + 1;
    char c[max_len];
    char Ch_buff[4];
    char En_buff[2];
    int i, j;
    int indicator;
    struct list *L = (struct list*)malloc(sizeof(struct list));
    struct node *new, *temp;
    struct node **list;
    bool is_new;
    bool done;

    // initialization for linked list L
    L->head = NULL;   
    L->tail = NULL;
    En_buff[1] = (char) 0;
    Ch_buff[3] = (char) 0;

    if (L->head == NULL && L->tail == NULL) {
        new = NewNode(10, "\n");
        L->head = new;
        L->tail = new;
    }
    while (fgets(c, max_len, stdin) != NULL) {
        i = 0;
        done = false;
        while (!done) {
            indicator = (int) c[i];
            if (indicator == 10) {
                L->head->count++;
                done = true;
            }
            else if (indicator >= 0) {
                En_buff[0] = c[i]; // English char
                // L already exist some nodes
                is_new = true;
                temp = L->head;  // start from the head of L
                while (temp) {
                    if (strcmp(temp->En_buff, En_buff) == 0) {
                        is_new = false;
                        temp->count += 1;
                    }
                    temp = temp->next;  // update temp to next node
                }
                if (is_new) {
                    new = NewNode(indicator, En_buff);
                    L->tail->next = new;
                    new->prev = L->tail;
                    L->tail = new; 
                }
                i++;
            }
            else {
                for (j = 0; j < 3; j++) {
                    Ch_buff[j] = c[i + j];
                }
                // L already exist some nodes
                is_new = true;
                temp = L->head;  // start from the head of L
                while (temp) {
                    if (strcmp(temp->Ch_buff, Ch_buff) == 0) {
                        is_new = false;
                        temp->count += 1;
                    }
                    temp = temp->next;  // update temp to next node
                }
                if (is_new) {
                    new = NewNode(indicator, Ch_buff);
                    L->tail->next = new;
                    new->prev = L->tail;
                    L->tail = new; 
                }
                i += 3;
            }
        }
    }
    // count the N_char |c| and Numchar by character frequency
    temp = L->head;  // start from the head of L
    N_char = 0;
    while (temp) {
        N_char++;
        Numchar += temp->count;
        temp = temp->next;  // update temp to next node
    }
    list = (struct node**)malloc(N_char * sizeof(struct node*));
    i = 0;
    // copy from linked list to array list
    temp = L->head;  // start from the head of L
    while (temp) {
        list[i] = temp;
        temp = temp->next;  // update temp to next node
        i++;
    }
    free(L);  // free L since we only need list later

    return list;
}

// NewNode: if no existing node contain this char, create new node for it
struct node* NewNode(int indicator, char new_char[])
{
    struct node* newNode = malloc(sizeof(struct node));

    if (indicator == 10) {
        strcpy(newNode->En_buff, new_char);
        newNode->count = 0; // initialize count to 0 for '\n'
        newNode->indicator = indicator; 
    }
    else if (indicator >= 0) {
        newNode->En_buff[1] = (char) 0;
        strcpy(newNode->En_buff, new_char);
        newNode->count = 1; // initialize count to 1 for new char
        newNode->indicator = indicator; 
    }
    else {
        newNode->Ch_buff[3] = (char) 0;
        strcpy(newNode->Ch_buff, new_char);
        newNode->count = 1; // initialize count to 1 for new char
        newNode->indicator = indicator; 
    }
    
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->lchild = NULL;
    newNode->rchild = NULL;
    
    return newNode;
}

// Heapify
void Heapify(struct node **list, int i, int n)
{
    int j;    // A[j] is the lchild
    struct node *temp;     // temp buffer to save A[i]
    bool done;    // boolean expression to determine whether heapify is done

    // temporary save element i since A[i] would change in heapify process
    temp = list[i];
    // when i is 0, its left child j is 1
    if (i == 0) {
        j = 1;
    }
    else {
        j = 2 * i + 1;  // lchild
    }
    done = false;
    while (j < n && done != true) {
        // check which path to go down 
        if (j + 1 < n) {
            if (list[j]->count > list[j + 1]->count) {  
                // if rchild is smaller
                j = j + 1;  // follow rchild to travse down
            }
        }
        if (temp->count < list[j]->count) {   // A[i] is already the max
            done = true;
        }
        else {  // continue going down
            list[(j - 1) / 2] = list[j];
            j = j * 2 + 1;
        }
    }
    list[(j - 1) / 2] = temp;   // temp finds right position
}

// HuffmanCoding
struct node* HuffmanCoding(struct node **list, int n)
{
    int i; // loop index
    int N = n;
    struct node *pt; 
    int j;

    for (i = 1; i <= N - 1; i++) {
        // create new combined node pt
        pt = (struct node*)malloc(sizeof(struct node));
        n--;
        pt->lchild = Least(list, n);
        n--; 
        pt->rchild = Least(list, n);
        // combine count in lchild and rchild
        pt->count = pt->lchild->count + pt->rchild->count;
        Insert(list, pt, n);
        n++;
    }
    return list[0];
}

struct node* Least(struct node **list, int n)
{
    struct node *t, *smallest; 

    smallest = list[0];
    // swap minimum and the last 
    t = list[n]; list[n] = list[0]; list[0] = t;
    Heapify(list, 0, n);
    return smallest;
}

void Insert(struct node **list, struct node *pt, int n)
{
    bool done = false;
    int i;
    i = n;

    // check upward with parent node
    while (!done) { 
        if (list[(i - 1) / 2]->count <= pt->count) {
            // already meet minheap property
            done = true;
        }
        else { // keep move up
            list[i] = list[(i - 1) / 2];
            i = (i - 1) / 2;
        }
        list[i] = pt;
    }
}

void DFS_print(struct node *A, int* path, int code, int n)
{
    int i; // loop index

    if (n > 0) {
        path[n - 1] = code;
        if (A->lchild == NULL && A->rchild == NULL) {
            total_bits += (double) n * (double) A->count;
            printf("  ");
            // Special case since we cannot print '\n' directly
            if (A->indicator == 10) {
                printf("%s ", "\'\\n\'");
                for (i = 0; i < n; i++) {
                    printf("%d", path[i]);
                }
                printf("\n");
            }
            // Special case since we cannot print space char '\t' directly
            else if (A->indicator == 32) {
                printf("%s ", "\' \'");
                for (i = 0; i < n; i++) {
                    printf("%d", path[i]);
                }
                printf("\n");
            }
            // print English character
            else if (A->indicator >= 0) {
                printf("%s ", A->En_buff);
                for (i = 0; i < n; i++) {
                    printf("%d", path[i]);
                }
                printf("\n");
            }
            // print remaining utf-8 character (Chinese)
            else {
                printf("%s ", A->Ch_buff);
                for (i = 0; i < n; i++) {
                    printf("%d", path[i]);
                }
                printf("\n");
            }    
        }    
    }
    if (A->lchild) {
        // continue visiting left child and input direction = 0
        DFS_print(A->lchild, path, 0, n + 1);
    }
    if (A->rchild) {
        // continue visiting right child and input direction = 1 
        DFS_print(A->rchild, path, 1, n + 1);
    }
}

