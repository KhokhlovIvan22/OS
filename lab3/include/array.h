#pragma once
#include <mutex>
#include <memory>
#include <ostream>

using std::mutex;
using std::unique_ptr;
using std::ostream;

class Array {
private:
	unique_ptr<int[]> arr;
	int size_;
	mutable mutex mtx;
public:
	explicit Array(int n);
	~Array() = default;
	Array(const Array&) = delete;
	Array& operator=(const Array&) = delete;
	Array(Array&& other) noexcept;
	Array& operator=(Array&& other) noexcept;
	int size() const;
	void print(ostream& out) const;
	bool tryMark(int index, int markerId);
	void reset(int markerId);
};
