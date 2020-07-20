#pragma once
#include "pch.h"
#include <functional>

namespace list_node_sample
{
	__interface i_list_node_serializer
	{
		HRESULT serialize(ostream&, const const_list_node_ptr& source_ptr);
		HRESULT deserialize(istream&, list_node_ptr& result_ptr);
		HRESULT deep_copy(const const_list_node_ptr& source_ptr, list_node_ptr& result_ptr);
	};

	typedef shared_ptr<i_list_node_serializer> list_node_serializer_ptr;
	typedef function<ostream& (ostream&, const node_info&)> node_info_serialize;
	typedef function<istream& (istream&, node_info&)> node_info_deserialize;

	list_node_serializer_ptr create_list_node_serializer(
		const node_info_serialize& serialize,
		const node_info_deserialize& deserialize);
}