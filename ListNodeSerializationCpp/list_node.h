#pragma once
#include "pch.h"

namespace list_node_sample
{
	class list_node final
	{
	public:
		shared_ptr<list_node> previous;
		shared_ptr<list_node> next;
		weak_ptr<list_node> random;

		string data;

		static shared_ptr<list_node> create_next(const shared_ptr<list_node>& previous);
		static shared_ptr<list_node> create_next(const shared_ptr<list_node>& previous, const string& data);
		static shared_ptr<list_node> create_next(const shared_ptr<list_node>& previous, const string& data, const weak_ptr<list_node>& random);

		static shared_ptr<list_node> create_preview(const shared_ptr<list_node>& next);
		static shared_ptr<list_node> create_preview(const shared_ptr<list_node>& next, const string& data);
		static shared_ptr<list_node> create_preview(const shared_ptr<list_node>& next, const string& data, const weak_ptr<list_node>& random);
	};

	typedef shared_ptr<list_node> list_node_ptr;
	typedef shared_ptr<const list_node> const_list_node_ptr;
}