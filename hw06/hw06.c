// EE3980 HW06 Stock Short Selling Revisted
// 106012017, 林冠廷
// 2021/04/17

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct sSTKprice {
    int year, month, day;
    double price, change; // the difference: current - previous
} STKprice;
// structure "Ans": to save the variable that related to the final answer
// low and high are the range of minimum subarray.
// sum is the summation in [low, high] range.
typedef struct answer {
    int low, high;
    double sum;
} Ans;

/* global variable declaration */
int N;  // the number of records

/* function declaration */
double GetTime(void);   // get current time from linux system 
STKprice* readData(STKprice* A);    // read stock data to A
Ans MinSubArrayBF_improved(STKprice* A);    // improved brute-force approach 
Ans MinSubArray_faster(STKprice* A);   // faster dynamic programming approach

/* main function */
int main(void)
{
    double t;   // a variable to measure time in second 
    int i;      // loop index
    STKprice *A;    // the pinter of 1d array that store all data information
    int repeatation = 5000;  // number of repeatation

    Ans ans;        // use Ans structure variable "ans" to save answer
                    // including (1) low index (2) high index (3) sum

    A = readData(A);        // read the data and save to A
    printf("N = %d\n", N);      // print out the number of records 

    // CPU time measurement for improved brute-force approach
    t = GetTime();
    for (i = 0; i < repeatation; i++) {
        ans = MinSubArrayBF_improved(A);
    }
    t = (GetTime() - t) / repeatation; 
    printf("Improved Brute-fore approach: CPU time %e s\n", t);
    printf("Sell: %d/%d/%d at %lf\n", A[ans.low].year, 
            A[ans.low].month, A[ans.low].day, A[ans.low].price);
                           
    printf("Buy: %d/%d/%d at %lf\n", A[ans.high].year, A[ans.high].month, 
                                        A[ans.high].day, A[ans.high].price);
    printf("Earning: %lf per share.\n", -ans.sum);  
    // add negative symbol since it is the min summation

    // CPU time measurement for faster algorithm 
    t = GetTime();
    for (i = 0; i < repeatation; i++) {
        ans = MinSubArray_faster(A);
    }
    t = (GetTime() - t) / repeatation; 
    printf("Faster approach: CPU time %e s\n", t);
    printf("Sell: %d/%d/%d at %lf\n", A[ans.low].year, 
            A[ans.low].month, A[ans.low].day, A[ans.low].price);
    printf("Buy: %d/%d/%d at %lf\n", A[ans.high].year, A[ans.high].month, 
                                        A[ans.high].day, A[ans.high].price);
    printf("Earning: %lf per share.\n", -ans.sum);  
    // add negative symbol since it is the min summation

    return 0;
}

/* function implementation */
// readData: read input stock data line by line to A.
STKprice* readData(STKprice* A)
{
    int i;  // loop index
    scanf("%d", &N);
    
    // allocate memory for STKprice array A
    A = (STKprice*)malloc(N * sizeof(STKprice));
    for (i = 0; i < N; i++) {  // read the line N times 
        scanf("%d %d %d %lf", &(A[i].year), &(A[i].month), &(A[i].day)
                                , &(A[i].price));
        // calculate "change"
        if (i == 0) {
            A[i].change = 0.0;
        }
        else {
            // the difference between current and previous price 
            A[i].change = A[i].price - A[i - 1].price;
        }
    }

    return A;
}

// Getime: current time from linux
double GetTime(void)
{           
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}

// MinSubArrayBF_improved: improve brute-force minimum subarray problem by 
// problem transformation to avoid summation in double for loop.
Ans MinSubArrayBF_improved(STKprice* A)
{
    double min = 0.0;   // record the minimum
    double diff;      
    int low = 0;
    int high = N-1;
    int i, j; // loop index
    Ans ans;  // Ans structure variable ans to return the answer information

    for (i = 0; i < N; i++) { 
        for (j = i + 1; j < N; j++) { // try out all possibility
            diff = A[j].price - A[i].price;
            
            if (diff < min) {
                // record low and high, update min
                min = diff; 
                low = i;
                high = j;
            }
        }
    }
    // assign answer information to ans
    ans.sum = min;
    ans.low = low;
    ans.high = high;

    return ans;
}

// MinSubArray_faster: apply dynamic programming to 
Ans MinSubArray_faster(STKprice* A)
{
    Ans ans;  // Ans structure variable ans to return the answer information
    int i;  // loop index
    int low = 0; 
    int high = N - 1;
    double global_min = 0.0; // global min for whole array
    double local_min = 0.0; // local min for certain range

    for (i = 0; i < N; i++) {
        // A[i].change or local_min + A[i].change is smaller
        if (A[i].change < local_min + A[i].change) {
            // new starting point
            local_min = A[i].change; 
            low = i;
            high = i;
        }
        else {  // continue adding local subarray range
            local_min = local_min + A[i].change;
        }
        // if finding smaller local_min, update the global_min 
        // and record the high index 
        if (local_min < global_min) {
            global_min = local_min;
            high = i;
        }
    }
    ans.sum = global_min;
    ans.low = low - 1;
    // low index is low - 1 because the actual starting day is previous day  
    // now the low index depends on "change" array      
    ans.high = high;

    return ans;
}
