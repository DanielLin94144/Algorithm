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
struct node* NewNode(int indicator, char new_char[]);
/* global variable declaration */

/* main function */
int main(void)
{
    int max_len = 67 * 3 + 1;
    char c[max_len];
    int i, j;
    char Ch_buff[4];
    int indicator;
    char En_buff[2];
    int cur_len;
    struct list *L = (struct list*)malloc(sizeof(struct list));
    struct node *new, *temp;
    bool is_new;
    int Numchar = 0;
    bool done;
    int N_char;
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
        cur_len = strlen(c);

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
       
        
        printf("%s\n", c);
    }

    temp = L->head;  // start from the head of L
    N_char = 0;
    while (temp) {
        if (temp->indicator == 10) {
            printf("%s %d\n", "\'\\n\'", temp->count);
        }
        else if (temp->indicator == 32) {
            printf("%s %d\n", "\' \'", temp->count);
        }
        else if (temp->indicator >= 0 && temp->indicator != 10) {
            printf("%c %d\n", temp->En_buff[0], temp->count);
        }
        else {
            printf("%s %d\n", temp->Ch_buff, temp->count);
        }
        N_char++;
        Numchar += temp->count;
        temp = temp->next;  // update temp to next node
    }
    printf("Number of characters read: %d\n", Numchar);
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


// HeapSort

// BinaryMerge
