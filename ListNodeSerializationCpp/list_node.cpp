#include "pch.h"

namespace list_node_sample
{
	list_node_ptr list_node::create_next(const list_node_ptr& previous)
	{
		auto list_node_ptr = make_shared<list_node>();
		if (previous != nullptr)
		{
			list_node_ptr->previous = previous;
			previous->next = list_node_ptr;
		}
		return list_node_ptr;
	}

	list_node_ptr list_node::create_next(const list_node_ptr& previous, const string& data)
	{
		auto list_node_ptr = create_next(previous);
		list_node_ptr->data = data;
		return list_node_ptr;
	}

	list_node_ptr list_node::create_next(const list_node_ptr& previous, const string& data, const weak_ptr<list_node>& random)
	{
		auto list_node_ptr = create_next(previous, data);
		list_node_ptr->random = random;
		return list_node_ptr;
	}

	list_node_ptr list_node::create_preview(const list_node_ptr& next)
	{
		auto list_node_ptr = make_shared<list_node>();
		list_node_ptr->next = next;
		if (next != nullptr)
			next->previous = list_node_ptr;

		return list_node_ptr;
	}

	list_node_ptr list_node::create_preview(const list_node_ptr& next, const string& data)
	{
		auto list_node_ptr = create_preview(next);
		list_node_ptr->data = data;
		return list_node_ptr;
	}

	list_node_ptr list_node::create_preview(const list_node_ptr& next, const string& data, const weak_ptr<list_node>& random)
	{
		auto list_node_ptr = create_preview(next, data);
		list_node_ptr->random = random;
		return list_node_ptr;
	}
}