#include <memory>
#include <ostream>
#include <mutex>
#include <stdexcept>
#include "array.h"

using std::make_unique;
using std::fill;
using std::move;
using std::mutex;
using std::lock_guard;
using std::invalid_argument;

Array::Array(int n) {
	if (n <= 0)
		throw invalid_argument("Array size must be positive");
	size_ = n;
	arr = make_unique<int[]>(n), 
	fill(arr.get(), arr.get() + n, 0);
}

Array::Array(Array&& other) noexcept: arr(move(other.arr)),size_(other.size_){
	other.size_ = 0;
}

Array& Array::operator=(Array&& other) noexcept {
	if (this == &other)
		return *this;
	arr = move(other.arr);
	size_ = other.size_;
	other.size_ = 0;
	return *this;
}

int Array::size() const { return size_; }

void Array::print(ostream& out) const
{
	lock_guard<mutex> lk(mtx);
	for (int i = 0; i < size_; ++i)
		out << arr[i] << " ";
	out << "\n";
}

bool Array::tryMark(int index, int markerId)
{
	lock_guard<mutex> lk(mtx);
	if (arr[index] == 0)
	{
		arr[index] = markerId;
		return true;
	}
	return false;
}

void Array::reset(int markerId)
{
	lock_guard<mutex> lk(mtx);
	for (int i = 0; i < size_; ++i)
		if (arr[i] == markerId)
			arr[i] = 0;
}
