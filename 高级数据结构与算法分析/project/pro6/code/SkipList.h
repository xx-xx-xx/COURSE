#pragma once
#ifndef SKIPLIST_H_
#define SKIPLIST_H_
#include <ctime>
#include <cstdlib>
#include<iostream>
using namespace std;
template<typename T>
class Entry {
private:
public:
	// The Constructor
	Entry(int k, T v) :value(v), key(k), pNext(nullptr), pDown(nullptr) {}
	// The Copy-constructor
	Entry(const Entry& e) :value(e.value), key(e.key), pNext(nullptr), pDown(nullptr) {}
	int key; 
	T value; 
	Entry* pNext;
	Entry* pDown;

public:
	/* overload operators */
	bool operator<(const Entry& right) {
		return key < right.key;
	}
	bool operator>(const Entry& right) {
		return key > right.key;
	}
	bool operator<=(const Entry& right) {
		return key <= right.key;
	}
	bool operator>=(const Entry& right) {
		return key >= right.key;
	}
	bool operator==(const Entry& right) {
		return key == right.key;
	}
	Entry*& next() {
		return pNext;
	}
	Entry*& down() {
		return pDown;
	}
};
template<typename T>
class SkipList_Entry {
private:
	struct Endpoint {
		Endpoint* up;
		Endpoint* down;
		Entry<T>* right;
	};
	struct Endpoint* header;
	int levelNum; // level_number 已存在的层数
	unsigned int seed;
	bool random() {
		srand(seed);
		int ret = rand() % 2;
		seed = rand();
		return ret == 0;
	}
public:
	SkipList_Entry() :levelNum(1), seed(time(0)) {
		header = new Endpoint();
	}
	/* Insert new element */
	void insert(Entry<T>* entry) { // Insertion is a series of bottom-up operations
		struct Endpoint* cur_header = header;
		// use the linked list header to get to the bottom
		while (cur_header->down != nullptr) {
			cur_header = cur_header->down;
		}
		// It is necessary to insert elements at the bottom
		// Whether to insert the jumping layers above is determined by random */
		int cur_lvl = 0; // current_level
		Entry<T>* temp_entry = nullptr; // Temporarily save a node pointer that has finished inserting
		do {
			Entry<T>* cur_cp_entry = new Entry<T>(*entry); // Copy new objects
			//  determine whether the current layer already exists. If not, add it
			cur_lvl++;
			if (levelNum < cur_lvl) {
				levelNum++;
				Endpoint *new_header = new Endpoint();
				new_header->down = header;
				header->up = new_header;
				header = new_header;
			}
			// !=1 Represents cur_ The header needs to move up and connect to the underlying pointer
			if (cur_lvl != 1) {
				cur_header = cur_header->up;
				cur_cp_entry->down() = temp_entry;
			}
			temp_entry = cur_cp_entry;
			// Whether there is an element node in the current linked list.
			// If it is an empty linked list, assign a value to the right pointer and jump out of the loop
			if (cur_header->right == nullptr) {
				cur_header->right = cur_cp_entry;
				break;
			}
			else {
				Entry<T>* cursor = cur_header->right; // create a cursor
				while (true) { 
					//Find the right insertable point in the current link list cycle
					//jump out of the current cycle after finding it
					if (*cur_cp_entry < *cursor) { 
						//Element is smaller than all elements of the current linked list, insert the chain header
						cur_header->right = cur_cp_entry;
						cur_cp_entry->next() = cursor;
						break;
					}
					else if (cursor->next() == nullptr) { 
						//Element is larger than all elements of the current list, insert the end of the list
						cursor->next() = cur_cp_entry;
						break;
					}
					else if (*cur_cp_entry < *cursor->next()) { // Insert in the middle of the list
						cur_cp_entry->next() = cursor->next();
						cursor->next() = cur_cp_entry;
						break;
					}
					cursor = cursor->next(); // Move cursor right
				}
			}
		} while (random());
	}

	/* search element */
	bool search(Entry<T>* entry) const {
		if (header->right == nullptr) { // judge is_null
			return false;
		}
		Endpoint* cur_header = header;
		// find the accessible point in the top layer
		for (int i = 0; i < levelNum; i++) {
			if (*entry < *cur_header->right) {
				cur_header = cur_header->down;
			}
			else {
				Entry<T>* cursor = cur_header->right;
				while (cursor->down() != nullptr) {
					while (cursor->next() != nullptr) {
						if (*entry <= *cursor->next()) {
							break;
						}
						cursor = cursor->next();
					}
					cursor = cursor->down();
				}
				while (cursor->next() != nullptr) {
					if (*entry > *cursor->next()) {
						cursor = cursor->next();
					}
					else if (*entry == *cursor->next()) {
						return true;
					}
					else {
						return false;
					}
				}
				return false; // Node larger than the last element node, return false
			}
		}
		return false; // no access point is found
	}
	/* delete */
	void remove(Entry<T>* entry) {
		if (header->right == nullptr) {
			return;
		}
		Endpoint* cur_header = header;
		Entry<T>* cursor = cur_header->right;
		int lvl_counter = levelNum; // Obtain the number of llevels before entering the cycle
		for (int i = 0; i < levelNum; i++) {
			if (*entry == *cur_header->right) {
				Entry<T>* delptr = cur_header->right;
				cur_header->right = cur_header->right->next();
				delete delptr;
			}
			else {
				Entry<T> *cursor = cur_header->right;
				while (cursor->next() != nullptr) {
					if (*entry == *cursor->next()) { // find the node
						Entry<T>* delptr = cursor->next();
						cursor->next() = cursor->next()->next();
						delete delptr;
						break;
					}
					cursor = cursor->next();
				}
			}
			// When moving down the chain header pointer
			// determine whether there is an entry node in the current chain list
			if (cur_header->right == nullptr) {
				Endpoint* delheader = cur_header;
				cur_header = cur_header->down;
				header = cur_header;
				delete delheader;
				levelNum--;
			}
			else {
				cur_header = cur_header->down;
			}
		}
	}
	void printList() {
		if (header->right == nullptr) { // Judge whether the right side of the chain header is a null pointer
			return;
		}
		Endpoint* cur_header = header;
		for (int i = 0; i < levelNum; i++) {
			Entry<T>* cursor = cur_header->right;
			cout << "Level" << i << "\t";
			while (cursor->next() != nullptr) {
				cout << cursor->key << ":" << cursor->value<<"  ";
				cursor = cursor->next();
			}
			cout << cursor->key << ":" << cursor->value<<endl;
			cur_header = cur_header->down;
		}
		cout << endl;
	}
};
#endif // !SKIPLIST_H_