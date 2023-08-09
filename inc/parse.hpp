#include "token.hpp"
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <cmath>
#include <cctype>

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

	class tree{
		public:
			struct metadata{
				const int index_;
				int priority_;
				metadata(int i, int p): index_(i), priority_(p) {}
			};

			struct node;
			struct node{
				// Node constructor
				node(token&& toke): toke_(std::move(toke)),
			       		left_(nullptr), right_(nullptr)	{}

				// Nodes cannot be default constructed
				// or copied or moved
				node() = delete;
				node(const node&) = delete;
				node& operator=(const node&) = delete;
				node(node&&) = delete;
				node& operator=(node&&) = delete;

				// Collapse a node by computing operation
				// defined by node and its subnode(s) then
				void collapse()
{
if( toke_.info() == token::type::unary_function )
{
toke_ = token(
		(*static_cast<const std::function<double(double)>* const>(toke_.take()))(
			(*static_cast<const double* const>((left_->toke_).take())))
		);
left_ = nullptr;
right_ = nullptr;
}
else if( toke_.info() == token::type::binary_function )
{
toke_ = token(
		(*static_cast<const std::function<double(double,double)>* const>(toke_.take()))(
			(*static_cast<const double* const>((left_->toke_).take())),
			(*static_cast<const double* const>((right_->toke_).take())))
		);
left_ = nullptr;
right_ = nullptr;
}
				}
						
				// Token of the node
				token toke_;

				// Left subnode
				std::unique_ptr<node> left_;

				// Right subnode
				std::unique_ptr<node> right_;
			};

			// Constructor
			tree(): base_(token(token::type::dummy)) {}

			// Access root node
			node& stump() { return base_; }
		private:
			// Root node
			node base_;
	};

	lex tokenize( const std::string& input, std::vector<token>& output );

	void parse( std::vector<token>::iterator first,
			std::vector<token>::iterator last, tree::node& massive );
}

