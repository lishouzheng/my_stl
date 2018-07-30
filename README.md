# STL vector

### 空间分配相关
- Alloc.cpp
- Alloc.h
- Construct.h


### 迭代器的父类
- Iterator.h  

### 萃取类型
- TypeTraits.h  

### 作用于未初始化空间上,批量构造
- UninitializedFunctions.h  

### vector的实现
- Vector.cpp  
- Vector.h  

## C++ STL alloc 类图
[![PCY0L4.md.png](https://s1.ax1x.com/2018/06/25/PCY0L4.md.png)](https://imgchr.com/i/PCY0L4)


### 代码截取:
```
	static T *allocate();//分配未初始化的存储 
	static void deallocate(T *ptr);//解分配存储
	static void construct(T *ptr);//在分配的存储构造对象
	static void destroy(T *ptr);//析构在已分配存储中的对象 

	//5种迭代器类型
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag :public input_iterator_tag {};
	struct bidirectional_iterator_tag :public forward_iterator_tag {};
	struct random_access_iterator_tag :public bidirectional_iterator_tag {};
	
	//特性萃取器
	template<class T>
	struct _type_traits
	struct _type_traits<bool>
	struct _type_traits<char>
	...
	
	
	//要么构造所有元素,要不一个也不构造
	//检查是否为POD类型,如果是,那么采用更高效的初值填写方式;如果不是,则采用更保守安全的方式
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	//构造X的复制品
	void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value)
	//构造n个X的复制品
	inline ForwardIterator uninitialized_fill_n(ForwardIterator first,
		Size n, const T& x)
		
	//insert_aux是其它插入函数的基础
	void vector<T, Alloc>::insert_aux(iterator position,
		InputIterator first,
		InputIterator last,
		std::false_type){
		difference_type locationLeft = endOfStorage_ - finish_; //未使用空间的剩余大小
		difference_type locationNeed = distance(first, last);//要插入元素的数目

		if (locationLeft >= locationNeed){//未使用空间够分配
			if (finish_ - position > locationNeed){//尾部元素数目大于要插入元素的数目
				TinySTL::uninitialized_copy(finish_ - locationNeed, finish_, finish_);//从finish_开始,挪以finish_为结尾的要插入元素数目个元素(因为要插入元素的数目一定小于未使用空间的数目,所以先挪这么多,再挪其它的,这样不会出现溢出的情况)
				std::copy_backward(position, finish_ - locationNeed, finish_);//将没挪的尾部元素挪到后面
				std::copy(first, last, position);//插入操作
			}
			else{//尾部元素数目小于等于要插入元素的数目
				iterator temp = TinySTL::uninitialized_copy(first + (finish_ - position), last, finish_);//首先填充数目为尾部元素数目和插入元素数目之差的元素
				TinySTL::uninitialized_copy(position, finish_, temp);//向后挪尾部元素
				std::copy(first, first + (finish_ - position), position);//插入操作
			}
			finish_ += locationNeed;//重定向尾部迭代器
		}
		else{
			reallocateAndCopy(position, first, last);//分配新的空间,并且复制插入点之前的元素,复制插入的元素,复制插入点之后的元素
		}
	}
	
    //分配新的空间,复制数据,销毁原来空间
	//配合inset_aux函数
	void vector<T, Alloc>::reallocateAndCopy(iterator position, InputIterator first, InputIterator last){
		//新空间的大小
		difference_type newCapacity = getNewCapacity(last - first);
		T *newStart = dataAllocator::allocate(newCapacity);//分配新的空间
		T *newEndOfStorage = newStart + newCapacity;//重定向迭代器
		T *newFinish = TinySTL::uninitialized_copy(begin(), position, newStart);//复制插入元素之前的元素
		newFinish = TinySTL::uninitialized_copy(first, last, newFinish);//复制插入元素
		newFinish = TinySTL::uninitialized_copy(position, end(), newFinish);//复制插入元素之后的元素
		destroyAndDeallocateAll();//销毁原来的vector
		start_ = newStart;//重定向迭代器
		finish_ = newFinish;//同上
		endOfStorage_ = newEndOfStorage;//同上
	}
		
	//==成员函数运算符重载
	bool vector<T, Alloc>::operator == (const vector& v)const{
		if (size() != v.size()){
			return false;
		}
		else{
			auto ptr1 = start_;
			auto ptr2 = v.start_;
			for (; ptr1 != finish_ && ptr2 != v.finish_; ++ptr1, ++ptr2){
				if (*ptr1 != *ptr2)
					return false;
			}
			return true;
		}
	}
	
	
	//删除vector多余的空间,将vector大小变为实际数据的大小
	void vector<T, Alloc>::shrink_to_fit(){
		//dataAllocator::deallocate(finish_, endOfStorage_ - finish_);
		//endOfStorage_ = finish_;
		T* t = (T*)dataAllocator::allocate(size());//重新分配空间
		finish_ = TinySTL::uninitialized_copy(start_, finish_, t);//初始化
		dataAllocator::deallocate(start_, capacity());//销毁原有的空间
		start_ = t;//定向
		endOfStorage_ = finish_;
	}
	
	
		
```

注:参考https://github.com/zouxiaohang/TinySTL.git

