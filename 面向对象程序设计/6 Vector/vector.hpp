#include<cstdio>
using namespace std;

template <class T>
class Vector {
public:
	Vector();                      // creates an empty vector
	Vector(int size);              // creates a vector for holding 'size' elements
	Vector(const Vector& r);       // the copy ctor
	~Vector();                     // destructs the vector 
	T& operator[](int index);      // accesses the specified element without bounds checking
	T& at(int index);              // accesses the specified element, throws an exception of type 'std::out_of_range' when index <0 or >=m_nSize
	int size() const;		 // return the size of the container
	void push_back(const T& x);    // adds an element to the end 
	void clear();                  // clears the contents
	bool empty() const;            // checks whether the container is empty 
private:
	void inflate();                // expand the storage of the container to a new capacity, e.g. 2*m_nCapacity
	T *m_pElements;                // pointer to the dynamically allocated storage
	int m_nSize;                   // the number of elements in the container
	int m_nCapacity;               // the number of elements that can be held in currently allocated storage
};

template<class T>
Vector<T>::Vector() {
	this->m_nCapacity = 1;
	this->m_nSize = 0;
	this->m_pElements = new T[1];
}

template<class T>
Vector<T>::Vector(int size) {
	m_nCapacity = size;
	m_nSize = 0;
	m_pElements = new T[size];
}

template <class T>
Vector<T>::Vector(const Vector& r) {
	m_nCapacity = r.m_nCapacity;
	m_nSize = r.m_nSize;
	m_pElements = new T[m_nCapacity];
	for (int i = 0; i < r.m_nSize; i++) {
		m_pElements[i] = r.m_pElements[i];
	}
}

template<class T>
Vector<T>::~Vector()
{
	if (this->empty()) {
		m_nCapacity = 0;
		m_nSize = 0;
	}
	else {
		m_nCapacity = 0;
		m_nSize = 0;
		delete[] m_pElements;
	}
	
	
}

template <class T>
T &Vector<T>::operator[](int index)
{
	return m_pElements[index];
}

template<class T>
void Vector<T>::push_back(const T& x) {
	if (m_nCapacity == m_nSize) {
		inflate();
	}
	m_pElements[m_nSize] = x;
	m_nSize++;
}

template<class T>
int Vector<T>::size()const {
	return m_nSize;
}

template<class T>
void Vector<T>::inflate() {
	if (m_nCapacity == 0) {
		m_nCapacity = 1;
		T *tmp = new T[1];
		delete[] m_pElements;
		m_pElements = tmp;
	}
	else {
		T *tmp = new T[m_nCapacity * 2];
		memcpy(tmp, m_pElements, m_nCapacity * sizeof(T));
		m_nCapacity = m_nCapacity * 2;
		m_pElements=new T[m_nCapacity * 2];
		m_pElements = tmp;
	}
}

template<class T>
void Vector<T>::clear() {
	m_nSize = 0;
	delete[] m_pElements;
}

template<class T>
bool Vector<T>::empty() const {
	if (m_nSize == 0)return true;
	return false;
}

template<class T>
T& Vector<T>::at(int index) {
	if (index < m_nSize) {
		return m_pElements[index];
	}
	else {
		throw std::out_of_range("accessed position is out of range");
	}
}
