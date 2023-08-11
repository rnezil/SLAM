#include "ast.hpp"

namespace slam {

lexer lex( const std::string& input, std::vector<token>& output, bool rad ){
	// Process input
	auto iter = input.begin();
	while( iter != input.end() )
	{
		switch(*iter){
			case 'Q':
				// Quit
				return lexer::quit;
			case '=':
				// End fo expression
				iter = input.end();
				break;
			case ' ':
				// Ignore whitespace
				++iter;
				break;
			case '\n':
				// Ignore newline
				++iter;
				break;
			case '.':
				// Decimals must inputted as, for
				// example, 0.1 instead of just .1
				return lexer::invalid_number;
			case '(':
				output.emplace_back(token(token::type::open_paren));
				++iter;
				break;
			case ')':
				output.emplace_back(token(token::type::closed_paren));
				++iter;
				break;
			case 'E':
				output.emplace_back(token([](double d1, double d2){
							return d1 * std::pow(10, d2);
							}, 'E'));
				++iter;
				break;
			case '^':
				output.emplace_back(token([](double d1, double d2){
							return std::pow(d1, d2);
							}, '^'));
				++iter;
				break;
			case '*':
				output.emplace_back(token([](double d1, double d2){
							return d1 * d2;
							}, '*'));
				++iter;
				break;
			case '/':
				output.emplace_back(token([](double d1, double d2){
							return d1 / d2;
							}, '/'));
				++iter;
				break;
			case '+':
				output.emplace_back(token([](double d1, double d2){
							return d1 + d2;
							}, '+'));
				++iter;
				break;
			case '-':
				output.emplace_back(token([](double d1, double d2){
							return d1 - d2;
							}, '-'));
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
								return lexer::invalid_number;
							}else{
								already_one_point = true;
							}
						}
					}

					if( val.back() == '.' )
						return lexer::invalid_number;
					
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
							output.emplace_back(token([rad](double d){
									if(rad){
										return std::sin(d);
									}else{
										return std::sin((d*double(180))/std::numbers::pi);
									}
									}));
							break;
					}else if( func == std::string("cos") ){
							output.emplace_back(token([rad](double d){
									if(rad){
										return std::cos(d);
									}else{
										return std::cos((d*double(180))/std::numbers::pi);
									}
									}));
							break;
					}else if( func == std::string("tan") ){
							output.emplace_back(token([rad](double d){
									if(rad){
										return std::tan(d);
									}else{
										return std::tan((d*double(180))/std::numbers::pi);
									}
									}));
							break;
					}else if( func == std::string("arcsin") ){
							output.emplace_back(token([rad](double d){
									if(rad){
										return std::asin(d);
									}else{
										return (std::asin(d)*double(180))/std::numbers::pi;
									}
									}, 's'));
							break;
					}else if( func == std::string("arccos") ){
							output.emplace_back(token([rad](double d){
									if(rad){
										return std::acos(d);
									}else{
										return (std::acos(d)*double(180))/std::numbers::pi;
									}
									}, 'c'));
							break;
					}else if( func == std::string("arctan") ){
							output.emplace_back(token([rad](double d){
									if(rad){
										return std::atan(d);
									}else{
										return (std::atan(d)*double(180))/std::numbers::pi;
									}
									}));
							break;
					}else if( func == std::string("log") ){
							output.emplace_back(token([](double d){
									return std::log10(d);
									}, 'l'));
							break;
					}else if( func == std::string("lob") ){
							output.emplace_back(token([](double d){
									return std::log2(d);
									}, 'l'));
							break;
					}else if( func == std::string("ln") ){
							output.emplace_back(token([](double d){
									return std::log(d);
									}, 'l'));
							break;
					}else if( func == std::string("exp") ){
							output.emplace_back(token([](double d){
									return std::exp(d);
									}));
							break;
					}else if( func == std::string("sqrt") ){
							output.emplace_back(token([](double d){
									return std::sqrt(d);
									}, 'r'));
							break;
					}else if( func == std::string("cbrt") ){
							output.emplace_back(token([](double d){
									return std::cbrt(d);
									}));
							break;
					}else if( func == std::string("fac") ){
							output.emplace_back(token([](double d){
									if(d){
										unsigned w;
										for( w = d-1; w; --w )
											d *= w;
										return d;
									}else{
										return ++d;
									}	
									}, 'f'));
							break;
					}else if( func == std::string("abs") ){
							output.emplace_back(token([](double d){
									return d < 0 ? -d : d;
									}));
							break;
					}else if( func == std::string("neg") ){
							output.emplace_back(token([](double d){
									return -d;
									}));
							break;
					}else if( func == std::string("pi") ){
							output.emplace_back(token(std::numbers::pi));
							break;
					}else{
						return lexer::unknown_function;
					}
				}
		}
	}

	// Perform some validity checks.

	// Empty expression
	if( !output.size() ){
		return lexer::blank_expression;
	}

	// Starts with binary operator
	if( output.front().info() == token::type::binary_function ){
		return lexer::missing_binary_operand;
	}

	// Ends with binary operator
	if( output.back().info() == token::type::binary_function ){
		return lexer::missing_binary_operand;
	}

	// Ends with unary operator
	if( output.back().info() == token::type::unary_function ){
		return lexer::missing_unary_operand;
	}

	int paren = 0;
	for( unsigned i = 0; i < output.size(); ++i ){
		// Ensured every closed parenthesis is
		// accompanied by an open parenthesis
		if( output.at(i).info() == token::type::open_paren ) ++paren;
		if( output.at(i).info() == token::type::closed_paren ) --paren;
		if( paren < 0 ){
			return lexer::unpaired_parentheses;
		}

		// Don't perform these checks on first token
		if( i ){
			// Implicit multiplication
			if( (output.at(i).info() == token::type::unary_function
					|| output.at(i).info() == token::type::open_paren)
					&&
					(output.at(i-1).info() == token::type::number
					 || output.at(i-1).info() == token::type::closed_paren)){
				return lexer::missing_binary_operator;
			}
		}

		// Don't perform these checks on last token
		if( i + 1 < output.size() ){
			// Empty parentheses
			if( output.at(i).info() == token::type::open_paren &&
					output.at(i+1).info() == token::type::closed_paren ){
				return lexer::empty_parentheses;
			}

			// Two numbers beside eachother
			if( output.at(i).info() == token::type::number &&
					output.at(i+1).info() == token::type::number ){
				return lexer::missing_binary_operator;
			}

			// Unary operator lacks clearly defined target
			if( output.at(i).info() == token::type::unary_function &&
					output.at(i+1).info() != token::type::open_paren ){
				return lexer::missing_unary_operand;
			}

			// Don't perform these checks on first or last token
			if( i ){
				// Binary operator missing operand(s)
				if( output.at(i).info() == token::type::binary_function &&
						( output.at(i-1).info() == token::type::binary_function
						|| output.at(i-1).info() == token::type::open_paren
						|| output.at(i+1).info() == token::type::binary_function
						|| output.at(i+1).info() == token::type::closed_paren )
					){
					return lexer::missing_binary_operand;
				}
			}
		}
	}

	// More open parentheses than closed parentheses
	if( paren ){
		return lexer::unpaired_parentheses;
	}

	// End of lexer validation process
	return lexer::success;
}


void parse( std::vector<token>::iterator first,
		std::vector<token>::iterator last, tree::node& massive ){
	// Remove any redundant parentheses
	while( whole_things_wrapped_in_brackets( first, last ) ){
		++first;
		--last;
	}

	// Expression defined by [first, last)
	if( last - first > 1 ){
		// Algorithm can be greatly simplified by acknowledging
		// the fact that a unary operator can only have the
		// lowest priority if it is operation on the entire rest
		// of the expression, such as for an expression like
		// 'sqrt( ... math and numbers ... )'
		auto unary_begin = first;
		++unary_begin;
		if( (*first).info() == token::type::unary_function &&
				whole_things_wrapped_in_brackets( unary_begin, last ) ){
			// Store operator token in AST node
			massive.toke_ = std::move(*first);

			// Only one recursion path for unary operators
			massive.left_ = std::make_unique<tree::node>(token(token::type::dummy));
			parse( unary_begin, last, *massive.left_ );
		}else{
			// Containers and variables for metadata collection
			// 	i to keep track of index
			// 	d to keep track of parenthesis depth
			std::vector<metadata> binary_func_data {};
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
					binary_func_data.push_back(metadata(i,d));
				}
	
				// Increment index
				++i;
			}

			// Determine what the lowest priority is relative
			// to parentheses
			int current_lowest = binary_func_data.front().priority_;
			std::vector<metadata> candidates {};
			for( auto b : binary_func_data ){
				if( b.priority_ == current_lowest ){
					// Add to list of candidates
					candidates.push_back(b);
				}else if( b.priority_ < current_lowest ){
					// Trump all other candidates
					current_lowest = b.priority_;
					candidates.clear();
					candidates.push_back(b);
				}
			}

			/*
			 * Binary Operator Precedence
			 *
			 * ^	2^2*7 = 28 not 16384
			 * /	18/2*9 = 81 not 1
			 * *	
			 * -	15-5+10 = 20 not 0
			 * +
			 *
			 */

			auto split = first;
			if( candidates.size() == 1 ){
				// One operator trumps all others
				split = split + candidates.front().index_;
			}else if( candidates.size() ){
				// Multiple operators share the same
				// priority so make choice based on
				// operator precedence
				bool choice = false;
				for( auto c = candidates.rbegin(); c != candidates.rend(); ++c ){
					if( (*(split + (*c).index_)).what() == '+' ){
						split = split + (*c).index_;
						choice = true;
						break;
					}
				}

				if( !choice ){
					for( auto c = candidates.rbegin(); c != candidates.rend(); ++c ){
						if( (*(split + (*c).index_)).what() == '-' ){
							split = split + (*c).index_;
							choice = true;
							break;
						}
					}
				}

				if( !choice ){
					for( auto c = candidates.rbegin(); c != candidates.rend(); ++c ){
						if( (*(split + (*c).index_)).what() == '*' ){
							split = split + (*c).index_;
							choice = true;
							break;
						}
					}
				}

				if( !choice ){
					for( auto c = candidates.rbegin(); c != candidates.rend(); ++c ){
						if( (*(split + (*c).index_)).what() == '/' ){
							split = split + (*c).index_;
							choice = true;
							break;
						}
					}
				}

				if( !choice ){
					for( auto c = candidates.rbegin(); c != candidates.rend(); ++c ){
						if( (*(split + (*c).index_)).what() == '^' ){
							split = split + (*c).index_;
							choice = true;
							break;
						}
					}
				}
				
				if( !choice ){
					for( auto c = candidates.rbegin(); c != candidates.rend(); ++c ){
						if( (*(split + (*c).index_)).what() == 'E' ){
							split = split + (*c).index_;
							choice = true;
							break;
						}
					}
				}

				if( !choice ){
					// Parse algorithm failed to split expression
					throw lexer::parse_failure;
				}
			}else{
				// Parse algorithm failed to split expression
				throw lexer::parse_failure;
			}

			// Store operator token in AST node
			massive.toke_ = std::move(*split);

			// Left branch recursion
			auto left_begin = first;
			auto left_end = split;
			massive.left_ = std::make_unique<tree::node>(token(token::type::dummy));
			parse( left_begin, left_end, *massive.left_ );

			// Right branch recursion
			auto right_begin = ++split;
			auto right_end = last;
			massive.right_ = std::make_unique<tree::node>(token(token::type::dummy));
			parse( right_begin, right_end, *massive.right_ );
		}
	}else if( last - first == 1 ){
		// Reaching this point in the code
		// indicates that the parse function
		// was called on a number, therefore
		// set the node token to that number
		// token
		massive.toke_ = std::move(*first);
	}else{
		// Parse called with invalid iterators
		throw lexer::parse_failure;
	}

	// Recursive node collapse to compute result
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

