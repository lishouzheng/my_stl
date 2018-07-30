#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <algorithm>
#include <type_traits>

#include "Allocator.h"
#include "Algorithm.h"
#include "Iterator.h"
#include "ReverseIterator.h"
#include "UninitializedFunctions.h"
/*
begin()和end()返回的是迭代器,end()为finish

front()和back()返回的是元素,back()为finish-1的元素

*/
namespace TinySTL{
	//********* vector *************
	template<class T, class Alloc = allocator<T>>
	class vector{
	private://首迭代器:start_,尾迭代器:finish_,可用空间尾迭代器:endOfStorage_
		T *start_;
		T *finish_;
		T *endOfStorage_;

		typedef Alloc dataAllocator;
	public://typedef 提供标准接口
		typedef T									value_type;
		typedef T*							iterator;
		//typedef const iterator					const_iterator;
		typedef const T*							const_iterator;
		typedef reverse_iterator_t<T*>				reverse_iterator;
		typedef reverse_iterator_t<const T*>				const_reverse_iterator;
		typedef iterator							pointer;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef size_t								size_type;
		typedef ptrdiff_t	difference_type;
	public://构造,复制,析构相关,比较操作相关,迭代器相关,与容量相关
		//构造,复制,析构相关
		vector()//构造函数
			:start_(0), finish_(0), endOfStorage_(0){}
		explicit vector(const size_type n);//explicit防止隐性转换
		vector(const size_type n, const value_type& value);//填充式的构造函数
		template<class InputIterator>
		vector(InputIterator first, InputIterator last);//给出迭代器的构造函数

		vector(const vector& v);//复制函数
		vector(vector&& v);//移动函数,会修改V临时对象的指针,所以不能用const
		vector& operator = (const vector& v);//赋值运算符重载
		vector& operator = (vector&& v);//移动赋值运算符重载
		~vector();//析构函数

		////比较操作相关
		bool operator == (const vector& v)const;//相等运算符重载
		bool operator != (const vector& v)const;//不等运算符重载

		//迭代器相关
		iterator begin(){ return (start_); }//返回开头迭代器
		const_iterator begin()const{ return (start_); }//返回开头迭代器的const版本
		const_iterator cbegin()const{ return (start_); }//指向常量的开头迭代器
		iterator end(){ return (finish_); }//返回结尾迭代器(最后一个元素+1)
		const_iterator end()const{ return (finish_); }//返回结尾迭代器的const版本
		const_iterator cend()const{ return (finish_); }//指向常量的开头迭代器
		reverse_iterator rbegin(){ return reverse_iterator(finish_); }//返回开头反向迭代器
		const_reverse_iterator crbegin()const{ return const_reverse_iterator(finish_); }//指向常量的开头反向迭代器
		reverse_iterator rend(){ return reverse_iterator(start_); }//同上
		const_reverse_iterator crend()const{ return const_reverse_iterator(start_); }//同上

		//与容量相关
		difference_type size()const{ return finish_ - start_; }//返回vector的容量
		difference_type capacity()const{ return endOfStorage_ - start_; }//返回vector的可用空间
		bool empty()const{ return start_ == finish_; }//返回vector是否为空

		//reserve只修改capacity大小，不修改size大小，resize既修改capacity大小，也修改size大小。
		//reserve表示容器预留空间，但并不是真正的创建对象，需要通过insert（）或push_back（）等创建对象。
		//resize既分配了空间，也创建了对象。
		void resize(size_type n, value_type val = value_type());//改变vector大小
		void reserve(size_type n);//改变capacity的大小
		void shrink_to_fit();//减少vector大小到满足元素所占存储空间的大小
	public://访问元素相关,修改容器相关的操作
		//访问元素相关
		reference operator[](const difference_type i){ return *(begin() + i); }//[]运算符重载,随机访问
		const_reference operator[](const difference_type i)const{ return *(cbegin() + i); }//[]运算符重载const版本
		reference front(){ return *(begin()); }//返回首元素的引用,访问第一个元素
		reference back(){ return *(end() - 1); }//范围尾元素的引用,访问最后一个元素
		pointer data(){ return start_; }//返回首迭代器指向整个vector

		//修改容器相关的操作
		void clear();//清空容器，销毁容器中的所有对象并使容器的size为0，但不回收容器已有的空间
		void swap(vector& v);//交换两个vector的元素
		void push_back(const value_type& value);//末尾添加元素
		void pop_back();//末尾删除元素
		iterator insert(iterator position, const value_type& val);//任意位置插入元素
		void insert(iterator position, const size_type& n, const value_type& val);//任意位置插入n个元素
		template <class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last);//在position插入first个last(存疑)
		iterator erase(iterator position);//任意位置删除元素
		iterator erase(iterator first, iterator last);//删除两个迭代器之间的元素

		//容器的空间配置器相关
		Alloc get_allocator(){ return dataAllocator; }//返回空间配置器
	private://容器的空间配置器相关
		void destroyAndDeallocateAll();
		void allocateAndFillN(const size_type n, const value_type& value);//配置空间并填满内容
		template<class InputIterator>
		void allocateAndCopy(InputIterator first, InputIterator last);

		template<class InputIterator>
		void vector_aux(InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void vector_aux(Integer n, const value_type& value, std::true_type);
		template<class InputIterator>
		void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
		template<class Integer>
		void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
		template<class InputIterator>
		void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
		void reallocateAndFillN(iterator position, const size_type& n, const value_type& val);
		size_type getNewCapacity(size_type len)const;
	public://友元:相等运算符==和不等运算符!=
		template<class T, class Alloc>
		friend bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);//两个vector作比较
		template<class T, class Alloc>
		friend bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);//同上
	};// end of class vector
}

#include "Vector.impl.h"//具体实现
#endif