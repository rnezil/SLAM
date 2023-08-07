#include "parse.hpp"
#include "print_token.hpp"

int main(){
	std::vector<slam::token> tokens {};
	std::string input {};
	while(1){
		input.clear();
		std::getline(std::cin, input);
		if( tokenize( input, tokens ) == slam::lex::success ){
			for( auto x : tokens ){
				slam::print_token(x);
				}
		}
		std::cout << '\n';
	}
	return 0;
}

