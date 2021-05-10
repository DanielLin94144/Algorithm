// EE3980 HW09 Coin Set Design
// 106012017, 林冠廷
// 2021/05/15

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

/* function declaration */
double GetTime(void);   // get current time from linux system 
int NCoinGreedy(int D, int Ncoin, int* Coins);

/* main function */
int main(void)
{
    int Ncoin = 4;
    int Coins[4] = {1, 5, 10, 50};
    int D;      // [1, 99]
    double AveNum = 0;
    int i, j;      // loop index
    double MinAveNum;
    int min_coin_3, min_coin_4;

    for (D = 1; D < 100; D++) {
        AveNum = AveNum + NCoinGreedy(D, Ncoin, Coins);
    }
    AveNum = AveNum / 99;
    printf("For coin set {1, 5, 10, 50} the average is %.5lf\n", AveNum);
    // change 50 
    MinAveNum = 101; 
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
    printf("Coin set {1, 5, 10, %d} has the minimum average of %.5lf\n", 
                                    min_coin_4, MinAveNum);
    Coins[3] = 50;  // recover 50 
    // change 10
    MinAveNum = 101; 
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
    printf("Coin set {1, 5, %d, 50} has the minimum average of %.5lf\n", 
                                    min_coin_3, MinAveNum);

    // change both 10 and 50
    MinAveNum = 101; 
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
    printf("Coin set {1, 5, %d, %d} has the minimum average of %.5lf\n", 
                        min_coin_3, min_coin_4, MinAveNum);

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

int NCoinGreedy(int D, int Ncoin, int* Coins)
{
    int diff[4];
    int i;      // loop index
    int min = 101; 
    int chosen_idx; 
    int num = 0;
    // printf("%d: \n", D);
    while (D > 0) {
        // compute the difference between current number and 
        // each type of coins
        for (i = 0; i < Ncoin; i++) {
            diff[i] = D - Coins[i];
        }
        // Greedy select coin: find minimum but > 0
        for (i = 0; i < Ncoin; i++) {
            if (diff[i] < min && diff[i] >= 0) {
                min = diff[i];      // update min
                chosen_idx = i;
            }
        }
        // printf("%d ", Coins[chosen_idx]);
        D = min;
        num++;
    }
    // printf("\n");
    // printf("num %d\n", num);

    return num;

}