#include "token.hpp"
#include <iostream>

namespace slam {

void print_token(const token& toke){
	if( toke.info() == token::type::exponent ){
		std::cout << '^';
	}
	if( toke.info() == token::type::multiply ){
		std::cout << '*';
	}
	if( toke.info() == token::type::divide ){
		std::cout << '/';
	}
	if( toke.info() == token::type::plus ){
		std::cout << '+';
	}
	if( toke.info() == token::type::minus ){
		std::cout << '-';
	}
	if( toke.info() == token::type::number ){
		std::cout << toke.value();
	}
}

}

