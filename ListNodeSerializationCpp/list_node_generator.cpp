#include "pch.h"
#include "i_list_node_generator.h"
#include <vector>

namespace list_node_sample
{
	//--------------------------------------------------------------------------------------------------
	// simple_generator: empty data & null random node
	//--------------------------------------------------------------------------------------------------
	class simple_generator final : public i_list_node_generator
	{
	public:
		simple_generator() = default;
		virtual ~simple_generator() = default;

		simple_generator(const simple_generator&) = delete;
		simple_generator& operator=(const simple_generator&) = delete;
		simple_generator(simple_generator&&) = delete;
		simple_generator& operator=(simple_generator&&) = delete;

	public:
		list_node_ptr generate(const unsigned count) const override
		{
			list_node_ptr node;
			for (unsigned index = 0; index < count; index++)
				node = list_node::create_preview(node);

			return  node;
		}
	};

	//--------------------------------------------------------------------------------------------------
	// rand_generator: random data & random node 
	//--------------------------------------------------------------------------------------------------
	class rand_generator final : public i_list_node_generator
	{
		const bool no_nulls;

	public:
		explicit rand_generator(const bool no_nulls)
			: no_nulls(no_nulls)
		{
		}

		virtual ~rand_generator() = default;

		rand_generator(const rand_generator&) = delete;
		rand_generator(rand_generator&&) = delete;
		rand_generator& operator=(const rand_generator&) = delete;
		rand_generator& operator=(rand_generator&&) = delete;

		list_node_ptr generate(const unsigned count) const override
		{
			vector<list_node_ptr> nodes;

			list_node_ptr node;
			for (unsigned index = 0; index < count; index++)
			{
				string data;
				if (no_nulls || rand() % 2 == 0)
					data = to_string(index);

				node = list_node::create_preview(node, data);
				nodes.push_back(node);
			}

			auto iterator = nodes.begin();
			while (iterator != nodes.end())
			{
				if (no_nulls || rand() % 2 == 0)
				{
					const list_node_ptr& node_ptr = *iterator;
					const unsigned index = rand() % nodes.size();
					node_ptr->random = nodes[index];
				}
				++iterator;
			}

			return node;
		}
	};


	//--------------------------------------------------------------------------------------------------
	// i_list_node_generator factory
	//--------------------------------------------------------------------------------------------------
	list_node_generator_ptr create_list_node_generator(random_mode random_mode)
	{
		if (random_mode == random_mode::only_nulls)
			return make_shared<simple_generator>();

		return make_shared<rand_generator>(random_mode == random_mode::no_nulls);
	}
}
