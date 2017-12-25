#include <iostream>
#include <mutex>
#include <memory>
#include <stdexcept>

template <typename T> class stack
{
public:
	stack() noexcept;
	~stack() noexcept;
	stack(const stack &); /*strong*/
	stack<T> & operator=(stack<T> const & other); /*strong*/
	size_t count() const noexcept;
	void push(T const &); /*strong*/
	auto pop()->std::shared_ptr<T>; /*strong*/
	void print() const; /*strong*/
	bool isempty() const noexcept;

private:
	T * array_;
	size_t array_size_;
	size_t count_;
	void swap(stack<T>&) noexcept;
	mutable std::mutex mutex_;
};


template<typename T>
stack<T>::stack() noexcept
{
	array_ = nullptr;
}

template<typename T>
stack<T>::~stack() noexcept
{
	count_ = 0;
	array_size_ = 0;
	delete[] array_;
}
template<typename T>
stack<T>::stack(const stack<T>& other)
{
	std::lock_guard<std::mutex> lock(other.mutex_);
	try
	{
		T* tmparray_ = new T[other.array_size_];
		std::copy(other.array_, other.array_ + other.array_size_, tmparray_);
		array_ = tmparray_;
		array_size_ = other.array_size_;
		count_ = other.count_;
	}
	catch (std::exception &err)
	{
		std::cerr << err.what() << std::endl;
		delete[] array_;
	}
	catch (...)
	{
		std::cerr << "error";
		delete[] array_;
	}
}

template <typename T>
bool stack<T>::isempty() const noexcept
{
	std::lock_guard<std::mutex> lock(mutex_);
	return (count_ == 0);
}


template <typename T>
void stack<T>::swap(stack<T> & other) noexcept
{
	std::lock(mutex_, other.mutex_);
	std::swap(other.array_size_, array_size_);
	std::swap(count_, other.count_);
	std::swap(other.array_, array_);
	mutex_.unlock();
	other.mutex_.unlock();
}

template<typename T>
stack<T>& stack<T>::operator= (stack<T> const & other)
{
	std::lock_guard<std::mutex> lock(other.mutex_);
	if (&other != this)
	{
		stack(other).swap(*this);
	}
	return *this;
}

template<typename T>
void stack<T>::print() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (isempty()) std::cout << "Stack is empty" << std::endl;
	else
		for (int i = 0; i < count_; i++)
			std::cout << array_[i] << ' ';
	std::cout << std::endl;
}

template<typename T>
size_t stack<T>::count() const noexcept
{
	std::lock_guard<std::mutex> lock(mutex_);
	return count_;
}

template <typename T>
auto stack<T>::pop() -> std::shared_ptr<T>
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (isempty())
	{
		throw ("Stack is empty!");
	}
	count_--;
	auto top = std::make_shared<T>(*array_);
	return top;
}

template<typename T>
void stack<T>::push(T const & value)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (array_size_ == count_)
	{
		size_t ar_size;
		if (isempty())
			ar_size = 1;
		else
			ar_size = array_size_ * 2;
		try
		{
			T *ptr = new T[ar_size];
			std::copy(array_, array_ + count_, ptr);
			delete[] array_;
			array_ = ptr;
			array_size_ = ar_size;
		}
		catch (std::exception &err)
		{
			std::cerr << err.what() << std::endl;
			delete[] array_;
		}
		catch (...)
		{
			std::cerr << "error";
		}
	}
	array_[count_] = value;
	count_++;
	
}
