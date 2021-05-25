#pragma once

#include <stdint.h>
#include <memory>
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
