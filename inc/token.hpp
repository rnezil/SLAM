#ifndef TOKE
#define TOKE

#include <functional>
#include <utility>

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
	token() = delete;
	token(const token&) = delete;
	token& operator=(const token&) = delete;

	// Move constructor
	token(token&& other): info_(other.info()), function_(nullptr) {
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
		// Copy information
		info_ = other.info();
		
		// Free memory allocated to this object
		if( info() == type::number ){
			delete static_cast<double*>(function_);
		}else if( info() == type::unary_function ){
			delete static_cast<std::function<double(double)>*>(function_);
		}else if( info() == type::binary_function ){
			delete static_cast<std::function<double(double,double)>*>(function_);
		}
		
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
	token( type info ): info_(info) { function_ = nullptr; }

	// Constructor for numbers
	token( double val ): info_(type::number) {
		function_ = new double(val);
	}

	// Constructor for unary operators
	token( std::function<double(double)>&& op ): info_(type::unary_function) {
		function_ = new std::function<double(double)>(std::move(op));
	}
	
	// Constructor for binary operators
	token( std::function<double(double,double)>&& op ): info_(type::binary_function) {
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

	// Return pointer to token-specific data
	const void* const take() const { return function_; }
private:
	type info_;
	void* function_;
};

}

#endif

