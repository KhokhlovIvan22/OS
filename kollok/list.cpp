#include "list.h"
#include <memory>
using std::unique_ptr;

int size()
{
	int size = 0;
	auto current = head;
	while (current != nullptr)
	{
		size++;
		current = current->next;
	}
}