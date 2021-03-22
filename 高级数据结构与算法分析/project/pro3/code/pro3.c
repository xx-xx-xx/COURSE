#pragma warning(disable : 4996);
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MAX 100000
#define Module 1000000007
#define FILE_NAME "./test1.txt"
//input:
//store the original sequence
int a[MAX];
//store the size of original sequence
int n;
//store the difference limided by the input data
int diff;
//store the dynamical results step by step
int k[MAX];
//bi[k] store the value of 2^k%(10e9+7)
int bi[MAX] = { 0 };
//dynamic programming;
void dyn();
//initialize
void init();
//init the bi sequence
void initbi(int n);
int main() {
    init();
    printf("%d", k[n]);
    return 0;
}
void init() {
    a[0] = 0; //As the reserved bit of the array
    FILE* fp;
    fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        printf("fail to open ");
        printf(FILE_NAME);
        exit(0);
    }
    else {
        fscanf(fp, "%d %d", &n, &diff);
        for (int i = 0; i < n; i++) {
            fscanf(fp, "%d", &a[i + 1]);
        }
        initbi(n);
        dyn();
    }
    fclose(fp);
}
void initbi(int n) {
    bi[0] = 1;
    for (int i = 1; i < n; i++) {
        bi[i] = (bi[i - 1] << 1) % Module;
    }
    return;
}
/*dynamic programming*/
/*Set k[i] as the number of beautiful sequences in the sequence consisting of numbers from a[1] to
the a[i]*/
/*step1 Suppose there are already n-1 elements in the sequence and k[n-1]*/
/*step2 After inserting a [n], first of all, it is combined with the generated k [n-1] beautiful
subsequences, adding k [n-1] beautiful subsequences*/
/*step3 Look for the element that will react with this new element from scratch*/
/*If the found element is a[1]£¬ only one beautiful subsequence will be added*/
/*else if, the number of newly added subsequences is pow(2, j - 1) + k[j - 1] - k[j]*/
void dyn() {
    k[0] = 0;
    for (int i = 1; i <= n; i++) {
        k[i] = (2 * (k[i - 1] % Module)) % Module;
        for (int j = 1; j < i; j++) {
            if (abs(a[j] - a[i]) <= diff && j != 1) {
                k[i] = (k[i]%Module +bi[j-1] + k[j - 1] % Module - k[j] % Module) % Module;
                if (k[i] < 0)k[i] = k[i] + Module;
            }
            else if (abs(a[j] - a[i]) <= diff && j == 1) {
                k[i] = (k[i] % Module + 1) % Module;
            }
        }
    }
}