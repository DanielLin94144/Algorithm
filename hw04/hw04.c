// EE3980 HW04 Stock Short Selling
// 106012017 林冠廷
// 2021/04/03

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
/* structure declaration */ 
// structure "STKprice": all the information provided by the input data
typedef struct sSTKprice {
    int year, month, day;
    double price, change; // the difference: current - previous
} STKprice;
// structure "Ans": to save the variable that related to the final answer
// low and high are the range of maximum/minimum subarray.
// sum is the summation in [low, high] range.
typedef struct answer {
    int low, high;
    double sum;
} Ans;

/* global variable declaration */
STKprice *A;    // the pinter of 1d array that store all data information
int repeatation = 5000;
int N;  // the number of records

/* function declaration */
double GetTime(void);
void readData(void);
Ans MaxSubArrayBF(void);    // brute-force approach
Ans MaxSubArray(int begin, int end);    // divide-and-conquer approach
Ans MaxSubArrayXB(int begin, int mid, int end); // cross-boundary funciton

int  main(void)
{
    double t;   // a variable to measure time in second 
    int i;      // loop index
    Ans ans;        // use Ans structure variable "ans" to save answer
                    // including (1) low index (2) high index (3) sum
    readData();        // read the data and save to A
    printf("N = %d\n", N);      // print out the number of records 

    // only measure Brute-force approach one time
    t = GetTime();
    ans = MaxSubArrayBF();
    t = GetTime() - t; 
    printf("Brute-fore approach: CPU time %e s\n", t);
    printf("Sell: %d/%d/%d at %lf\n", A[ans.low - 1].year, \
            A[ans.low - 1].month, A[ans.low - 1].day, A[ans.low - 1].price);
    // low index is low - 1 because the actual starting day is previous day  
    // now the low index depends on "change" array                                 
    printf("Buy: %d/%d/%d at %lf\n", A[ans.high].year, A[ans.high].month, \
                                        A[ans.high].day, A[ans.high].price);
    printf("Earning: %lf per share.\n", -ans.sum);  
    // add negative symbol since it is the min summation

    // divide-and-conquer CPU time measurement
    t = GetTime();
    // declare the begin and end (the input parameter of MaxSubArray)
    int begin = 0;
    int end = N - 1;
    for (i = 0; i < repeatation; i++) {
        ans = MaxSubArray(begin, end);
    }
    t = (GetTime() - t) / repeatation; 
    printf("Divide and Conquer: CPU time %e s\n", t);
    printf("Sell: %d/%d/%d at %lf\n", A[ans.low - 1].year, \
            A[ans.low - 1].month, A[ans.low - 1].day, A[ans.low - 1].price);
    // low index is low - 1 because the actual starting day is previous day
    // now the low index depends on "change" array
    printf("Buy: %d/%d/%d at %lf\n", A[ans.high].year, A[ans.high].month, \
                                        A[ans.high].day, A[ans.high].price);
    printf("Earning: %lf per share.\n", -ans.sum);  
    // add negative symbol since it is the min summation
    return 0;
}

/* function implementation */
void readData(void)
{
    int i;  // loop index
    scanf("%d", &N);
    
    // allocate memory for STKprice array A
    A = (STKprice*)malloc(N * sizeof(STKprice));
    for (i = 0; i < N; i++) {  // read the line N times 
        scanf("%d %d %d %lf", &(A[i].year), &(A[i].month), &(A[i].day) \
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
}
// brute-force approach implementation
Ans MaxSubArrayBF(void) 
{
    double min = 0.0;   // record the minimum
    double sum;      
    int low = 0;
    int high = N-1;
    int j, k, i; // loop index
    Ans ans;  // Ans structure variable ans to return the answer information

    for (j = 0; j < N; j++) { 
        for (k = j; k < N; k++) { // try out all possibility
            sum = 0.0;
            for (i = j; i <= k; i++) {
                sum = sum + A[i].change;
            }
            if (sum < min) {
                // if the adding new element makes lower summation
                // record low and high, update min
                min = sum; 
                low = j;
                high = k;
            }
        }
    }
    // assign answer information to ans
    ans.sum = min;
    ans.low = low;
    ans.high = high;

    return ans;
}

// divide-and-conquer approach
// begin <= low <= high <= end
Ans MaxSubArray(int begin, int end)
{
    Ans ans, lans, rans, xans; 
    int mid; 
    if (begin == end) {     // termination condition: only one element 
        ans.low = begin; ans.high = end;
        ans.sum = A[begin].change;
        return ans;
    }
    mid = (begin + end) / 2;    // floor to integer
    // divide to left, right and cross boundary part
    lans = MaxSubArray(begin, mid);
    rans = MaxSubArray(mid + 1, end);
    xans = MaxSubArrayXB(begin, mid, end);
    // doing comparison for selecting the best ans in the three choices.
    if (lans.sum <= rans.sum && lans.sum <= xans.sum) {
        return lans;
    } 
    else if (rans.sum <= lans.sum && rans.sum <= xans.sum) {
        return rans;
    }
    else {
        return xans;
    }
}

Ans MaxSubArrayXB(int begin, int mid, int end)
{
    Ans ans;    
    double sum; 
    int i;      // loop index
    // left side 
    double lsum = 0.0;
    int low = mid;
    sum = 0.0; 
    for (i = mid; i >= begin; i--) { // from middle to the beginning
        sum = sum + A[i].change;
        if (sum < lsum) { 
        // if the adding new element makes lower summation
        // update lsum and record low
            lsum = sum;
            low = i;
        }
    }
    // right side
    double rsum = 0.0;
    int high = mid + 1;
    sum = 0.0;      // re-initialize sum to 0.0
    for (i = mid + 1; i <= end; i++) { // from middle + 1 to the end
        sum = sum + A[i].change;
        if (sum < rsum) {
        // if the adding new element makes lower summation
        // update rsum and record high
            rsum = sum;
            high = i;
        }
    }
    // assign answer information to ans
    ans.low = low;
    ans.high = high;
    ans.sum = lsum + rsum;
    return ans;
}

double GetTime(void)
{                       // get current time from linux
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}





