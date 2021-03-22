#include <bits/stdc++.h>
using namespace std;
#define MAXVERTEX 1005
#define MAXNUM 105
int num_fruits, num_tips, max_fruits = 0, max_price = 0;
//the number of tips and fruits in the basket, that is, N and M
int graph[MAXNUM][MAXNUM];
int price[MAXNUM], visit[MAXNUM], safe_fruits[MAXNUM];
//store the price of each fruit
int dp[MAXNUM];
//dp[i] is the number of nodes in the maximum clique from i to num_fruits
int id2index[MAXVERTEX], index2id[MAXNUM];
//id2index[id]=index, index2id[index]=id
vector<int> max_basket;
//store the index of max safe fruits in the bastet
vector<pair<int, int>> tips(MAXNUM);
//store the unsafe fruits in pair
void dfs(int i, int sum_price, int step);

int main()
{
    int i, j;
    //initialize the graph (build graph)
    memset(graph, 1, sizeof(graph));

    //start input
    //freopen("test.txt", "r", stdin);
    cin >> num_tips >> num_fruits;
    //read in tips
    for (i = 0; i < num_tips; i++)
        cin >> tips[i].first >> tips[i].second;
    //read in prices
    for (i = 0; i < num_fruits; i++)
    {
        int id, temp_price;
        cin >> id >> temp_price;
        //note that the index and id both start from 1
        id2index[id] = i + 1;
        index2id[i + 1] = id;
        price[id2index[id]] = temp_price;
    }
    //unconnect the unsafe fruits
    //note that the graph's vertex is represented by index
    for (i = 0; i < num_tips; i++)
        graph[id2index[tips[i].first]][id2index[tips[i].second]] = graph[id2index[tips[i].second]][id2index[tips[i].first]] = 0;

    //Bron-Kerbosch Algorithm(Maximum Clique)
    for (i = num_fruits; i > 0; i--)
    {
        visit[0] = i;
        //traversal
        dfs(i, price[i], 1);
        dp[i] = max_fruits;
    }

    //store the id of max safe fruits in max_basket
    for (i = 0; i < max_fruits; i++)
        max_basket.push_back(index2id[safe_fruits[i]]);

    //sort the index of fruits increasingly
    sort(max_basket.begin(), max_basket.end());

    //output
    cout << max_fruits << endl;
    for (i = 0; i < max_fruits - 1; i++)
        printf("%03d ", max_basket[i]);
    printf("%03d\n", max_basket[max_fruits - 1]);
    cout << max_price << endl;

    return 0;
}

void dfs(int i, int sum_price, int step)
{
    int m, n;
    //enumerate each node and record those connected with larger index
    for (m = i + 1; m <= num_fruits; m++)
    {
        int flag = 0;
        if (dp[m] + step < max_fruits || (dp[m] + step == max_fruits && sum_price >= max_price))
            //pruning
            return;      //vertex cannot be added to the clique anymore
        if (graph[i][m]) //i and m are connected
        {
            for (n = 0; n < step; n++)
                if (graph[m][visit[n]] == 0) 
                {   //jump over the loop if m and n are not connected
                    flag = 1;
                    break;
                }
            if (!flag)
            {
                visit[step] = m;
                dfs(m, sum_price + price[m], step + 1); 
                //process the nodes recursively
            }
        }
    }
    //return the maximum results if step>=max_fruits
    if (step > max_fruits || (step == max_fruits && sum_price < max_price))
    {
        for (m = 0; m < step; m++)
            safe_fruits[m] = visit[m];
        max_fruits = step;
        max_price = sum_price;
    }
}
