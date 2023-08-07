#include "parse.hpp"
#include "print_token.hpp"

int main(){
	std::string input {"12345.11sqrt"};
	std::vector<slam::token> tokens {};
	tokenize( input, tokens );
	for( auto t : tokens ){
		if( t.info() == slam::token::type::function )
			std::cout << "[function] ";
		else
			print_token(t);
	}std::cout << '\n';

	double d = 4761;

	std::cout << "The square root of d = " << d << " is " << tokens.at(1).function()(d);
	std::cout << '\n';

	return 0;
}

