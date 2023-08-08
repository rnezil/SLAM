#include "token.hpp"
#include <vector>
#include <string>
#include <memory>

namespace slam{
	enum class lex{
		success = 0,
		too_many_decimal_points,
		no_digits_after_decimal_point,
		mismatched_brackets,
		adjacent_binary_operators,
		unknown_function_name,
		undefined_function_target,
		missing_binary_operand,
	};

	class tree;
	struct tree::node;
	class tree{
		public:
			struct node{
				// Node constructor
				node( const token& data ): data_(data) {}

				// Nodes cannot be default constructed
				node() = delete;

				// Node data
				token data_;

				// Parent node
				std::unique_ptr<node> parent_;

				// Left subnode
				std::unique_ptr<node> left_;

				// Right subnode
				std::unique_ptr<node> right_;
			};

			// Tree is not default constructible,
			// copy constructible, move constructible,
			// copy assignable, or move assignable
			tree() = delete;
			tree(const tree&) = delete;
			tree(tree&&) = delete;
			tree& operator=(const tree&) = delete;
			tree& operator=(tree&&) = delete;

			// Constructor
			tree( const token& lowest_priority ): base_(lowest_priority) {}
		private:
			// Base of the tree
			node base_;
	};

	lex tokenize( const std::string& input, std::vector<token>& output );
}

