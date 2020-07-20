#pragma once
#include "pch.h"

namespace list_node_sample
{
	struct node_info
	{
		int id;
		string data;

		node_info() = default;
		node_info(int id, string data);
	};


	inline node_info::node_info(const int id, string data)
		: id(id)
		, data(std::move(data))
	{
	}
}
