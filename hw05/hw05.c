// EE3980 HW5 Better Sorts
// 106012017, 林冠廷
// 2021/04/10
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

/* global variable declaration */
int N;                                      // input size
char **A;                                   // array to be sorted
char **B;                                   // array to be sorted 
char **data;                                // input raw data
int R = 5000;                               // number of repetitions
int length = 30;                            // word buffer size

/* function declaration */
void readInput(void);                   // read inputs (dynamic allocation)
void printArray(char **A);              // print the content of array A
void copyArray_simple(void);            // copy array data to existed array A 
double GetTime(void);                   // get local time in seconds

void HeapSort(char **list, int n);  // heapsort function
void Heapify(char **list, int i, int n); // heapify subtree under list[i]
void Merge(char **list, int low, int mid, int high); 
// merge left and right region
void MergeSort(char **list, int low, int high); // mergesort function
int Partition(char **list, int low, int high);
// return mid index by pivot element 
void QuickSort(char **list, int low, int high);
// use mid index by partition function to divide list to left and right region

/* main function */
int main(void)
{
    int i;                                  // loop index
    double t;                               // for CPU time tracking
    int high, low;                          // the range of list [low:high]
    
    readInput();                            // read input to data array 
    printf("N = %d\n", N);                  // print of the number of records
    
    // allocate array A, B (for MergeSort) memory
    A = (char**)malloc(sizeof(char*) * N);
    B = (char**)malloc(sizeof(char*) * N);
    for (i = 0; i < N; i++) {
        A[i] = (char*)malloc(sizeof(char) * (length + 1));
    }
    for (i = 0; i < N; i++) {
        B[i] = (char*)malloc(sizeof(char) * (length + 1));
    }

    // HeapSort CPU time measurement 
    t = GetTime();                          // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray_simple();        // copy data to A
        HeapSort(A, N);
    }
    t = (GetTime() - t) / R;
    printf("HeapSort CPU time: %e seconds\n", t);

    // MergeSort CPU time measurement
    // range of list [low:high] = [0: N - 1]
    low = 0;
    high = N - 1;

    t = GetTime();                          // initialize time counter
    for (i = 0; i < R; i++) {
        copyArray_simple();        // copy data to A
        MergeSort(A, low, high);
    }
    t = (GetTime() - t) / R;
    printf("MergeSort CPU time: %e seconds\n", t);

    // QuickSort CPU time measurement
    t = GetTime();                          // initialize time counter
    // In quicksort, we asume A[high + 1] has infinitely large priority
    // here I set A[high + 1] = "zzzzz" as "infinitely large"
    A[N] = (char*)malloc(sizeof(char) * (length + 1));
    A[N] = "zzzzz";
    for (i = 0; i < R; i++) {
        copyArray_simple();        // copy data to A
        QuickSort(A, low, high);
    }
    t = (GetTime() - t) / R;
    printf("QuickSort CPU time: %e seconds\n", t);
    printArray(A);      // print final sorted result

    return 0;
}

/* funciton implementation */
void readInput(void)
{
    int j;                                  // loop index to read lines
    int len;                                // measured string length
    char word[length];                      // buffer for incoming string
    
    scanf("%d", &N);  // read the first line of the input, number of records
    // allocate memory for array data
    data = (char**)malloc(sizeof(char*) * N);
    
    for (j = 0; j < N; j++) {
        scanf("%s", word);
        // len = strlen(word);                 // measure len of the string
        // allocate memory according to len of string
        data[j] = (char*)malloc(sizeof(char) * (length + 1));   
        strcpy(data[j], word);               // copy buffer string to data[j]
    }
}
// printArray: print wordlist with index from 0 to N
void printArray(char **A)
{
    int j;  // loop index

    for (j = 0; j < N; j++) {
        printf("%d ", j + 1);                 // print the index
        printf("%s", A[j]);                   // print the word
        printf("\n");
    }
}

// copyArray_simple: simply copy every records from "data" to "A"
void copyArray_simple(void)
{
    int j;      // loop index
    char word[length];               // buffer for word from array data

    for (j = 0; j < N; j++) {
        strcpy(word, data[j]);       // copy string from data[j] to buffer
        strcpy(A[j], word);          // copy string from buffer to A
    }
}

// GetTime: get current time from linux
double GetTime(void) {                       
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

/* Sorting algorithm implementation */
void HeapSort(char **list, int n)  
{
    int i;      // loop index 
    char *t;      // temp variable

    // initialize A[1:n] to max heap
    for (i = (n - 1) / 2; i >= 0; i--) {
        // start from the deepest right parents, the index is n / 2
        // i is the root of subtree that needs to heapify       
        Heapify(list, i, n);
    }
    // extract max at A[0] and swap the end to A[0]
    for (i = n - 1; i > 0; i--) {
        t = list[i]; list[i] = list[0]; list[0] = t;
        Heapify(list, 0, i);  // maximun is already in A[n]
    }
}

// Heapify: i is the element that needs to be rearrange 
// to suit the maxheap property
// NOTE: in C implementation, array starts from 0 to n-1
// so the child index is 2 * i + 1 and i is 0 needs to be handle
void Heapify(char **list, int i, int n)
{
    int j;    // A[j] is the lchild
    char *temp;     // temp buffer to save A[i]
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
        if (j < n - 1 && strcmp(list[j], list[j + 1]) < 0){  
            // if rchild is larger
            j = j + 1;  // follow rchild to travse down
        }
        if (strcmp(temp, list[j]) > 0) {   // A[i] is already the max
            done = true;
        }
        else {  // continue going down
            list[(j - 1) / 2] = list[j];
            j = j * 2 + 1;
        }
    }
    list[(j - 1) / 2] = temp;   // temp finds right position
}

// MergeSort: divide list to half part, and merge them 
void MergeSort(char **list, int low, int high)  
{
    int mid = (low + high) / 2;     // middle index

    if (low < high) {       // if at least two elements
        MergeSort(list, low, mid);
        MergeSort(list, mid + 1, high);
        Merge(list, low, mid, high);    // merge left and right part
    }
}

// Merge: merge left and right part to array B, finally copy B to list
void Merge(char **list, int low, int mid, int high) 
{
    int left_idx = low;     // the index of A[low:mid]
    int right_idx = mid + 1;        // the index of A[mid + 1:high]
    int left_count = 0;
    int right_count = 0; 
    int i = low;        // B's index
    int k;      // loop index
    int n1 = mid - low + 1;
    int n2 =  high - mid;

    while (left_count < n1 && right_count < n2) {
        // add smaller element between right and left part
        if (strcmp(list[left_idx + left_count], 
            list[right_idx + right_count]) < 0) { 
            // current left part is smaller
            B[i] = list[left_idx + left_count]; 
            left_count++;       // update counter
        }
        else {      // current right part is smaller
            B[i] = list[right_idx + right_count];
            right_count++;     // update counter
        }
        i++;    // update B index counter
    }
    while (left_count < n1) {
        B[i] = list[left_idx + left_count];
        left_count++;       // update counter
        i++;    // update B index counter
    }
    while (right_count < n2) {
        B[i] = list[right_idx + right_count];
        right_count++;      // update counter
        i++;    // update B index counter
    }
    // copy B to list
    for (k = low; k <= high; k++) {
        list[k] = B[k];
    }
}
// QuickSort:  use mid index given by parition function to divide list 
void QuickSort(char **list, int low, int high)
{
    int mid;

    if (low < high) {
        // assume list[high + 1] is infinite large
        mid = Partition(list, low, high + 1);  
        QuickSort(list, low, mid - 1);
        QuickSort(list, mid + 1, high);
    }
}
// Partition: return mid index and rearrange list so that the right region
// only contains larger elements and the left region only contains smaller 
// elements compared to pivot element
int Partition(char **list, int low, int high)
{
    char* curr = list[low];     
    // pivot element to compare with left and right region elements
    char* t;    // buffer for swapping string
    int i = low;    // the increasing index starts from low 
    int j = high;   // the decreasing index starts from high

    do {
        do {
            i++;    
            // i starts from left to right to find word larger than curr
        } while (strcmp(list[i], curr) < 0); 
        // until find larger element in left region
        do {
            j--;
            // j starts from right to left to find word smaller than curr
        } while (strcmp(list[j], curr) > 0);
        // until find smaller element in right region
        if (i < j) {    // swapping list[i] and list[j]
            t = list[i]; list[i] = list[j]; list[j] = t;
        }
    } while (i < j);
    // move curr to mid position
    list[low] = list[j];
    list[j] = curr;

    return j;   // return mid index for QuickSort to divide
}
