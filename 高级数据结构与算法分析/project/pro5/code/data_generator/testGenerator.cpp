#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
#include<iostream>
using namespace std;
int main() {
	srand(time(0));
	cout << "how many rectangles?" << endl;
	int n;
	cin >> n;
	cout << "Fixed width?" << endl;
	int fixWidth;
	cin >> fixWidth;
	cout << "Biggest height?" << endl;
	int mHeight;
	cin >> mHeight;
	char s[100];

	sprintf(s, "mkdir test%dtimeswith%d", n,fixWidth);
	system(s);
	sprintf(s, "test%dtimeswith%d/data.in", n, fixWidth);
	freopen(s, "w", stdout);
	cout << n << ' ' << fixWidth << endl;
	int i;
	for (i = 0; i < n; i++) {
		int width, height;
		width = rand() % fixWidth;
		height = rand() % mHeight;
		cout << width << " " << height<<" ";
	}
	fclose(stdout);
	sprintf(s, "test%dtimeswith%d/Readme.txt", n, fixWidth);
	freopen(s, "w", stdout);
	cout << "This testing file is generated randomly.\n" << "n=" << n << "   fixed width=" << fixWidth << "\n";
	fclose(stdout);
}
