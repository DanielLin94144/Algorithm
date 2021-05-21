// EE3980 HW10 Coin Set Design, II
// 106012017, 林冠廷
// 2021/05/22

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdbool.h>

/* function declaration */
double GetTime(void);   // get current time from linux system 
int NCoinDP_R(int D, int Ncoin, int Coins[]); // DP recursive approach
int NCoinDP_TD(int D, int Ncoin, int Coins[]); // DP top-down approach
void NCoinDP_BU(int D, int Ncoin, int Coins[]); // DP bottom approach

/* global variable declaration */
int inf = 1000000;    // represent infinity
int **g;    // g[1:D][1:Ncoin] table for top-down DP
int *d;     // d[1:D] table for buttom-up DP

/* main function */
int main(void)
{
    int Ncoin = 4;      // number of coin type
    int Coins[4] = {1, 5, 10, 50};  // coin value
    int D;      // [1, 99]
    int D_low = 1;
    int D_high = 99;
    double AveNum = 0;
    double MinAveNum;
    int i, j, k, l;      // loop index
    int min_coin_3, min_coin_4; // value of 3rd, 4th coin in Coins list
    double t;

    // Original coin set
    // Recursive DP
    t = GetTime();
    for (D = D_low; D <= D_high; D++) {
        AveNum = AveNum + NCoinDP_R(D, Ncoin, Coins);
    }
    AveNum = AveNum / D_high;  
    t = (GetTime() - t);
    printf("Original coin set:\n");
    printf("  DP recursive: {1, 5, 10, 50} average is %.5lf ", AveNum);
    printf("CPU time: %g sec\n", t);

    // top-down DP
    t = GetTime();
    // initialize g[0:D]
    g = (int**)malloc((D_high + 1) * sizeof(int*));
    for (i = 0; i <= D_high; i++) {
        g[i] = (int*)malloc((Ncoin + 1) * sizeof(int));
    }
    for (i = D_low; i <= D_high; i++) {
        for (j = 1; j <= Ncoin; j++) {
            g[i][j] = -inf;
        }
    }
    AveNum = 0;
    for (D = D_low; D <= D_high; D++) {
        AveNum = AveNum + NCoinDP_TD(D, Ncoin, Coins);
    }
    AveNum = AveNum / D_high;  
    t = (GetTime() - t);
    printf("  DP top-down: {1, 5, 10, 50} average is %.5lf ", AveNum);
    printf("CPU time: %g sec\n", t);

    // bottom-up DP
    t = GetTime();
    d = (int*)malloc((D + 1) * sizeof(int));
    for (i = 0; i <= D_high; i++) {
        d[i] = 0;
    }
    AveNum = 0;
    D = D_high;
    // only call once for bottom-up approach
    NCoinDP_BU(D, Ncoin, Coins);
    for (i = D_low; i <= D_high; i++) {
        AveNum += d[i];
    }
    AveNum = AveNum / D_high;  
    t = (GetTime() - t);
    printf("  DP bottom-up: {1, 5, 10, 50} average is %.5lf ", AveNum);
    printf("CPU time: %g sec\n", t);

    // change 50 to coin which ranges from 11 to 99 in 4th index
    // Recursive DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 11; i <100; i++) {
        Coins[3] = i;
        AveNum = 0;
        for (D = D_low; D <= D_high; D++) {
            AveNum = AveNum + NCoinDP_R(D, Ncoin, Coins);
        }        
        AveNum = AveNum / D_high;  
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_4 = i;
        }
    }
    t = (GetTime() - t);
    printf("Replacing $50:\n");
    printf("  DP recursive: {1, 5, 10, %d} average is %.5lf ", 
                                            min_coin_4, MinAveNum);
    printf("CPU time: %g sec\n", t);

    // Top-down DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 11; i <100; i++) {
        Coins[3] = i;
        AveNum = 0;
        // initalization of g[1:D][1:Ncoin]
        for (k = D_low; k <= D_high; k++) {
            for (j = 1; j <= Ncoin; j++) {
                g[k][j] = -inf;
            }
        }
        for (D = D_low; D <= D_high; D++) {
            AveNum = AveNum + NCoinDP_TD(D, Ncoin, Coins);
        }
        AveNum = AveNum / D_high;  
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_4 = i;
        }
    }
    t = (GetTime() - t);
    printf("  DP top-down: {1, 5, 10, %d} average is %.5lf ", 
                                            min_coin_4, MinAveNum);
    printf("CPU time: %g sec\n", t);

    // bottom-up DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 11; i <100; i++) {
        Coins[3] = i;
        AveNum = 0;
        // initalization of d[1:D]
        for (k = 0; k <= D_high; k++) {
            d[k] = 0;
        }
        D = D_high;
        // only call once for bottom-up approach
        NCoinDP_BU(D, Ncoin, Coins);
        for (k = D_low; k <= D_high; k++) {
            AveNum += d[k];
        }
        AveNum = AveNum / D_high;  
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_4 = i;
        }
    }
    t = (GetTime() - t);
    printf("  DP bottom-up: {1, 5, 10, %d} average is %.5lf ", 
                                            min_coin_4, MinAveNum);
    printf("CPU time: %g sec\n", t);
    Coins[3] = 50;   // recover $50 to Coins[3]

    // change $10 to coin which ranges from 6 to 49 in 3th index
    // Recursive DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 50; i++) {
        Coins[2] = i;
        AveNum = 0;       
        for (D = D_low; D <= D_high; D++) {
            AveNum = AveNum + NCoinDP_R(D, Ncoin, Coins);
        }
        AveNum = AveNum / D_high;  
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_3 = i;
        }
    }
    t = (GetTime() - t);
    printf("Replacing $10:\n");
    printf("  DP recursive: {1, 5, %d, 50} average is %.5lf ", 
                                            min_coin_3, MinAveNum);
    printf("CPU time: %g sec\n", t);

    // Top-down DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 50; i++) {
        Coins[2] = i;
        AveNum = 0;
        // initalization of g[1:D][1:Ncoin]
        for (k = D_low; k <= D_high; k++) {
            for (j = 1; j <= Ncoin; j++) {
                g[k][j] = -inf;
            }
        }
        for (D = D_low; D <= D_high; D++) {
            AveNum = AveNum + NCoinDP_TD(D, Ncoin, Coins);
        }
        AveNum = AveNum / D_high;  
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_3 = i;
        }
    }
    t = (GetTime() - t);
    printf("  DP top-down: {1, 5, %d, 50} average is %.5lf ", 
                                            min_coin_3, MinAveNum);
    printf("CPU time: %g sec\n", t);

    // bottom-up DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 50; i++) {
        Coins[2] = i;
        AveNum = 0;
        // initalization of d[1:D]
        for (k = 0; k <= D_high; k++) {
            d[k] = 0;
        }
        D = D_high;
        // only call once for bottom-up approach
        NCoinDP_BU(D, Ncoin, Coins);
        for (k = D_low; k <= D_high; k++) {
            AveNum += d[k];
        }
        AveNum = AveNum / D_high;  
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_3 = i;
        }
    }
    t = (GetTime() - t);
    printf("  DP bottom-up: {1, 5, %d, 50} average is %.5lf ", 
                                            min_coin_3, MinAveNum);
    printf("CPU time: %g sec\n", t);

    // change both 10 and 50 in 3rd and 4th index
    // 3rd coin ranges from 6 to 98
    // 4th coin ranges from "3rd value" to 99
    // Recursive DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 99; i++) {
        Coins[2] = i;
        for (j = i + 1; j < 100; j++) {
            Coins[3] = j;
            AveNum = 0;       
            for (D = D_low; D <= D_high; D++) {
                AveNum = AveNum + NCoinDP_R(D, Ncoin, Coins);
            }        
            AveNum = AveNum / D_high;  
            if (AveNum < MinAveNum) {
                MinAveNum = AveNum;
                min_coin_3 = i;
                min_coin_4 = j;
            }
        }
    }
    t = (GetTime() - t);
    printf("Replacing $10 and $50:\n");
    printf("  DP recursive: {1, 5, %d, %d} average is %.5lf ", 
                                min_coin_3, min_coin_4, MinAveNum);
    printf("CPU time: %g sec\n", t);

    // Top-down DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 99; i++) {
        Coins[2] = i;
        for (j = i + 1; j < 100; j++) {
            Coins[3] = j;
            AveNum = 0;
            // initalization of g[1:D][1:Ncoin]
            for (k = D_low; k <= D_high; k++) {
                for (l = 1; l <= Ncoin; l++) {
                    g[k][l] = -inf;
                }
            }
            for (D = D_low; D <= D_high; D++) {
                AveNum = AveNum + NCoinDP_TD(D, Ncoin, Coins);
            }
            AveNum = AveNum / D_high;  
            if (AveNum < MinAveNum) {
                MinAveNum = AveNum;
                min_coin_3 = i;
                min_coin_4 = j;
            }
        }
    }
    t = (GetTime() - t);
    printf("  DP top-down: {1, 5, %d, %d} average is %.5lf ", 
                                min_coin_3, min_coin_4, MinAveNum);
    printf("CPU time: %g sec\n", t);

    // bottom-up DP
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 99; i++) {
        Coins[2] = i;
        for (j = i + 1; j < 100; j++) {
            Coins[3] = j;
            AveNum = 0;
            // initalization of d[1:D]
            for (k = 0; k <= D_high; k++) {
                d[k] = 0;
            }
            D = D_high;
            // only call once for bottom-up approach
            NCoinDP_BU(D, Ncoin, Coins);
            for (k = D_low; k <= D_high; k++) {
                AveNum += d[k];
            }
            AveNum = AveNum / D_high;  
            if (AveNum < MinAveNum) {
                MinAveNum = AveNum;
                min_coin_3 = i;
                min_coin_4 = j;
            }
        }
    }
    t = (GetTime() - t);
    printf("  DP bottom-up: {1, 5, %d, %d} average is %.5lf ", 
                                min_coin_3, min_coin_4, MinAveNum);
    printf("CPU time: %g sec\n", t);

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
        return D;
    }
    for (Xn = 0; Xn <= D / Cn; Xn++) {
        gn = Xn + NCoinDP_R(D - Xn * Cn, Ncoin - 1, Coins);
        if (gn < min) {
            min = gn;
        }
    }
    return min;
}

// DP top-down approach
int NCoinDP_TD(int D, int Ncoin, int Coins[])
{
    int Xn;     
    int min = inf; 
    int Cn = Coins[Ncoin - 1];
    int gn;
    // use value in g
    if (g[D][Ncoin] > 0) {
        return g[D][Ncoin];
    }
    // termination condiction
    // use C1 to represent all remaining dollars
    if (Ncoin == 1) {
        return D;
    }
    for (Xn = 0; Xn <= D / Cn; Xn++) {
        gn = Xn + NCoinDP_TD(D - Xn * Cn, Ncoin - 1, Coins);
        if (gn < min) {
            min = gn;
        }
    }
    g[D][Ncoin] = min;
    return min;
}

// DP bottom-up approach
void NCoinDP_BU(int D, int Ncoin, int Coins[])
{
    int i, j; 
    int min = inf;
    int gn, Cn, Xn;
    int n;
    
    for (i = 1; i <= D; i++) {
        d[i] = i;
    }
    for (i = 2; i <= Ncoin; i++) {
        for (j = 1; j <= D; j++) {
            Cn = Coins[i - 1];
            min = inf;
            for (Xn = 0; Xn <= j / Cn; Xn++) {
                gn = Xn + d[j - Xn * Cn];
                if (gn < min) {
                    min = gn;
                }
            }
            d[j] = min;
        }   
    }
}
