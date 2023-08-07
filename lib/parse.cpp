#include "parse.hpp"
#include <iostream>
#include <cmath>
#include <cctype>

namespace slam {
// Lexer
lex tokenize( const std::string& input, std::vector<token>& output ){
	// Ensure fresh output container
	output.clear();

	// Initialize some things
	auto iter = input.begin();
	int open_bracket_count = 0;
	int closed_bracket_count = 0;
	bool too_many_decimal_points = false;
	while( iter != input.end() ){
		switch(*iter){
			case ' ':
				// Whitespace
				++iter;
				break;
			case '\n':
				// Newline
				++iter;
				break;
			case '^':
				if( output.empty() )
					return lex::missing_binary_operand;

				if(
						output.back().info() == token::type::exponent ||
						output.back().info() == token::type::multiply ||
						output.back().info() == token::type::divide ||
						output.back().info() == token::type::plus ||
						output.back().info() == token::type::minus
				  ) return lex::adjacent_binary_operators;

				if( output.back().info() == token::type::open_bracket )
					return lex::missing_binary_operand;

				if( output.back().info() == token::type::function )
					return lex::undefined_function_target;

				output.push_back(token(token::type::exponent));
				++iter;
				break;
			case '*':
				if( output.empty() )
					return lex::missing_binary_operand;

				if(
						output.back().info() == token::type::exponent ||
						output.back().info() == token::type::multiply ||
						output.back().info() == token::type::divide ||
						output.back().info() == token::type::plus ||
						output.back().info() == token::type::minus
				  ) return lex::adjacent_binary_operators;

				if( output.back().info() == token::type::open_bracket )
					return lex::missing_binary_operand;

				if( output.back().info() == token::type::function )
					return lex::undefined_function_target;

				output.push_back(token(token::type::multiply));
				++iter;
				break;
			case '/':
				if( output.empty() )
					return lex::missing_binary_operand;

				if(
						output.back().info() == token::type::exponent ||
						output.back().info() == token::type::multiply ||
						output.back().info() == token::type::divide ||
						output.back().info() == token::type::plus ||
						output.back().info() == token::type::minus
				  ) return lex::adjacent_binary_operators;

				if( output.back().info() == token::type::open_bracket )
					return lex::missing_binary_operand;

				if( output.back().info() == token::type::function )
					return lex::undefined_function_target;

				output.push_back(token(token::type::divide));
				++iter;
				break;
			case '+':
				if( output.empty() )
					return lex::missing_binary_operand;

				if(
						output.back().info() == token::type::exponent ||
						output.back().info() == token::type::multiply ||
						output.back().info() == token::type::divide ||
						output.back().info() == token::type::plus ||
						output.back().info() == token::type::minus
				  ) return lex::adjacent_binary_operators;

				if( output.back().info() == token::type::open_bracket )
					return lex::missing_binary_operand;

				if( output.back().info() == token::type::function )
					return lex::undefined_function_target;

				output.push_back(token(token::type::plus));
				++iter;
				break;
			case '-':
				if( output.empty() )
					return lex::missing_binary_operand;

				if(
						output.back().info() == token::type::exponent ||
						output.back().info() == token::type::multiply ||
						output.back().info() == token::type::divide ||
						output.back().info() == token::type::plus ||
						output.back().info() == token::type::minus
				  ) return lex::adjacent_binary_operators;

				if( output.back().info() == token::type::open_bracket )
					return lex::missing_binary_operand;

				if( output.back().info() == token::type::function )
					return lex::undefined_function_target;

				output.push_back(token(token::type::minus));
				++iter;
				break;
			case '(':
				output.push_back(token(token::type::open_bracket));
				++open_bracket_count;
				++iter;
				break;
			case ')':
				if( output.empty() )
					return lex::mismatched_brackets;

				output.push_back(token(token::type::closed_bracket));
				++closed_bracket_count;
				++iter;
				break;
			case '=':
				// End of expression
				iter = input.end();
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
					output.push_back(token(std::stod(val)));
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
							output.push_back(token([](double d){
									return std::sin(d);
									}));
					}else if( func == std::string("cos") ){
							output.push_back(token([](double d){
									return std::cos(d);
									}));
					}else if( func == std::string("tan") ){
							output.push_back(token([](double d){
									return std::tan(d);
									}));
					}else if( func == std::string("arcsin") ){
							output.push_back(token([](double d){
									return std::asin(d);
									}));
					}else if( func == std::string("arccos") ){
							output.push_back(token([](double d){
									return std::acos(d);
									}));
					}else if( func == std::string("arctan") ){
							output.push_back(token([](double d){
									return std::atan(d);
									}));
					}else if( func == std::string("log") ){
							output.push_back(token([](double d){
									return std::log10(d);
									}));
					}else if( func == std::string("lob") ){
							output.push_back(token([](double d){
									return std::log2(d);
									}));
					}else if( func == std::string("ln") ){
							output.push_back(token([](double d){
									return std::log(d);
									}));
					}else if( func == std::string("exp") ){
							output.push_back(token([](double d){
									return std::exp(d);
									}));
					}else if( func == std::string("sqrt") ){
							output.push_back(token([](double d){
									return std::sqrt(d);
									}));
					}else if( func == std::string("cbrt") ){
						output.push_back(token([](double d){
									return std::cbrt(d);
									}));
					}else if( func == std::string("cbrt") ){
						output.push_back(token([](double d){
									return std::cbrt(d);
									}));
					}else if( func == std::string("cbrt") ){
						output.push_back(token([](double d){
									return std::cbrt(d);
									}));
					}else{
						return lex::unknown_function_name;
					}

					// Validity check
					if( iter == input.end() )
						return lex::undefined_function_target;
				}
		}
	}

	// Perform some final validation checks before returning
	for( auto toke = output.begin(); toke != output.end(); ++toke ){
		if(
				(*toke).info() == token::type::function &&
				(*++toke).info() != token::type::open_bracket
				) return lex::undefined_function_target;
		--toke;
	}

	if( open_bracket_count == closed_bracket_count )
		return lex::success;
	else
		return lex::mismatched_brackets;
}

}

