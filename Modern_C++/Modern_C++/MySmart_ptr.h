#pragma once

#include <stdint.h>
#include <utility>//swap
#include <iostream>

class Shape
{
public:
	virtual ~Shape() { std::cout << " ~Shape() \n"; }

	virtual float Area() = 0;	
	virtual uint32_t Lines() = 0;
	virtual float Length() = 0;
};

class Circle : public Shape
{
public:
	explicit Circle(uint32_t radius) {
		m_radius = radius;
	}
	~Circle(){ std::cout << " ~Circle() \n"; }
	//
	virtual float Area() { return m_base * m_radius * m_radius; }
	virtual uint32_t Lines() { return 1; }
	virtual float Length() { return 2 * m_base * m_radius; }

private:
	uint32_t m_radius = 0;
	const int m_base = 3.1415;
};

//////////////////////////////////////////////////////////////////////////
//Time: 202105025
//description: 实现引用技术的智能指针(uinque_ptr);

template <typename T>
class MySmart_ptr
{
public:
	explicit MySmart_ptr(T *ptr) {//默认构造;
		m_ptr = ptr;
	}

	~MySmart_ptr() {
		delete m_ptr;
		m_ptr = nullptr;
	}
	//禁止拷贝和赋值构造;
	MySmart_ptr(const MySmart_ptr &other) = delete;
	MySmart_ptr& operator= (const MySmart_ptr &other) = delete;

	MySmart_ptr(MySmart_ptr &&other) {
		m_ptr = other.release();
	}
	MySmart_ptr& operator= (MySmart_ptr &&other) {
		other.Swap(*this);
		return *this;
	}


	T* release() {
		T *temp = m_ptr;
		m_ptr = nullptr;
		return temp;
	}

	void Swap(MySmart_ptr& other) {
		using std::swap;
		swap(m_ptr, other.m_ptr);
	}

	//operator;
	T& operator*()	const { return *m_ptr; }
	T* operator->()	const { return m_ptr; }
	operator bool() const { return m_ptr; }

private:
	T *m_ptr = nullptr;
};


//////////////////////////////////////////////////////////////////////////
//Time: 202105026
//description: 实现引用技术的智能指针(shared_ptr) {不是线程安全的};

class Shared_count
{
public:
	Shared_count() : m_count(1){}
	void Add_count() noexcept  { m_count++; }
	long Reduce_count() noexcept { return --m_count; }
	long Get_count() const noexcept { return m_count; }

private:
	long m_count = 0;
};

template <typename T>
class MyShared_ptr
{
public:
	template <typename U>
	friend class MyShared_ptr;

	explicit MyShared_ptr(T *ptr)
	{
		if (ptr) {
			m_ptr = ptr;
			m_shared_count = new Shared_count;
		}
	}

	~MyShared_ptr() {
		if (m_ptr && 
			!m_shared_count->Reduce_count()) {
			delete m_ptr;
			delete m_shared_count;
		}
	}

	//[copy and assignment]
	MyShared_ptr(const MyShared_ptr& rhs) {
		m_ptr = rhs.m_ptr;
		if (m_ptr) {
			//whether it is real memory otherwise no meanings 
			rhs.m_shared_count->Add_count();
			m_shared_count = rhs.m_shared_count;
		}
	}

	MyShared_ptr& operator= (const MyShared_ptr& rhs) {
		rhs.Swap(*this);
		return *this;
	}

	//[move operations]
	//consider other type assignment;
	template <typename U>
	MyShared_ptr(const MyShared_ptr<U> &other) noexcept {
		m_ptr = other.m_ptr; // pointer type cast
		if (m_ptr) {
			other.m_shared_count->Add_count();
			m_shared_count = other.m_shared_count;
		}
	}

	template <typename U>
	MyShared_ptr(MyShared_ptr&& other) noexcept {
		m_ptr = other.m_ptr;
		if (m_ptr) {
			other.m_ptr = nullptr;

			m_shared_count = other.m_shared_count;
		}
	}

	//type cast
	template <typename U>
	MyShared_ptr(T* ptr, const MyShared_ptr<U>& rhs) noexcept {
		m_ptr = ptr;
		if (m_ptr) {
			rhs.m_shared_count->Add_count();
			m_shared_count = rhs.m_shared_count;
		}
	}

	//utility
	void Swap(MyShared_ptr &rhs) {
		using std::swap;
		swap(rhs.m_ptr, this->m_ptr);
		swap(rhs.m_shared_count, this->m_shared_count);
	}

	T* GetPointer() const noexcept {
		return m_ptr;
	}
	long Use_count() const noexcept {
		if (m_ptr) {
			return m_shared_count->Get_count();
		}
		else
		{
			return 0;
		}
	}
	T* operator-> () const { return m_ptr; }
	T& operator* () const { return *m_ptr; }
	operator bool() const { return m_ptr; }

private:
	T*		m_ptr = nullptr;
	Shared_count *m_shared_count = nullptr;
};

template <typename U>
void Swap(MyShared_ptr<U> &lhs, MyShared_ptr<U> &rhs)
{
	return lhs.Swap(rhs);
}

template <typename T, typename U>
MyShared_ptr<T> Static_SharedPtr_Cast(const MyShared_ptr<U> &other)
{
	T* _ptr = static_cast<T*>(other->GetPointer());
	return MyShared_ptr<T>(_ptr, other);
}
