#include "pch.h"
#include "i_list_node_serializer.h"


namespace list_node_sample
{
	class list_node_serializer final : public i_list_node_serializer
	{
	public:
		list_node_serializer(node_info_serialize serialize, node_info_deserialize deserialize);
		virtual ~list_node_serializer() = default;

		list_node_serializer(const list_node_serializer&) = delete;
		list_node_serializer& operator=(const list_node_serializer&) = delete;
		list_node_serializer(list_node_serializer&&) = delete;
		list_node_serializer& operator=(list_node_serializer&&) = delete;

		HRESULT serialize(ostream&, const const_list_node_ptr& source_ptr) override;
		HRESULT deserialize(istream&, list_node_ptr& result_ptr) override;
		HRESULT deep_copy(const const_list_node_ptr& source_ptr, list_node_ptr& result_ptr) override;

	private:
		const node_info_serialize serializer_;
		const node_info_deserialize deserializer_;
	};


	//------------------------------------------------------------------------------------------------------
	// list_node_serializer factory
	//------------------------------------------------------------------------------------------------------
	list_node_serializer_ptr create_list_node_serializer(
		const node_info_serialize& serialize,
		const node_info_deserialize& deserialize)
	{
		return make_shared<list_node_serializer>(serialize, deserialize);
	}

	//------------------------------------------------------------------------------------------------------
	// list_node_serializer implementation
	//------------------------------------------------------------------------------------------------------
	list_node_serializer::list_node_serializer(node_info_serialize serialize, node_info_deserialize deserialize)
		: serializer_(std::move(serialize))
		, deserializer_(std::move(deserialize))
	{
	}

	//------------------------------------------------------------------------------------------------------
	HRESULT get_id(const const_list_node_ptr& node_ptr, const unordered_map<const list_node*, unsigned>& map, int& id)
	{
		const list_node* p_random = node_ptr.get();
		if (p_random != nullptr)
		{
			const auto iterator = map.find(p_random);
			if (iterator != map.end())
			{
				id = iterator->second;
				return S_OK;
			}
		}
		return E_FAIL;
	}


	//------------------------------------------------------------------------------------------------------
	HRESULT list_node_serializer::serialize(ostream& stream, const const_list_node_ptr& source_ptr)
	{
		unordered_map<const list_node*, unsigned> map;
		auto result = list_node_helper(source_ptr).generate_ids(map);
		if (FAILED(result))
			return result;

		const list_node* p_node = source_ptr.get();
		while (p_node != nullptr)
		{
			node_info info(-1, p_node->data);
			const auto random_ptr = p_node->random.lock();
			if (random_ptr != nullptr)
			{
				result = get_id(random_ptr, map, info.id);
				if (FAILED(result))
					return result;
			}

			serializer_(stream, info);

			p_node = p_node->next.get();
		}
		return S_OK;
	}

	//------------------------------------------------------------------------------------------------------
	HRESULT list_node_serializer::deserialize(istream& stream, list_node_ptr& result_ptr)
	{
		vector<list_node_ptr> nodes;

		list_node_ptr list_node;
		unsigned index = 0;

		while (!stream.eof())
		{
			node_info info;
			deserializer_(stream, info);
			if (stream.fail())
				break;

			if (index >= nodes.size())
			{
				list_node = list_node::create_next(list_node, info.data);
				nodes.push_back(list_node);
			}
			else
				nodes[index]->data = info.data;

			if (info.id >= 0)
			{
				const auto random_id = static_cast<unsigned>(info.id);
				while (random_id >= nodes.size())
				{
					list_node = list_node::create_next(list_node);
					nodes.push_back(list_node);
				}
				nodes[index]->random = nodes[random_id];
			}

			index++;
		}

		if (nodes.empty())
			return E_FAIL;

		result_ptr = nodes.front();
		return S_OK;
	}

	//------------------------------------------------------------------------------------------------------
	HRESULT list_node_serializer::deep_copy(const const_list_node_ptr& source_ptr, list_node_ptr& result_ptr)
	{
		if (source_ptr == nullptr)
			return E_INVALIDARG;

		unordered_map<const list_node*, list_node_ptr> nodes;

		const list_node* p_source = source_ptr.get();
		list_node_ptr copy_node_ptr;
		while (p_source != nullptr)
		{
			string data;
			const string& source_data = p_source->data;
			if (source_data.length() > 0)
				data = source_data.substr(0, source_data.length());

			copy_node_ptr = list_node::create_next(copy_node_ptr, data, p_source->random);
			nodes.insert({ p_source, copy_node_ptr });
			p_source = p_source->next.get();
		}

		list_node_ptr node_ptr;
		while (copy_node_ptr != nullptr)
		{
			copy_node_ptr->next = node_ptr;
			node_ptr = copy_node_ptr;

			auto random_ptr = copy_node_ptr->random.lock();
			const list_node* const p_random = random_ptr.get();
			if (p_random != nullptr)
				copy_node_ptr->random = nodes[p_random];

			copy_node_ptr = copy_node_ptr->previous;
		}

		result_ptr = node_ptr;

		return S_OK;
	}
}
