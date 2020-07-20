#include "pch.h"

namespace unit_tests
{
	//--------------------------------------------------------------------------------------------------
	// simple serialization (to string stream)
	//--------------------------------------------------------------------------------------------------
	ostream& serialize(ostream& stream, const node_info& node_info)
	{
		stream << node_info.id << ",";
		stream << node_info.data << ";";
		return stream;
	}

	//--------------------------------------------------------------------------------------------------
	// simple deserialization
	//--------------------------------------------------------------------------------------------------
	istream& deserialize(istream& stream, node_info& node_info)
	{
		string delimiter;
		stream >> node_info.id;
		std::getline(stream, delimiter, ',');
		std::getline(stream, node_info.data, ';');
		return stream;
	}

	//--------------------------------------------------------------------------------------------------
	// test_helper implementation
	//--------------------------------------------------------------------------------------------------
	list_node_serializer_ptr test_serializer_factory::create_test_serializer()
	{
		return create_list_node_serializer(serialize, deserialize);
	}
}

