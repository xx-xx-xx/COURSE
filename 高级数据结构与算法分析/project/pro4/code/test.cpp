#define _CRT_SECURE_NO_WARNINGS

/*
Test cases generator
--------------------------------
generate the input cases of Huffman code

Input: N & M
Output: (into one .txt file)
N
2*N numbers(consist of array of characters and frequencies)
M
M*N lines of possible codes
*/

#include <queue>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <vector>
#include <iomanip>
#include <time.h>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

#define MAXN 63
#define MAXM 1000

std::map<int, char> Code_dict; //字典Code_dict
char sym[MAXN];
int freq[MAXN];

int Binary(int i)
{
	if (i < 2) return i;
	else
	{
		return (Binary(i / 2) * 10 + i % 2);
	}
}

class Node {
public:
	char c; //表示字符
	int frequency; //表示该字符出现的次数或频率
	Node* left;
	Node* right;
	Node(char _c, int f, Node* l = NULL, Node* r = NULL)
		:c(_c), frequency(f), left(l), right(r) { }
	bool operator<(const Node& node) const { //重载<运算法以至于在加入优先队列的时候决定如何处理结点位置
		return frequency > node.frequency;
	}
};

void initNode(priority_queue<Node>& q, int N)
{
	for (int i = 0; i < N; i++)
	{
		Node node(sym[i], freq[i]);
		q.push(node);
	}
}

//构造哈夫曼树
void huffmanTree(priority_queue<Node>& q) {
	while (q.size() != 1) {
		Node* left = new Node(q.top()); q.pop();
		Node* right = new Node(q.top()); q.pop();
		Node node('R', left->frequency + right->frequency, left, right);
		q.push(node);
	}
}

// 打印哈夫曼编码
void huffmanCode(Node* root, string& prefix, map<char, string>& result) {
	string m_prefix = prefix;
	if (root->left == NULL) return;
	//处理左子树
	prefix += "0";
	//如果是叶子结点则输出,否则递归打印左子树
	if (root->left->left == NULL) result[root->left->c] = prefix;
	else huffmanCode(root->left, prefix, result);
	//还原原来的路径,回溯
	prefix = m_prefix;
	//处理右子树
	prefix += "1";
	//如果是叶子结点，则输出, 否则递归打印右子树
	if (root->right->right == NULL) result[root->right->c] = prefix;
	else huffmanCode(root->right, prefix, result);
}

void testResult(map<char, string> result) {
	//迭代map容器
	map<char, string>::const_iterator it = result.begin();
	while (it != result.end())
	{
		cout << it->first << ' ' << it->second << endl;
		++it;
	}
}

int main() 
{
	priority_queue<Node> q;
	int N, M;
	//初始化字符信息
	cout << "Please input the number of N and M:";
	cin >> N >> M;

	// 保存cout流缓冲区指针
	streambuf* coutBuf = cout.rdbuf();
	ofstream of("out.txt");
	// 获取文件out.txt流缓冲区指针
	streambuf* fileBuf = of.rdbuf();
	// 设置cout流缓冲区指针为out.txt的流缓冲区指针
	cout.rdbuf(fileBuf);

	cout << N << endl;
	//数字i作为key, 对应字符作为内容 
	int c = 48;
	int* Flag = (int*)malloc(sizeof(int) * 63);
	for (int i = 0; i < 10 + 26 * 2 + 1; i++, c++)
	{
		if (i == 10) c = 65;
		if (i == 36) c = 95;
		if (i == 37) c = 97;
		Code_dict[i] = c;
		//0 48 A 65 _ 95 a 97 
		Flag[i] = 0;
	}
	//随机生成字符以及对应的频率，保存在数组中
	srand((unsigned)(time(NULL)));
	for (int i = 0; i < N; i++)
	{
		int a = rand() % 63;
		if (!Flag[a])
		{
			sym[i] = Code_dict[a];
			int b = rand() % 1000 + 1;
			freq[i] = b;
			cout << sym[i] << ' ' << freq[i] << ' ';
			Flag[a] = 1;
		}
		else i--;
	}
	free(Flag);
	cout << endl;
	cout << M << endl;

	///**使用Huffman算法算出正确的编码**///
	initNode(q, N);
	//构造哈夫曼树
	huffmanTree(q);
	//构造哈夫曼编码
	Node root = q.top();
	string prefix = "";
	map<char, string> result;
	huffmanCode(&root, prefix, result);
	//检验结果是否正确
	for (int j = 1; j <= M / 2; j++) testResult(result);

	///**使用等长码进行编码**///
	//判断需要用几位二进制位
	int num = 1, k = N;
	while ((k = k / 2) != 0)
	{
		num++;
	}
	//对于code赋值,输出字符所对应的编码
	for (int j = M / 2 + 1; j <= M; j++)
	{
		for (int i = 1; i <= N; i++)
		{
			int code = 0;
			code = Binary(i);
			cout << sym[i - 1] << " ";
			if (j == M && i == N)
			{
				cout.width(num);
				cout.fill('0');
				cout << code;
			}
			else
			{
				cout.width(num);
				cout.fill('0');
				cout << code << endl;
			}
			
		}
	}

	of.flush();
	of.close();
	// 恢复cout原来的流缓冲区指针
	cout.rdbuf(coutBuf);
	return 0;
}
