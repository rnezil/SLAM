#include "ast.hpp"

int main(int argc, char** argv){
	std::string input {};
	std::vector<slam::token> expression {};

	// Initialize the calculator
	slam::tree calculator;
	bool ON = true;

	while( ON ){
		// Get a line from stdin
		std::getline(std::cin, input, '=');

		// Tokenize the input
		slam::lexer status = slam::lex(input, expression);

		// Proceed, quit, or print error
		if( status == slam::lexer::success ){
			slam::parse( expression.begin(), expression.end(), calculator.stump() );
			std::cout << calculator.result() << '\n';
		}else if( status == slam::lexer::quit ){
			ON = false;
		}else{
			std::cout << "\nproblem encountered\n";
		}

		// If debugging, print information about tokens
		/*std::cout << '{' << ' ';
		for(auto debug = expression.begin(); debug != expression.end(); ++debug){
			(*debug).print();
			std::cout << ' ';
		}std::cout << '}' << '\n';*/

		// Reset expression
		expression.clear();
	}

	return std::cout ? 0 : 1;
}

