#pragma once
#include "pch.h"

namespace list_node_sample
{
	__interface i_list_node_generator
	{
		list_node_ptr generate(unsigned count) const;
	};

	typedef shared_ptr<i_list_node_generator> list_node_generator_ptr;


	enum class random_mode { only_nulls, random_nulls, no_nulls };

	list_node_generator_ptr create_list_node_generator(random_mode random_mode);
}