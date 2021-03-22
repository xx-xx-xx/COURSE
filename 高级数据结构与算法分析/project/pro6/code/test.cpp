#include <iostream>
#include <cstdlib>
#include <ctime>
#include "SkipList.h"
#define MAX_LEVEL 16
#define MAX_NUM 100000
using namespace std;

int main()
{
    clock_t start, total;
    clock_t insertion[14] = {0}, search[14] = {0}, deletion[14] = {0};
    int Test_set[MAX_NUM] = {0};
    int size[14] = {5000, 6000, 7000, 8000, 9000, 10000, 12000, 14000, 16000, 18000, 20000, 30000, 40000, 50000};

    //read in test set
    freopen("test_set.in", "r", stdin);
    freopen("test_result.out", "w", stdout);

    //performance test
    SkipList_Entry<int> mysl;
    printf("Start performance test:\n");
    for (int i = 0; i < 10; i++)
    {
        Entry<int> my(i, i);
        mysl.insert(&my);
        cout << "after insert key=" << i << " value=" << i << endl;
        mysl.printList();
    }

    for (int i = 0; i < 5; i++)
    {
        Entry<int> my(i, i);
        mysl.remove(&my);
        cout << "after delete key=" << i << " value=" << i << endl;
        mysl.printList();
    }

    for (int i = 6; i < 12; i++)
    {
        Entry<int> my(i, i);
        bool exist = mysl.search(&my);
        cout << "after search key=" << i << " value=" << i << endl;
        if (exist)
            cout << "exist!" << endl;
        else
            cout << "not exist!" << endl;
    }

    //run time test
    printf("\nStart run time test:\n");
    for (int i = 0; i < MAX_NUM; i++)
        scanf("%d", &Test_set[i]);
    for (int i = 0; i < 14; i++)
    {
        SkipList_Entry<int> Test;

        for (int j = 0; j < size[i]; j++)
        {
            Entry<int> my(Test_set[j], j);
            Test.insert(&my);
        }
        start = clock();
        for (int j = 99000; j > 98000; j--)
        {
            Entry<int> my(Test_set[j], j);
            Test.insert(&my);
        }
        total = clock();
        insertion[i] = total - start;

        start = clock();
        for (int j = 99000; j > 98000; j--)
        {
            Entry<int> my(Test_set[j], j);
            Test.remove(&my);
        }
        total = clock();
        deletion[i] = total - start;

        start = clock();
        for (int j = 0; j < 1000; j++)
        {
            Entry<int> my(Test_set[j], j);
            Test.search(&my);
        }
        total = clock();
        search[i] = total - start;

        printf("Now handling %d\n", i);
    }

    printf("Search:------------------------------\n");
    for (int i = 0; i < 14; i++)
    {
        printf("%.3f\n", (double)search[i] / (double)CLOCKS_PER_SEC);
    }
    printf("Insertion:----------------------------\n");
    for (int i = 0; i < 14; i++)
    {
        printf("%.3f\n", (double)insertion[i] / (double)CLOCKS_PER_SEC);
    }
    printf("Deletion:-----------------------------\n");
    for (int i = 0; i < 14; i++)
    {
        printf("%.3f\n", (double)deletion[i] / (double)CLOCKS_PER_SEC);
    }

    fclose(stdin);
    fclose(stdout);
    system("pause");
    
    return 0;
}