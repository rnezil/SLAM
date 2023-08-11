#include "ast.hpp"

int main(){
	std::string input {"420sqrt="};
	std::vector<slam::token> tokens {};
	slam::lex( input, tokens, false );
	
	double d = 4761;

	std::cout << "The square root of d = " << d << " is " <<
		(*static_cast<const std::function<double(double)>* const>(tokens.at(1).take()))(d);
	std::cout << '\n';

	return 0;
}

