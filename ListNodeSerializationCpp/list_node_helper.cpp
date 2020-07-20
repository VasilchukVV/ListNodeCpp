#include "pch.h"

namespace list_node_sample
{
	//--------------------------------------------------------------------------------------------------
	list_node_helper::list_node_helper(const_list_node_ptr node_ptr)
		: node_ptr_(std::move(node_ptr))
	{
	}

	//--------------------------------------------------------------------------------------------------
	HRESULT list_node_helper::generate_ids(unordered_map<const list_node*, unsigned>& map) const
	{
		if (!map.empty())
			return E_INVALIDARG;

		const list_node* p_node = node_ptr_.get();
		unsigned id = 0;
		while (p_node != nullptr)
		{
			map.insert({ p_node, id });
			p_node = p_node->next.get();
			++id;
		}

		return S_OK;
	}

	//--------------------------------------------------------------------------------------------------
	HRESULT list_node_helper::check_consistency(const unsigned count) const
	{
		unordered_map<const list_node*, unsigned> map;
		const auto result = generate_ids(map);
		if (FAILED(result))
			return result;

		const list_node* p_node = node_ptr_.get();
		unsigned index = 0;
		while (p_node != nullptr)
		{
			const auto random_ptr = p_node->random.lock();
			const list_node* p_random = random_ptr.get();
			if (p_random != nullptr)
			{
				const auto iterator = map.find(p_random);
				if (iterator == map.end())
					return E_FAIL;
			}

			const list_node* p_next = p_node->next.get();
			if (p_next != nullptr)
			{
				if (p_next->previous.get() != p_node)
					return E_FAIL;
			}
			p_node = p_next;
			++index;
		}

		if (index != count)
			return E_FAIL;

		return S_OK;
	}

	//--------------------------------------------------------------------------------------------------
	HRESULT list_node_helper::check_identical(const const_list_node_ptr& clone_ptr) const
	{
		if (node_ptr_ == nullptr && clone_ptr == nullptr)
			return S_OK;

		if (node_ptr_ == nullptr || clone_ptr == nullptr)
			return E_FAIL;

		unordered_map<const list_node*, unsigned> source_ids;
		auto result = generate_ids(source_ids);
		if (FAILED(result))
			return result;

		unordered_map<const list_node*, unsigned> clone_ids;
		result = list_node_helper(clone_ptr).generate_ids(clone_ids);
		if (FAILED(result))
			return result;

		if (source_ids.size() != clone_ids.size())
			return E_FAIL;

		const list_node* p_source = node_ptr_.get();
		const list_node* p_clone = clone_ptr.get();

		while (p_source != nullptr && p_clone != nullptr)
		{
			auto source_id = source_ids[p_source];
			auto clone_id = clone_ids[p_clone];
			if (source_id != clone_id)
				return E_FAIL;

			source_id = source_ids[p_source->previous.get()];
			clone_id = clone_ids[p_clone->previous.get()];
			if (source_id != clone_id)
				return E_FAIL;

			source_id = source_ids[p_source->random.lock().get()];
			clone_id = clone_ids[p_clone->random.lock().get()];
			if (source_id != clone_id)
				return E_FAIL;

			if (p_source->data != p_clone->data)
				return E_FAIL;

			if (p_source->data.data() == p_clone->data.data())
				return E_FAIL;

			p_source = p_source->next.get();
			p_clone = p_clone->next.get();
		}

		if (p_source != nullptr || p_clone != nullptr)
			return E_FAIL;

		return S_OK;
	}
}

