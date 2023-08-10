#ifndef TOKE
#define TOKE

#include <functional>
#include <utility>
#include <iostream>

namespace slam {

class token
{
public:
	enum class type {
		number = 0,
		unary_function,
		binary_function,
		open_paren,
		closed_paren,
		dummy,
	};

	// Tokens are not default constructible
	// or copyable
	token() = delete;
	token(const token&) = delete;
	token& operator=(const token&) = delete;

	// Move constructor
	token(token&& other): info_(other.info()), id_(other.what()), function_(nullptr) {
		if( info() == type::number ){
			function_ = new double(std::move(
					*static_cast<double*>(
						other.function_
						)
					));
		}else if( info() == type::unary_function ){
			function_ = new std::function<double(double)>(std::move(
					*static_cast<std::function<double(double)>*>(
						other.function_
						)
					));
		}else if( info() == type::binary_function ){
			function_ = new std::function<double(double,double)>(std::move(
					*static_cast<std::function<double(double,double)>*>(
						other.function_
						)
					));
		}
	}

	// Move assignment operator
	token& operator=(token&& other){
		// Free memory allocated to this object
		if( info() == type::number ){
			delete static_cast<double*>(function_);
		}else if( info() == type::unary_function ){
			delete static_cast<std::function<double(double)>*>(function_);
		}else if( info() == type::binary_function ){
			delete static_cast<std::function<double(double,double)>*>(function_);
		}
		
		// Copy information
		info_ = other.info();
		id_ = other.what();
		
		// Allocate new object and populate with old resources
		if( info() == type::number ){
			function_ = new double(std::move(
					*static_cast<double*>(
						other.function_
						)
					));
		}else if( info() == type::unary_function ){
			function_ = new std::function<double(double)>(std::move(
					*static_cast<std::function<double(double)>*>(
						other.function_
						)
					));
		}else if( info() == type::binary_function ){
			function_ = new std::function<double(double,double)>(std::move(
					*static_cast<std::function<double(double,double)>*>(
						other.function_
						)
					));
		}

		// Return newly created object
		return *this;
	}

	// Constructor for parentheses and dummy tokens
	// Precondition: argument is type::open_paren,
	// type_closed::paren, or type::dummy
	token( type info ): info_(info), function_(nullptr) , id_(0) {}

	// Constructor for numbers
	token( double val ): info_(type::number), id_('#') {
		function_ = new double(val);
	}

	// Constructor for unary operators
	token( std::function<double(double)>&& op, char id = '?' ):
		info_(type::unary_function), id_(id) {
		function_ = new std::function<double(double)>(std::move(op));
	}
	
	// Constructor for binary operators
	token( std::function<double(double,double)>&& op, char id ):
		info_(type::binary_function), id_(id) {
		function_ = new std::function<double(double,double)>(std::move(op));
	}
	
	// Destructor
	~token(){
		// Free memory
		if( info() == type::number ){
			delete static_cast<double*>(function_);
		}else if( info() == type::unary_function ){
			delete static_cast<std::function<double(double)>*>(function_);
		}else if( info() == type::binary_function ){
			delete static_cast<std::function<double(double,double)>*>(function_);
		}
		
		// Nullify pointer
		function_ = nullptr;
	}

	// Return token type
	type info() const { return info_; }

	// Return binary operation key
	char what() const { return id_; }

	// Return pointer to token-specific data
	const void* const take() const { return function_; }

	// Print details about this token
	void print() const {
		std::cout << '[';
		if( info() == type::number )
			std::cout << *static_cast<const double* const>(take());
		else if( info() == type::dummy )
			std::cout << '~';
		else if( info() == type::open_paren )
			std::cout << '(';
		else if( info() == type::closed_paren )
			std::cout << ')';
		else if( info() == type::unary_function )
			std::cout << 'F';
		else if( info() == type::binary_function )
			std::cout << what();
		else
			std::cout << '?';
		std::cout << ']';
	}
private:
	type info_;
	void* function_;
	char id_;
};

}

#endif

