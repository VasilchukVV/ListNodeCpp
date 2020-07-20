#include "stdafx.h"
#include <sstream>

using namespace list_node_sample;

ostream& serialize(ostream& stream, const node_info& node_info);
istream& deserialize(istream& stream, node_info& node_info);

//-------------------------------------------------------------------------------------------------------------
int main()
{
	const auto serializer = create_list_node_serializer(serialize, deserialize);

	list_node_ptr source;
	const auto mode = data_generation_mode::all;
	auto result = generate_list_node(3, mode, source);
	if (FAILED(result))
		return result;

	list_node_ptr clone;
	result = serializer->deep_copy(source, clone);
	if (FAILED(result))
		return result;

	ostringstream oss;

	result = serializer->serialize(oss, clone);
	if (FAILED(result))
		return result;

	oss.flush();
	const string data = oss.str();
	cout << data;

	std::istringstream iss(data);

	list_node_ptr copy;
	result = serializer->deserialize(iss, copy);
	if (FAILED(result))
		return result;

	cout << source;
	cout << clone;

	return 0;
}


//-------------------------------------------------------------------------------------------------------------
ostream& serialize(ostream& stream, const node_info& node_info)
{
	stream << node_info.id << " ";
	stream << node_info.data << " ";
	return stream;
}

//-------------------------------------------------------------------------------------------------------------
istream& deserialize(istream& stream, node_info& node_info)
{
	stream >> node_info.id;
	stream >> node_info.data;
	return stream;
}




