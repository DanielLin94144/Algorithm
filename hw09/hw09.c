// EE3980 HW09 Coin Set Design
// 106012017, 林冠廷
// 2021/05/15

#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>

/* function declaration */
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
int NCoinGreedy(int D, int Ncoin, int* Coins)
{
    int i;      // loop index
    int num = 0;
    bool done; 

    while (D > 0) {
        // compute the difference between current number and 
        // each type of coins
        done = false;
        i = Ncoin - 1;  // start from large to small coin
        while (i >= 0 && done==false) {
            if (D - Coins[i] >= 0) {
                done = true;
                D = D - Coins[i];
            }
            i--;
        }    
        num++;
    }
    return num;
}