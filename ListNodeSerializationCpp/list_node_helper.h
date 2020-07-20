#pragma once
#include "pch.h"

namespace list_node_sample
{
	class list_node_helper final
	{
		const const_list_node_ptr node_ptr_;

	public:
		explicit list_node_helper(const_list_node_ptr);
		~list_node_helper() = default;

		list_node_helper(const list_node_helper&) = default;
		list_node_helper(list_node_helper&&) = delete;
		list_node_helper& operator=(const list_node_helper&) = delete;
		list_node_helper& operator=(list_node_helper&&) = delete;

		HRESULT generate_ids(unordered_map<const list_node*, unsigned>&) const;
		HRESULT check_consistency(unsigned count) const;
		HRESULT check_identical(const const_list_node_ptr&) const;
	};
}
