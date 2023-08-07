#include "token.hpp"
#include <vector>
#include <string>

namespace slam{
	enum class lex{
		success = 0,
		too_many_decimal_points,
		no_digits_after_decimal_point,
		mismatched_brackets,
		adjacent_binary_operators,
		unknown_function_name,
		undefined_function_target,
		missing_binary_operand,
	};

	lex tokenize( const std::string& input, std::vector<token>& output );
}

