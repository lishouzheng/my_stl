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
```

注:参考https://github.com/zouxiaohang/TinySTL.git
