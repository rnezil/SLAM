#include "ast.hpp"

int main(){
	std::string expression {"2+2="};
	slam::tree good_ol;
	std::vector<slam::token> tokens {};
	slam::lex( expression, tokens, false );
	slam::parse( tokens.begin(), tokens.end(), good_ol.stump() );
	std::cout << expression << good_ol.result() << '\n';

	return 0;
}

