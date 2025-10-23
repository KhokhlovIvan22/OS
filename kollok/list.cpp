#include "list.h"
#include <memory>
using std::shared_ptr;
using std::make_shared;

int List::size()
{
	int size_ = 0;
	auto current = head;
	while (current != nullptr)
	{
		size_++;
		current = current->next;
	}
	return size_;
}

void List::push_back (int value)
{
	if (head == nullptr)
		head = make_shared<Node>(value);
	else
	{
		auto current = head;
		while (current->next != nullptr)
		current = current->next;
		current->next = make_shared<Node>(value);
	}
}

shared_ptr<Node> List::head() {return head;}

List List::reverse() {
	vector<int> values;
	auto cur = head_;
	while (cur) {
		values.push_back(cur->value);
		cur = cur->next;
	}
	List newList;
	for (auto it = values.rbegin(); it != values.rend(); ++it)
		newList.push_back(*it);
	return newList;
}