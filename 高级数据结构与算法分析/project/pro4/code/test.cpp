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

std::map<int, char> Code_dict; //�ֵ�Code_dict
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
	char c; //��ʾ�ַ�
	int frequency; //��ʾ���ַ����ֵĴ�����Ƶ��
	Node* left;
	Node* right;
	Node(char _c, int f, Node* l = NULL, Node* r = NULL)
		:c(_c), frequency(f), left(l), right(r) { }
	bool operator<(const Node& node) const { //����<���㷨�������ڼ������ȶ��е�ʱ�������δ�����λ��
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

//�����������
void huffmanTree(priority_queue<Node>& q) {
	while (q.size() != 1) {
		Node* left = new Node(q.top()); q.pop();
		Node* right = new Node(q.top()); q.pop();
		Node node('R', left->frequency + right->frequency, left, right);
		q.push(node);
	}
}

// ��ӡ����������
void huffmanCode(Node* root, string& prefix, map<char, string>& result) {
	string m_prefix = prefix;
	if (root->left == NULL) return;
	//����������
	prefix += "0";
	//�����Ҷ�ӽ�������,����ݹ��ӡ������
	if (root->left->left == NULL) result[root->left->c] = prefix;
	else huffmanCode(root->left, prefix, result);
	//��ԭԭ����·��,����
	prefix = m_prefix;
	//����������
	prefix += "1";
	//�����Ҷ�ӽ�㣬�����, ����ݹ��ӡ������
	if (root->right->right == NULL) result[root->right->c] = prefix;
	else huffmanCode(root->right, prefix, result);
}

void testResult(map<char, string> result) {
	//����map����
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
	//��ʼ���ַ���Ϣ
	cout << "Please input the number of N and M:";
	cin >> N >> M;

	// ����cout��������ָ��
	streambuf* coutBuf = cout.rdbuf();
	ofstream of("out.txt");
	// ��ȡ�ļ�out.txt��������ָ��
	streambuf* fileBuf = of.rdbuf();
	// ����cout��������ָ��Ϊout.txt����������ָ��
	cout.rdbuf(fileBuf);

	cout << N << endl;
	//����i��Ϊkey, ��Ӧ�ַ���Ϊ���� 
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
	//��������ַ��Լ���Ӧ��Ƶ�ʣ�������������
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

	///**ʹ��Huffman�㷨�����ȷ�ı���**///
	initNode(q, N);
	//�����������
	huffmanTree(q);
	//�������������
	Node root = q.top();
	string prefix = "";
	map<char, string> result;
	huffmanCode(&root, prefix, result);
	//�������Ƿ���ȷ
	for (int j = 1; j <= M / 2; j++) testResult(result);

	///**ʹ�õȳ�����б���**///
	//�ж���Ҫ�ü�λ������λ
	int num = 1, k = N;
	while ((k = k / 2) != 0)
	{
		num++;
	}
	//����code��ֵ,����ַ�����Ӧ�ı���
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
	// �ָ�coutԭ������������ָ��
	cout.rdbuf(coutBuf);
	return 0;
}
