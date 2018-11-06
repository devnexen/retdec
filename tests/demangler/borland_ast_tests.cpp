/**
 * @file tests/demangler/borland_ast_tests.cpp
 * @brief Tests for the borland ast representation.
 * @copyright (c) 2018 Avast Software, licensed under the MIT license
 */

#include <memory>
#include <gtest/gtest.h>

#include "llvm/Demangle/borland_ast.h"

#define AST_EQ(expected, ast) ast_eq(expected, ast.get())

using namespace ::testing;

namespace retdec {
namespace demangler {
namespace borland {
namespace tests {

class BorlandAstTests: public Test
{
	public:
		BorlandAstTests() = default;

	protected:
		void ast_eq(const std::string &expected, Node *ast) {
			std::stringstream ss;
			ast->print(ss);
			EXPECT_EQ(expected, ss.str());
		}
};

TEST_F(BorlandAstTests,	NestedNameTest)
{
	auto foo = NameNode::create("foo");
	auto bar = NameNode::create("bar");
	auto foo_bar = NestedNameNode::create(
		std::move(foo),
		std::move(bar));
	AST_EQ("foo::bar", foo_bar);

	auto baz = NameNode::create("baz");
	auto foo_bar_baz = NestedNameNode::create(
		std::move(foo_bar),
		std::move(baz));
	AST_EQ("foo::bar::baz", foo_bar_baz);

	EXPECT_EQ(foo_bar_baz->kind(), Node::KNestedName);
}

TEST_F(BorlandAstTests, ParseNameTest)
{
	std::string mangled = "@Project1@mojaproc$";
	std::string expected = "Project1::mojaproc";

	BorlandASTParser parser = BorlandASTParser(mangled);
	auto ast = parser.ast();
	auto ast2 = parser.ast();
	std::string demangled = ast->str();
	ast2->str();

	EXPECT_EQ(demangled, expected);
}

} // tests
} // borland
} // demangler
} // retdec/