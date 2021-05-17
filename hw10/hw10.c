// EE3980 HW10 Coin Set Design, II
// 106012017, 林冠廷
// 2021/05/22

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* function declaration */
int NCoinGreedy(int D, int Ncoin, int* Coins);
double GetTime(void);   // get current time from linux system 
int NCoinDP_R(int D, int Ncoin, int Coins[]); // DP recursive approach
int NCoinDP_TD(int D, int Ncoin, int Coins[]); // DP top-down approach
void NCoinDP_BU(int D, int Ncoin, int Coins[]); // DP bottom approach

int inf = 1000000;    // represent infinity
int *g;
int *d;
/* main function */
int main(void)
{
    int Ncoin = 4;      // number of coin type
    int Coins[4] = {1, 5, 10, 50};  // coin value
    int D;      // [1, 99]
    double AveNum = 0;
    double MinAveNum;
    int i, j;      // loop index
    int min_coin_3, min_coin_4; // value of 3rd, 4th coin in Coins list
    double t;

    // Original coin set
    // Recursive DP
    t = GetTime();
    for (D = 1; D < 100; D++) {
        AveNum = AveNum + NCoinDP_R(D, Ncoin, Coins);
    }
    t = (GetTime() - t) / 99;
    AveNum = AveNum / 99;  

    printf("Original coin set:\n");
    printf("  DP recursive: {1, 5, 10, 50} average is %.5lf ", AveNum);
    printf("CPU time: %e sec\n", t);

    // top-down DP
    t = GetTime();
    // initialize g[0:D]
    g = (int*)malloc(100 * sizeof(int));
    for (i = 0; i <= 100; i++) {
        g[i] = -inf;
    }
    AveNum = 0;
    for (D = 1; D < 100; D++) {
        AveNum = AveNum + NCoinDP_TD(D, Ncoin, Coins);
    }
    t = (GetTime() - t) / 99;
    AveNum = AveNum / 99;  
    printf("  DP top-down: {1, 5, 10, 50} average is %.5lf ", AveNum);
    printf("CPU time: %e sec\n", t);

    // bottom-up DP
    t = GetTime();
    d = (int*)malloc(100 * sizeof(int));
    for (i = 1; i <= 100; i++) {
        d[i] = inf;
    }
    AveNum = 0;
    D = 99;
    // only call once for bottom-up approach
    NCoinDP_BU(D, Ncoin, Coins);
    for (i = 1; i < 100; i++) {
        // printf("%d %d\n", i, d[i]);
        AveNum += d[i];
    }
    t = (GetTime() - t) / 99;
    AveNum = AveNum / 99;  
    printf("  DP bottom-up: {1, 5, 10, 50} average is %.5lf ", AveNum);
    printf("CPU time: %e sec\n", t);



    // // change 50 to coin which ranges from 11 to 99 in 4th index
    // MinAveNum = inf; 
    // for (i = 11; i <100; i++) {
    //     Coins[3] = i;
    //     AveNum = 0;
    //     for (D = 1; D < 100; D++) {
    //         AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
    //     }
    //     AveNum = AveNum / 99;
    //     if (AveNum < MinAveNum) {
    //         MinAveNum = AveNum;
    //         min_coin_4 = i;
    //     }
    // }
    // printf("Coin set {1, 5, 10, %d} has the minimum average of %.5lf\n", 
    //                                 min_coin_4, MinAveNum);
    // Coins[3] = 50;  // recover 50 dollar for 4th coin

    // // change 10 to coin which ranges from 6 to 49 in 3th index
    // MinAveNum = inf; 
    // for (i = 6; i < 50; i++) {
    //     Coins[2] = i;
    //     AveNum = 0;
    //     for (D = 1; D < 100; D++) {
    //         AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
    //     }
    //     AveNum = AveNum / 99;
    //     if (AveNum < MinAveNum) {
    //         MinAveNum = AveNum;
    //         min_coin_3 = i;
    //     }
    // }
    // printf("Coin set {1, 5, %d, 50} has the minimum average of %.5lf\n", 
    //                                 min_coin_3, MinAveNum);

    // // change both 10 and 50 in 3rd and 4th index
    // // 3rd coin ranges from 6 to 98
    // // 4th coin ranges from "3rd value" to 99
    // MinAveNum = inf; 
    // for (i = 6; i < 99; i++) {
    //     Coins[2] = i;
    //     for (j = i + 1; j < 100; j++) {
    //         Coins[3] = j;
    //         AveNum = 0;
    //         for (D = 1; D < 100; D++) {
    //             AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
    //         }
    //         AveNum = AveNum / 99;
    //         if (AveNum < MinAveNum) {
    //             MinAveNum = AveNum;
    //             min_coin_3 = i;
    //             min_coin_4 = j;
    //         }
    //     }
    // }
    // printf("Coin set {1, 5, %d, %d} has the minimum average of %.5lf\n", 
    //                     min_coin_3, min_coin_4, MinAveNum);

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

// NCoinGreedy: find minimum number of coins for D
// Input: D input dollar ranging from [1,99], Ncoin number of coin types, 
// Coins[1:Ncoin] dollar of each type of coins.
// Output: num number of coins needed for D
int NCoinGreedy(int D, int Ncoin, int* Coins)
{
    int i;      // loop index
    int num = 0;    // initialize num to 0
    bool done;   // whether is done

    // select the coin that smaller than D and closest to D
    while (D > 0) {
        done = false;
        i = Ncoin - 1;  // start from the largest to smallest coin
        while (i >= 0 && done==false) {
            if (D - Coins[i] >= 0) {  // Coin[i] <= D
                done = true;    // terminate the inner while loop
                D = D - Coins[i]; // update D after subtract current coin
            }
            i--;    // update to one-step smaller coin index
        }    
        num++;      // update num
    }
    return num;
}

// DP recursive approach
int NCoinDP_R(int D, int Ncoin, int Coins[])
{
    int Xn;     
    int min = inf; 
    int Cn = Coins[Ncoin - 1];
    int gn;

    // termination condiction
    // use C1 to represent all remaining dollars
    if (Ncoin == 1) {
        return D / Cn;
    }
    for (Xn = 0; Xn <= D / Cn; Xn++) {
        gn = Xn + NCoinDP_R(D - Xn * Cn, Ncoin - 1, Coins);
        if (gn < min) {
            min = gn;
        }
    }
    
    return min;
}

// // DP top-down approach
int NCoinDP_TD(int D, int Ncoin, int Coins[])
{
    int Xn;     
    int min = inf; 
    int Cn = Coins[Ncoin - 1];
    int gn;
    // use value in g
    if (g[D] > 0) {
        return g[D];
    }
    // termination condiction
    // use C1 to represent all remaining dollars
    if (Ncoin == 1) {
        return D / Cn;
    }
    for (Xn = 0; Xn <= D / Cn; Xn++) {
        gn = Xn + NCoinDP_TD(D - Xn * Cn, Ncoin - 1, Coins);
        if (gn < min) {
            min = gn;
        }
    }
    g[D] = min;
    return min;
}

// // DP bottom-up approach
void NCoinDP_BU(int D, int Ncoin, int Coins[])
{
    int i; 
    int min = inf;
    int gn, Cn, Xn;
    int n;

    d[1] = 1;

    for (i = 2; i <= D; i++) {
        min = inf;
        n = Ncoin;
        // while (n > 0) {
            Cn = Coins[n - 1];
            for (Xn = 0; Xn <= i / Cn; Xn++) {
                gn = Xn + d[i - Xn * Cn];
                if (gn < min) {
                    min = gn;
                }
            }
            // n--;
        // }
        d[i] = min;
    }
}
