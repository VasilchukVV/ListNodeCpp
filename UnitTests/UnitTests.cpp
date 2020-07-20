#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{
	TEST_CLASS(UnitTests)
	{
	public:

		TEST_METHOD(SimpleTestMethod)
		{
			const auto serializer = test_serializer_factory::create_test_serializer();

			const auto generator_ptr = create_list_node_generator(random_mode::random_nulls);
			const unsigned count = 5;
			const auto source_ptr = generator_ptr->generate(count);

			const list_node_helper source_node_helper(source_ptr);

			auto result = source_node_helper.check_consistency(count);
			Assert::IsTrue(SUCCEEDED(result));

			list_node_ptr copy_ptr;
			result = serializer->deep_copy(source_ptr, copy_ptr);
			Assert::IsTrue(SUCCEEDED(result));

			result = source_node_helper.check_identical(copy_ptr);
			Assert::IsTrue(SUCCEEDED(result));

			ostringstream oss;

			result = serializer->serialize(oss, copy_ptr);
			Assert::IsTrue(SUCCEEDED(result));

			oss.flush();
			const string data = oss.str();
			cout << data;

			std::istringstream iss(data);

			list_node_ptr clone_ptr;
			result = serializer->deserialize(iss, clone_ptr);
			Assert::IsTrue(SUCCEEDED(result));

			result = source_node_helper.check_identical(clone_ptr);
			Assert::IsTrue(SUCCEEDED(result));
		}
	};
}
