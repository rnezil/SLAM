#include "token.hpp"
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <cctype>
#include <numbers>

namespace slam{
	enum class lexer{
		success = 0,
		invalid_number,
		unknown_function,
		unpaired_parentheses,
		empty_parentheses,
		blank_expression,
		missing_binary_operand,
		missing_binary_operator,
		missing_unary_operand,
		bad_division,
		bad_square_root,
		bad_factorial,
		bad_sine,
		bad_cosine,
		bad_logarithm,
		parse_failure,
		quit,
	};
			
	struct metadata{
		int index_;
		int priority_;
		metadata(int i, int p): index_(i), priority_(p) {}
	};


	class tree{
		public:
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
				void collapse(){
					if( toke_.info() == token::type::unary_function )
					{
						// Check for square rooting negative number
						if( toke_.what() == 'r' &&
								(*static_cast<const double* const>((left_->toke_).take())) < double(0) )
						{
							throw lexer::bad_square_root;
						}
						else if( toke_.what() == 'f' &&
								(std::floor((*static_cast<const double* const>((left_->toke_).take()))) != (*static_cast<const double* const>((left_->toke_).take())) || (*static_cast<const double* const>((left_->toke_).take())) < 0) )
						{
							// Check for factorializing non-whole
							// or negative number
							throw lexer::bad_factorial;
						}
						else if( toke_.what() == 's' &&
								((*static_cast<const double* const>((left_->toke_).take())) < double(-1) || (*static_cast<const double* const>((left_->toke_).take())) > double(1)) )
						{
							// Check for out-of-range arcsine
							// argument
							throw lexer::bad_sine;
						}
						else if( toke_.what() == 'c' &&
								((*static_cast<const double* const>((left_->toke_).take())) < double(-1) || (*static_cast<const double* const>((left_->toke_).take())) > double(1)) )
						{
							// Check for out-of-range arccosine
							// argument
							throw lexer::bad_cosine;
						}
						else if( toke_.what() == 'l' &&
								(*static_cast<const double* const>((left_->toke_).take())) <= double(0) )
						{
							// Check for negative logarithm
							// argument
							throw lexer::bad_logarithm;
						}

						toke_ = token(
								(*static_cast<const std::function<double(double)>* const>(toke_.take()))(
									(*static_cast<const double* const>((left_->toke_).take()))));
						left_ = nullptr;
						right_ = nullptr;
					}
					else if( toke_.info() == token::type::binary_function )
					{
						// Check for division by zero
						if( toke_.what() == '/' &&
								(*static_cast<const double* const>((right_->toke_).take())) == double(0) )
						{
							throw lexer::bad_division;
						}

						toke_ = token(
								(*static_cast<const std::function<double(double,double)>* const>(toke_.take()))(
									(*static_cast<const double* const>((left_->toke_).take())),
									(*static_cast<const double* const>((right_->toke_).take()))));
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

			// Get result of computation
			double result() const { return double(*static_cast<const double* const>((base_.toke_).take())); }

			// Reset the tree
			bool reset() {
				base_.toke_ = token(token::type::dummy);
				base_.left_ = nullptr;
				base_.right_ = nullptr;
				return base_.toke_.info() == token::type::dummy &&
					base_.left_ == nullptr &&
					base_.right_ == nullptr;
			}
		private:
			// Root node
			node base_;
	};

	lexer lex( const std::string& input, std::vector<token>& output, bool rad );

	void parse( std::vector<token>::iterator first,
			std::vector<token>::iterator last, tree::node& massive );

	bool whole_things_wrapped_in_brackets( std::vector<token>::const_iterator first,
			std::vector<token>::const_iterator last );
}

