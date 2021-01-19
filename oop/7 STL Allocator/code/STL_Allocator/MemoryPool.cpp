#include"MemoryPool.h"

#define block_size 4096
#define block_num 64

//for default
Mem_Pool::Mem_Pool():Mem_Pool(block_num, block_size)
{}

Mem_Pool::Mem_Pool(size_t num, size_t size)
{
	if (num < block_num) num_block = block_num;
	else num_block = num;
	if (size < block_size)size_block = block_size;
	else size_block = size;
	size_pool = (sizeof(block) + size_block)*num_block;

	poolH = operator new(size_pool); //create a pool and now poolH point to the pool
	mnex = nullptr;

	//block[i+1]->pre=block[i+2]
	//block[i+1]->nex=block[i]
	//freeH always points to block with largest index that is not allocated
	if (poolH != nullptr) {
		for (int i = 0; i < num_block; i++) {
			block* x = reinterpret_cast<block*>(static_cast<char*>(poolH) + i * (sizeof(block) + size_block));
			x->pre = nullptr;
			x->nex = freeH;
			if (freeH != nullptr) freeH->pre = x;
			freeH = x;
		}
	}
}

void * Mem_Pool::alloc(size_t size)
{
	if (size > size_block)
		return ::operator new(size);
	else {
		//allocate the block with freeH, change freeH to the next block
		block *x = freeH;
		freeH = x->nex;
		if (freeH != nullptr)
			freeH->pre = nullptr;

		//the allocated block is linked to allocH
		x->nex = allocH;
		if (allocH != nullptr)
			allocH->pre = x;
		allocH = x;
		return static_cast<void*>(reinterpret_cast<char*>(x) + sizeof(block));
	}
	
}

Mem_Pool * Mem_Pool::dealloc(Mem_Pool * cur, Mem_Pool * end,void * ptr, size_t size)
{
	if(cur==nullptr)
		return nullptr;
	else {
		void* tmpx = cur->poolH;
		//if ptr is in the current pool
		if (ptr > tmpx&&ptr < (void*)((char*)tmpx + cur->size_pool)) {
			//get to the current block 
			block * now = reinterpret_cast<block *>(static_cast<char*>(ptr) - sizeof(block));
			if (now == cur->allocH) {
				
				cur->allocH = now->nex;
				if (now->nex != nullptr)
					now->nex->pre = nullptr; //relink the blocks
			}
			else {
				//relink the blocks
				now->pre->nex = now->nex;
				if (now->nex != nullptr)
					now->nex->pre = now->pre;
			}

			now->nex = cur->freeH;//add the current block to the free list
			if (cur->freeH != nullptr)
				cur->freeH->pre = now;
			cur->freeH = now;
			return cur;
		}
		//if ptr is not in the current pool, seek it in the next pool
		else if (cur != end && cur->mnex != nullptr)
			return dealloc(cur->mnex, end, ptr, size);
		else return nullptr;
	}
}

Mem_Pool::~Mem_Pool()
{
	if (poolH != nullptr)
		::operator delete(poolH);
}

Mem_List::Mem_List()
{
	Mem_Pool *k = new Mem_Pool();
	first_pool = k;
	free_pool = first_pool;
}

void * Mem_List::alloc(size_t size)
{
	//if the current memory pool is full, then allocate in the next pool
	if (free_pool->freeH == nullptr) {
		//if there is no next free pool, initialize a new pool
		if (free_pool->mnex == nullptr) {
			free_pool->mnex = new Mem_Pool();
			free_pool->mnex->id = free_pool->id + 1;
		}
		//link the pools
		Mem_Pool* curr = free_pool;
		free_pool = free_pool->mnex;
		free_pool->mpre = curr;
	}
	return free_pool->alloc(size);
}

void Mem_List::dealloc(void * ptr, size_t size)
{
	//find ptr in memory pools
	Mem_Pool* cur = first_pool->dealloc(first_pool, free_pool, ptr,size);
	if (cur == nullptr) {
		operator delete(ptr);
		return;
	}
	//found
	if (cur->allocH == nullptr&&cur != free_pool) {
		//relink pools
		if (cur != first_pool)
		{
			cur->mpre->mnex = cur->mnex;
			cur->mnex->mpre = cur->mpre;
		}
		else {
			first_pool = first_pool->mnex;
			first_pool->mpre = nullptr;
		}
		cur->mnex = free_pool->mnex;
		if (free_pool->mnex != nullptr)
			free_pool->mnex->mpre = cur;
		cur->mpre = free_pool;
		free_pool->mnex = cur;
	}
}
