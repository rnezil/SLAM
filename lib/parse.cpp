#include "parse.hpp"

namespace slam {

// Lexer
lex tokenize( const std::string& input, std::vector<token>& output ){
	// Ensure fresh output container
	//output.clear();

	// Process input
	auto iter = input.begin();
	while( iter != input.end() )
	{
		switch(*iter){
			case ' ':
				// Ignore whitespace
				++iter;
				break;
			case '\n':
				// Ignore newline
				++iter;
				break;
			case '=':
				// End of expression
				iter = input.end();
				break;
			case '(':
				output.emplace_back(token(token::type::open_paren));
				++iter;
				break;
			case ')':
				output.emplace_back(token(token::type::closed_paren));
				++iter;
				break;
			case '^':
				output.emplace_back(token([](double d1, double d2){
							return std::pow(d1, d2);
							}));
				++iter;
				break;
			case '*':
				output.emplace_back(token([](double d1, double d2){
							return d1 * d2;
							}));
				++iter;
				break;
			case '/':
				output.emplace_back(token([](double d1, double d2){
							return d1 / d2;
							}));
				++iter;
				break;
			case '+':
				output.emplace_back(token([](double d1, double d2){
							return d1 + d2;
							}));
				++iter;
				break;
			case '-':
				output.emplace_back(token([](double d1, double d2){
							return d1 - d2;
							}));
				++iter;
				break;
			default:
				if( std::isdigit(*iter) ){
					// Save first digit of number
					std::string val {};
					val.push_back(*iter);

					// Algorithm for extracting number.
					// Loop condition safeguards against
					// dereferencing invalid iterator
					bool done = false;
					while( ++iter != input.end() ){
						switch( *iter ){
							case ' ':
								break;
							case '\n':
								break;
							case '.':
								val.push_back(*iter);
								break;
							default:
								if( std::isdigit(*iter) ){
									val.push_back(*iter);
								}else{
									done = true;
								}
						}

						if( done ){
							break;
						}
					}

					// Ensure number stored in string is valid
					bool already_one_point = false;
					for( auto a : val ){
						if( a == '.' ){
							if( already_one_point ){
								return lex::too_many_decimal_points;
							}else{
								already_one_point = true;
							}
						}
					}

					if( val.back() == '.' )
						return lex::no_digits_after_decimal_point;
					
					// Convert string number to double and store
					// token
					output.emplace_back(token(std::stod(val)));
				}else{
					// Get function keyword stored in a string
					std::string func {};
					func += *iter;
					while( ++iter != input.end() ){
						if( *iter != ' ' || *iter != '\n' ){
							if( std::isalpha(*(iter)) ){
								func += *iter;
							}else{
								break;
							}
						}
					}

					if( func == std::string("sin") ){
							output.emplace_back(token([](double d){
									return std::sin(d);
									}));
							break;
					}else if( func == std::string("cos") ){
							output.emplace_back(token([](double d){
									return std::cos(d);
									}));
							break;
					}else if( func == std::string("tan") ){
							output.emplace_back(token([](double d){
									return std::tan(d);
									}));
							break;
					}else if( func == std::string("arcsin") ){
							output.emplace_back(token([](double d){
									return std::asin(d);
									}));
							break;
					}else if( func == std::string("arccos") ){
							output.emplace_back(token([](double d){
									return std::acos(d);
									}));
							break;
					}else if( func == std::string("arctan") ){
							output.emplace_back(token([](double d){
									return std::atan(d);
									}));
							break;
					}else if( func == std::string("log") ){
							output.emplace_back(token([](double d){
									return std::log10(d);
									}));
							break;
					}else if( func == std::string("lob") ){
							output.emplace_back(token([](double d){
									return std::log2(d);
									}));
							break;
					}else if( func == std::string("ln") ){
							output.emplace_back(token([](double d){
									return std::log(d);
									}));
							break;
					}else if( func == std::string("exp") ){
							output.emplace_back(token([](double d){
									return std::exp(d);
									}));
							break;
					}else if( func == std::string("sqrt") ){
							output.emplace_back(token([](double d){
									return std::sqrt(d);
									}));
							break;
					}else if( func == std::string("cbrt") ){
							output.emplace_back(token([](double d){
									return std::cbrt(d);
									}));
							break;
					}else{
						return lex::unknown_function_name;
					}
				}
		}
	}

	// Naive return
	return lex::success;
}


void parse( std::vector<token>::iterator first,
		std::vector<token>::iterator last, tree::node& massive ){
	// Expression defined by [first, last)
	if( last - first > 1 ){
		// Algorithm can be greatly simplified by acknowledging
		// the fact that a unary operator can only have the
		// lowest priority if it is operation on the entire rest
		// of the expression, such as for an expression like
		// 'sqrt( ... math and numbers ... )'
		auto unary_begin = first;
		++unary_begin;
		if( (*first).info() == token::type::unary_operator &&
				whole_things_wrapped_in_brackets( unary_begin, last ) ){
			// Store operator token in AST node
			massive.toke_ = std::move(*first);

			// De-parenthesize
			while( whole_things_wrapped_in_brackets( unary_begin, last ) ){
				++unary_begin;
				--last;
			}

			// Only one recursion path for unary operators
			massive.left_ = std::make_unique<node>(token(token::type::dummy));
			parse( unary_begin, last, *massive.left_ );
		}else{
			// Containers and variables for metadata collection
			// 	i to keep track of index
			// 	d to keep track of parenthesis depth
			std::vector<tree::metadata> binary_func_data {};
			int i = 0;
			int d = 0;
	
			// Collect some data
			for( auto find = first; find != last; ++find ){
				// Entering parentheses
				if( (*find).info() == token::type::open_paren )
					++d;
	
				// Exiting parentheses
				if( (*find).info() == token::type::closed_paren )
					--d;
	
				// Found a binary operator
				if( (*find).info() == token::type::binary_function ){
					binary_func_data.push_back(tree::metadata(i,d));
				}
	
				// Increment index
				++i;
			}

			// Determine which operator has the lowest priority
			tree::metadata binary_candidate = binary_func_data.front().priority_;
			int current_lowest = binary_candidate.priority_;
			for( auto b : binary_func_data ){
				if( b.priority_ < current_lowest ){
					current_lowest = b.priority_;
				}
			}

			bool no_add_sub = true;
			for( auto b : binary_func_data ){
				if( (*(first + b.index_).info() == token::type::plus ||
					*(first + b.index_).info() == token::type::minus) &&
				 	b.priority_ == current_lowest ){
					binary_candidate = b;
					no_add_sub = false;
					break;
				}
			}

			if( no_add_sub ){
				bool no_mul_div = true;
				for( auto b : binary_func_data ){
					if( (*(first + b.index_).info() ==
							token::type::multiply
							||
						*(first + b.index_).info() ==
							token::type::divide) &&
							b.priority_ == current_lowest )
					{
						binary_candidate = b;
						no_mul_div = false;
						break;
					}
				}

				if( no_mul_div ){
					for( auto b : binary_func_data ){
						if( *(first + b.index_).info() ==
								token::type::exponent &&
								b.priority_ == current_lowest )
						{
							binary_candidate = b;
							break;
						}
					}
				}
			}

			// Procure lowest priority binary operator
			auto split = first;
			for( int k = 0; k < binary_candidate.index_; ++k )
				++split;
			
			// Store operator token in AST node
			massive.toke_ = std::move(*split);

			// Left branch recursion
			auto left_begin = first;
			auto left_end = split;
			while( whole_things_wrapped_in_brackets( left_begin, left_end ) ){
				++left_begin;
				--left_end;
			}
			massive.left_ = std::make_unique<node>(token(token::type::dummy));
			parse( left_begin, left_end, *massive.left_ );

			// Right branch recursion
			auto right_begin = ++split;
			auto right_end = last;
			while( whole_things_wrapped_in_brackets( right_begin, right_end ) ){
				++right_begin;
				--right_end;
			}
			massive.right_ = std::make_unique<node>(token(token::type::dummy));
			parse( right_begin, right_end, *massive.right_ );
		}
	}

	// Recursive node collapse leaves result in root node!
	massive.collapse();
}

bool whole_things_wrapped_in_brackets( std::vector<token>::const_iterator first,
		std::vector<token>::const_iterator last ){
	if( (*first).info() != token::type::open_paren ||
			(*--last).info() != token::type::closed_paren ){
		return false;
	}else{
		int count = 1;
		while( ++first != last && count ){
			if( (*first).info() == token::type::open_paren ){
				++count;
			}else if( (*first).info() == token::type::closed_paren ){
				--count;
			}
		}

		return first == last ? true : false;
	}
}

}

