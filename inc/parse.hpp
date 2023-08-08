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
			struct node;
			struct node{
				// Node constructor
				node(token&& toke): toke_(std::move(toke)) {}

				// Nodes cannot be default constructed
				node() = delete;

				// Collapse a node
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

//	void parse( std::vector<token>::const_iterator first,
//			std::vector<token>::const_iterator last, tree::node& subject );
}

