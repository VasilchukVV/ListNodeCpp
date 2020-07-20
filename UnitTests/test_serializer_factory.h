#pragma once
#include "pch.h"

namespace unit_tests
{
	class test_serializer_factory final
	{
	public:
		static list_node_serializer_ptr create_test_serializer();
	};
}

