#include "parse.hpp"

namespace slam {

// Lexer
lex tokenize( const std::string& input, std::vector<token>& output ){
	// Ensure fresh output container
	output.clear();

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
					}else if( func == std::string("cos") ){
							output.emplace_back(token([](double d){
									return std::cos(d);
									}));
					}else if( func == std::string("tan") ){
							output.emplace_back(token([](double d){
									return std::tan(d);
									}));
					}else if( func == std::string("arcsin") ){
							output.emplace_back(token([](double d){
									return std::asin(d);
									}));
					}else if( func == std::string("arccos") ){
							output.emplace_back(token([](double d){
									return std::acos(d);
									}));
					}else if( func == std::string("arctan") ){
							output.emplace_back(token([](double d){
									return std::atan(d);
									}));
					}else if( func == std::string("log") ){
							output.emplace_back(token([](double d){
									return std::log10(d);
									}));
					}else if( func == std::string("lob") ){
							output.emplace_back(token([](double d){
									return std::log2(d);
									}));
					}else if( func == std::string("ln") ){
							output.emplace_back(token([](double d){
									return std::log(d);
									}));
					}else if( func == std::string("exp") ){
							output.emplace_back(token([](double d){
									return std::exp(d);
									}));
					}else if( func == std::string("sqrt") ){
							output.emplace_back(token([](double d){
									return std::sqrt(d);
									}));
					}else if( func == std::string("cbrt") ){
							output.emplace_back(token([](double d){
									return std::cbrt(d);
									}));
					}else{
						return lex::unknown_function_name;
					}
				}
		}
	}
}

/*
void parse( std::vector<token>::const_iterator first,
		std::vector<token>::const_iterator last, tree::node& subject ){
}
*/
}

