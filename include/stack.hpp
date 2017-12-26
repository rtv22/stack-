#include <mutex>
#include <memory>
#include <iostream>

template <typename T>
class stack
{
public:
	stack();
	size_t count() const;
	void push(T const &);
	stack(const stack<T> &);
	auto pop() -> std::shared_ptr<T>;
	void swap(stack<T>&);
private:
	T * array_;
	mutable std::mutex mutex_;
	size_t array_size_;
	size_t count_;
};

template <typename T> 
stack<T>::stack() : count_(0), array_size_(0), array_{ nullptr }
{}

template <typename T> 
stack<T>::stack(const stack<T>& copy)
{
	std::lock_guard<std::mutex> lock(copy.mutex_);
	T *tmp = new T[copy.array_size_];
	count_ = copy.count_;
	array_size_ = copy.array_size_;
	array_ = tmp;
	try
	{
		std::copy(copy.array_, copy.array_ + count_, array_);
	}
	catch(...){
		delete[] array_;
	}
}

template <typename T>
auto stack<T>::pop() -> std::shared_ptr<T>
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (!empty())
		--count_;
	auto top = std::make_shared<T>(*array_);
	return top;
}

template<class T>
size_t stack<T>::count() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return count_;
}

template<typename T> 
void stack<T>::swap(stack& x) 
{
	x.mutex_.lock();
	std::swap(x.array_size_, array_size_);
	std::swap(count_, x.count_);
	std::swap(x.array_, array_);
	x.mutex_.unlock();
}

template <typename T>
void stack<T>::push(T const &value)
{
	mutex_.lock();
	if (array_size_ == count_) 
	{
		array_size_ *= 2;
		stack<T> temp(*this);
		swap(temp);
	}
	array_[count_] = value;
	++count_;
	mutex_.unlock();
}
