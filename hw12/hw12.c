// EE3980 HW12 Travelling Salesperson Problem
// 106012017, 林冠廷
// 2021/06/12

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* function declaration */
double GetTime(void);   // get current time from linux system 

/* global variable declaration */
int N;  // number of cities
int **d;  // 2D distance array
char **name;    // name of cities
int string_len = 17;    // max length of string: 16 + 1 (null char)
/* main function */
int main(void)
{
    double t;   // CPU running time 
    int D;  // travelling distance
    int i, j;  // loop index
    // read the first line to get N
    scanf("%d", &N);
    // memory allocation 
    name = (char**)malloc(N * sizeof(char*));
    d = (int**)malloc(N * sizeof(int*));
    for (i = 0; i < N; i++) {
        name[i] = (char*)malloc(string_len * sizeof(char));
        d[i] = (int*)malloc(N * sizeof(int));
    }
    // read name of cities
    char buffer[100];
    for (i = 0; i < N; i++) {
        getchar();  // remove "\n"
        scanf("%[^\n]", name[i]);   
    }
    // read distance matrix    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            scanf("%d", &d[i][j]);
        }
    }
    
    for (i = 0; i < N; i++) {
        printf("%s\n", name[i]);
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ",d[i][j]);
        }
        printf("\n");
    }
    
    printf("Minimum distance route:\n");
    // printf("Total travelling distacne: %d\n", D);

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
