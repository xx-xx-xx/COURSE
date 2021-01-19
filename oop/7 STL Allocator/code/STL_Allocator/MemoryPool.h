#pragma once
#include<iostream>

using namespace std;

typedef struct block
{
	block* pre = nullptr;
	block* nex = nullptr;

}block;

class Mem_Pool {
public:
	block* allocH = nullptr; //header pointer for allocation
	block* freeH = nullptr; //header pointer for free block
	void* poolH = nullptr; //header pointer of the memory pool
	size_t num_block;  //number of blocks
	size_t size_block;  //size of blocks
	size_t size_pool;  //size of pool
	int id = 0;   //the sequence of block
	Mem_Pool* mpre;  //the previous pool
	Mem_Pool* mnex;  //the next pool

	Mem_Pool();   //initialize a space with default size
	Mem_Pool(size_t num, size_t size); //initialize a space with given blocks and size 
	void* alloc(size_t size); //allocate a space of size "size"
	Mem_Pool* dealloc(Mem_Pool* cur, Mem_Pool* end, void* ptr, size_t size); //deallocate the space ptr points at
	~Mem_Pool(); //destroy the memory pool
};

class Mem_List {
public:
	Mem_Pool* first_pool;  //the first pool in the list
	Mem_Pool* free_pool;  //the first free pool in the list
	Mem_List(); //initialize a list
	void* alloc(size_t size);  //allocate a list with size "size"
	void dealloc(void* ptr, size_t size);  //deallocate a space that ptr points at
};