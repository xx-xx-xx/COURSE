#include<cstdio>
#include<string>
#include<iostream>
#include<cstdlib>
#include"vector.hpp"
using namespace std;

int main() {
	
	cout << "Now you are testing INT type Vector" << endl;

	//creates a vector for holding 'size' elements
	cout << "Testing Vector(int size)" << endl;
	cout << "size=";
	int size;
	cin >> size;
	Vector<int>myVec(size);
	cout << "Success\n"<<endl;
	
	//creates an empty vector and push_back some elements
	cout << "Testing Vector()" << endl;
	cout << "How many numbers will you input" << endl;
	int num;
	cin >> num;
	cout << "Please enter the numbers" << " like 6 7 8 2" << endl;
	Vector<int>myVec2;
	for (int i = 0; i < num; i++) {
		int tmp;
		cin >> tmp;
		myVec2.push_back(tmp);
	}
	cout << "testing int size() const		Size of the vector is: " << myVec2.size()<<endl;
	cout << "testing T& operator[](int index)	The elements in it is: ";
	for (int i = 0; i < myVec2.size(); i++) {
		cout << " " << myVec2[i];
	}
	cout <<"\n"<< endl;

	//the copy ctor
	cout << "testing Copy Vector";
	Vector<int>copyVec(myVec2);
	cout << "Size of the copied vector is: " << copyVec.size() << endl;
	cout << "The elements in it is: ";
	for (int i = 0; i < copyVec.size(); i++) {
		cout << " " << copyVec[i];
	}
	cout << endl;
	
	cout << "testing void clear()"<< "		now the size of the vector:";
	myVec2.clear();
	cout << myVec2.size()<<endl;
	cout << "testing bool empty() const		is it empty:" << myVec2.empty()<<"\n"<<endl;


	Vector<int>myVec3;
	cout << "\ntesting T& at(int index) may cause an exception, enter anything to begin" << endl;
	char x;
	cin >> x;
	for (int i = 0; i < 10 ; i++) {
		srand(i);
		myVec3.push_back(rand());
	}
	cout << "Size of the vector is: " << myVec3.size() << endl;
	cout << "The elements in it is: ";
	for (int i = 0; i < myVec3.size(); i++) {
		cout << " " << myVec3[i];
	}
	cout << "\nPlease enter the index you want to check: ";
	int i;
	cin >> i;
	cout<<myVec3.at(i);
	system("pause");
}
