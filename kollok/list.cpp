#include "list.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include <ostream>
#include <istream>
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::istream;
using std::invalid_argument;
using std::ostream;

int List::size() const
{
	if (head == nullptr)
		return 0;
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
	else {
		auto current = head;
		while (current->next != nullptr)
		current = current->next;
		current->next = make_shared<Node>(value);
	}
}

shared_ptr<Node> List::getHead() const {return head;}

void List::read(istream& in, int n) {
	if (n < 0) 
		throw invalid_argument("List size must be non-negative");
	for (int i = 0; i < n; ++i) {
		int v;
		if (!(in >> v)) 
			throw invalid_argument("Invalid list element input");
		push_back(v);
	}
}

void List::print(ostream &out) const {
	auto current = head;
	while (current!=nullptr) {
		out << current->value << " ";
		current = current->next;
	}
	out << "\n";
}
