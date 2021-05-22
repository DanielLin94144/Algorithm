// EE3980 HW09 Coin Set Design
// 106012017, 林冠廷
// 2021/05/15

#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>

/* function declaration */
int NCoinGreedy(int D, int Ncoin, int* Coins);
double GetTime(void);   // get current time from linux system 
// NCoinGreedy function to find minimum number of coins for D

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
    int inf = 1000000;    // represent infinity
    double t;
    // calculate the average number of coins one needs to carry if 
    // the probabilities of carrying $1 to $99 coins are equal.
    t = GetTime();
    for (D = 1; D < 100; D++) {
        AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
    }
    AveNum = AveNum / 99;  
    t = GetTime() - t;
    printf("For coin set {1, 5, 10, 50} the average is %.5lf\n", AveNum);
    printf("CPU time: %g\n", t);
    // change 50 to coin which ranges from 11 to 99 in 4th index
    MinAveNum = inf; 
    t = GetTime();
    for (i = 11; i <100; i++) {
        Coins[3] = i;
        AveNum = 0;
        for (D = 1; D < 100; D++) {
            AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
        }
        AveNum = AveNum / 99;
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_4 = i;
        }
    }
    t = GetTime() - t;
    printf("Coin set {1, 5, 10, %d} has the minimum average of %.5lf\n", 
                                    min_coin_4, MinAveNum);
    printf("CPU time: %g\n", t);
    Coins[3] = 50;  // recover 50 dollar for 4th coin

    // change 10 to coin which ranges from 6 to 49 in 3th index
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 50; i++) {
        Coins[2] = i;
        AveNum = 0;
        for (D = 1; D < 100; D++) {
            AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
        }
        AveNum = AveNum / 99;
        if (AveNum < MinAveNum) {
            MinAveNum = AveNum;
            min_coin_3 = i;
        }
    }
    t = GetTime() - t;
    printf("Coin set {1, 5, %d, 50} has the minimum average of %.5lf\n", 
                                    min_coin_3, MinAveNum);
    printf("CPU time: %g\n", t);
    // change both 10 and 50 in 3rd and 4th index
    // 3rd coin ranges from 6 to 98
    // 4th coin ranges from "3rd value" to 99
    MinAveNum = inf; 
    t = GetTime();
    for (i = 6; i < 99; i++) {
        Coins[2] = i;
        for (j = i + 1; j < 100; j++) {
            Coins[3] = j;
            AveNum = 0;
            for (D = 1; D < 100; D++) {
                AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
            }
            AveNum = AveNum / 99;
            if (AveNum < MinAveNum) {
                MinAveNum = AveNum;
                min_coin_3 = i;
                min_coin_4 = j;
            }
        }
    }
    t = GetTime() - t;
    printf("Coin set {1, 5, %d, %d} has the minimum average of %.5lf\n", 
                        min_coin_3, min_coin_4, MinAveNum);
    printf("CPU time: %g\n", t);
    return 0;
}

/* function implementation */
// NCoinGreedy: find minimum number of coins for D
// Input: D input dollar ranging from [1,99], Ncoin number of coin types, 
// Coins[1:Ncoin] dollar of each type of coins.
// Output: num number of coins needed for D
int NCoinGreedy(int D, int Ncoin, int* Coins)
{
    int i;      // loop index
    int num = 0;    // initialize num to 0

    // select the coin that smaller than D and closest to D
    i = Ncoin - 1;  // start from the largest to smallest coin
    while (i >= 0 && D > 0) {
        if (D - Coins[i] >= 0) {  // Coin[i] <= D
            D = D - Coins[i]; // update D after subtract current coin
            num++;
        }
        else {
            i--;    // update to one-step smaller coin index
        }
    }    
    return num;
}
// Getime: current time from linux
double GetTime(void)
{           
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return tv.tv_sec + 1e-6 * tv.tv_usec;
}