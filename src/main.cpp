#include "ast.hpp"

int main(int argc, char** argv){
	// Initialize local variables
	std::string input {};
	std::vector<slam::token> expression {};
	bool running = true;

	// Initialize SLAM environment variables
	bool SLAM_debug = false;
	bool SLAM_interactive = false;
	bool SLAM_radian = true;
	bool SLAM_precision = false;
	int precision = 0;

	// Failure condition
	if( argc == 1 ){
		std::cerr << "SLAM user error: blank input expressions only allowed in interactive mode.\n";
		return 1;
	}

	// Handle command line arguments
	int options = 1;
	while( **(argv+options) == '-' ){
		switch( *(*(argv+options)+1) ){
			case 'g':
				if( *(*(argv+options)+2) == '\0' ){
					SLAM_radian = false;
					break;
				}else{ 
					std::cerr << "SLAM user error: \"" <<
						*(argv+options) << "\" is not a valid option\n";
					return 1;
				}
			case 'i':
				if( *(*(argv+options)+2) == '\0' ){
					SLAM_interactive = true;
					break;
				}else{
					std::cerr << "SLAM user error: \"" <<
						*(argv+options) << "\" is not a valid option\n";
					return 1;
				}
			case 'd':
				if( *(*(argv+options)+2) == '\0' ){
					SLAM_debug = true;
					break;
				}else{
					std::cerr << "SLAM user error: \"" <<
						*(argv+options) << "\" is not a valid option\n";
					return 1;
				}
			case 'p':
				if( *(*(argv+options)+2) == '=' ){
					if( *(*(argv+options+3)) == '\0' ){
						std::cerr << "SLAM user error: \"" <<
							*(argv+options) <<
							"\" is not a valid option\n";
						return 1;
					}else{
						std::string prec {};
						for( int G = 3; *(*(argv+options)+G) != '\0'; ++G ){
							prec += *(*(argv+options)+G);
						}

						try{
							precision = std::stoi(prec);
							if( precision > 15 ){
								std::cerr << "SLAM user error: precision \"" << *(argv+options) << "\" is too large -- using default precision instead\n";
							}else if( precision < 0 ){
								std::cerr << "SLAM user error: \"" << *(argv+options) << "\" gives a negative precision value -- using default precision instead\n";
							}else{
								SLAM_precision = true;
							}
						}catch( const std::invalid_argument& e ){
							std::cerr << "SLAM user error: \"" <<
								*(argv+options) << "\" is not a valid option\n";
						}catch( const std::out_of_range& e ){
							std::cerr << "SLAM user error: precision \"" <<
								*(argv+options) << "\" is too large -- using default precision instead\n";
						}
						break;
					}
				}else{
					std::cerr << "SLAM user error: \"" <<
						*(argv+options) << "\" is not a valid option\n";
					return 1;
				}
			default:
				std::cerr << "SLAM user error: \"" <<
					*(argv+options) << "\" is not a valid option\n";
					return 1;
				break;
		}

		if( ++options == argc )
			break;
	}

	if( options == argc && !SLAM_interactive ){
		std::cerr << "SLAM user error: blank input expressions only allowed in interactive mode\n";
		return 1;
	}

	if( argc - options > 1 ){
		std::cerr << "SLAM user error: command line options must be entered before expressions\n";
		return 1;
	}
	
	// Get expression from command line arguments
	if( SLAM_interactive ){
		// Can't use command line expressions in interactive mode
		std::cerr << "SLAM user error: command line expressions disallowed in interactive mode\n";
	}else{
		// Store input string
		input = std::string(*(argv+options));

		// Append an equals sign to the end of the
		// string if this was not done already
		if( input.back() != '=' ){
			input += '=';
		}
	}

	// Greet interactive users
	if( SLAM_interactive ){
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		std::cout << "\t\t\t\t\tRyland Nezil 2023";
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		std::cout << "\tWelcome to SLAM! To get started, enter an expression followed by an equals sign below:";
		std::cout << "\n\t\t\t\t\t (Enter Q= to quit)";
		if( !SLAM_radian ){
			std::cout << "\n\n\t\t     WARNING: Degree mode does not produce reliable results.";
		}
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
	}
	
	// Instantiate the tree object
	slam::tree calculator;

	while(running){
		if( SLAM_interactive ){
			// Get a line from stdin
			std::cout << "\t\t";
			std::getline(std::cin, input, '=');
		}else{
			// Indicate program should end after this computation
			running = false;
		}

		// Tokenize the input
		slam::lexer status = slam::lex(input, expression, SLAM_radian);

		// Proceed, quit, or print error
		if( status == slam::lexer::success ){
			try{
				// Parse expression and compute result
				slam::parse( expression.begin(), expression.end(), calculator.stump() );

				// Print extra info if debugging
				if( SLAM_debug ){
					std::cout << "\nTokens:\t{ ";
					for(auto debug = expression.begin(); debug != expression.end(); ++debug){
						(*debug).print();
						std::cout << ' ';
					}
					
					std::cout << "}\n";
				}

				// Print result from calculation
				if( SLAM_precision ){
					// Format result to specified precision
					std::string formatted_result = std::to_string(calculator.result());
					bool has_fractional = false;
					int fractional_part = 0;
					for( auto dig : formatted_result ){
						// Count how many fractional digits
						if( has_fractional ){
							++fractional_part;
						}

						// Find position of decimal point
						if( dig == '.' ){
							has_fractional = true;
						}
					}

					// Chop extra digits off if necessary
					if( has_fractional ){
						if( fractional_part > precision ){
							for( fractional_part; fractional_part != precision; --fractional_part ){
								formatted_result.pop_back();
							}
						}
					}

					// Output formatted result
					if( SLAM_interactive ){
						std::cout << "\n\t\t\t";
						std::cout << formatted_result;
						std::cout << "\n\n\n";
					}else{
						std::cout << formatted_result;
					}
				}else{
					// Output result with native precision
					if( SLAM_interactive ){
						std::cout << "\n\t\t\t";
						std::cout << calculator.result();
						std::cout << "\n\n\n";
					}else{
						std::cout << calculator.result();
					}
				}
			}catch( const slam::lexer parse_error ){
				if( parse_error == slam::lexer::bad_division ){
					std::cerr << "SLAM user error: attempted division by zero (nice try)\n";
				}else if( parse_error == slam::lexer::bad_square_root ){
					std::cerr << "SLAM user error: tried to take square root of a negative number (complex arithmetic is not supported)\n";
				}else if( parse_error == slam::lexer::bad_factorial ){
					std::cerr << "SLAM user error: factorial argument must be a whole number\n";
				}else if( parse_error == slam::lexer::bad_sine ){
					std::cerr << "SLAM user error: argument of inverse sine function must not have magnitude greater than unity\n";
				}else if( parse_error == slam::lexer::bad_cosine ){
					std::cerr << "SLAM user error: argument of inverse cosine function must not have magnitude greater than unity\n";
				}else if( parse_error == slam::lexer::bad_logarithm ){
					std::cerr << "SLAM user error: logarithm arguments must be greater than zero\n";
				}else if( parse_error == slam::lexer::parse_failure ){
					std::cerr << "SLAM internal error: parse algorithm failed\n";
				}else{
					std::cerr << "SLAM internal error: critical failure\n";
					return 1;
				}
			}
		}else if( status == slam::lexer::invalid_number ){
			std::cerr << "SLAM user error: invalid number\n";
		}else if( status == slam::lexer::unknown_function ){
			std::cerr << "SLAM user error: unknown function keyword\n";
		}else if( status == slam::lexer::unpaired_parentheses ){
			std::cerr << "SLAM user error: improper parenthesization\n";
		}else if( status == slam::lexer::empty_parentheses ){
			std::cerr << "SLAM user error: empty parentheses\n";
		}else if( status == slam::lexer::blank_expression ){
			std::cerr << "SLAM user error: received blank input expression\n";
		}else if( status == slam::lexer::missing_binary_operand ){
			std::cerr << "SLAM user error: binary operator expected more operands (ensure proper syntax is being being used for negative numbers)\n";
		}else if( status == slam::lexer::missing_binary_operator ){
			std::cerr << "SLAM user error: expected binary operator (typically due to implicit multiplication, for example writing 5log(10) instead of 5*log(10))\n";
		}else if( status == slam::lexer::missing_unary_operand ){
			std::cerr << "SLAM user error: cannot identify unary function argument\n";
		}else if( status == slam::lexer::quit ){
			break;
		}else{
			std::cerr << "SLAM internal error: critical failure\n";
			return 1;
		}


		// Reset the data structure
		calculator.reset();

		// Clear old tokens from expression
		expression.clear();
	}

	return calculator.reset() ? 0 : 1;
}

