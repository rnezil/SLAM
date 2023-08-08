#ifndef TOKE
#define TOKE

#include <functional>

namespace slam {

class token
{
public:
	enum class type {
		number = 0,
		function,
		exponent,
		multiply,
		divide,
		plus,
		minus,
		open_bracket,
		closed_bracket,
	};

	// Tokens are not default constructible
	token() = delete;

	// For numbers
	token( double val ): info_(type::number), value_(val) {}

	// For ^, *, /, +, -, (, )
	token( type what ): info_(what) {}

	// For unary operators
	token( std::function<double(double)> op ): info_(type::function), function_(op) {}

	// Return value of number
	double value() const { return value_; }

	// Return token type
	type info() const { return info_; }

	// Return function of unary operator
	std::function<double(double)> function() const { return function_; }
private:
	type info_;
	std::function<double(double)> function_;
	double value_;
};

}

#endif

