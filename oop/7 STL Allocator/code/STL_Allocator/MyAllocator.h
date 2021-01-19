#pragma once
#include "MemoryPool.h"
#include <iostream>
#include <limits>

template <typename T>
class MyAlloc {
public:
	typedef T value_type;
	typedef  value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	template<typename U>
	struct rebind { typedef MyAlloc<U> other; };

	MyAlloc() {};
	~MyAlloc() {};
	MyAlloc(MyAlloc const&) {};
	
	template<typename U>
	inline explicit MyAlloc(MyAlloc<U> const&) {}

	inline pointer address(reference _Val) { return &_Val; };  //return address
	inline const_pointer address(const_reference _Val) {return &_Val;}

	//allocate a space
	inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) {
		return reinterpret_cast<pointer>(Memorypool.alloc(cnt * sizeof(T)));
	}

	//deallocate a space
	inline void deallocate(pointer p, size_type n) {
		Memorypool.dealloc(p,n);
	}

	inline size_type max_size() const {
		return std::numeric_limits<size_type>::max() / sizeof(T);
	}

	// 构造函数/析构函数
	inline void construct(pointer p, const T& t) { new(p) T(t); }
	inline void destroy(pointer p) { p->~T(); }

	inline bool operator==(MyAlloc const&) { return true; }
	inline bool operator!=(MyAlloc const& a) { return !operator==(a); }

private:
	static Mem_List Memorypool;


};
template <typename T> Mem_List MyAlloc<T>::Memorypool;