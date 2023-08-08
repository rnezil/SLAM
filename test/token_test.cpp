#include "token.hpp"
#include <iostream>
#include <cmath>

using toke = slam::token;

int main(){
	toke val(69);
	std::cout << "Number token: " <<
		(val.info() == toke::type::number) <<
		". Has value: " << *static_cast<const double* const>(val.take()) << '\n';


	toke log([](double d){return std::log10(d);});
	std::cout << "Unary log token: " <<
		(log.info() == toke::type::unary_function) <<
		". For input=1000, returns: " <<
		(*static_cast<const std::function<double(double)>* const>(log.take()))(1000) << '\n';


	toke add([](double d1, double d2){return d1 + d2;});
	std::cout << "Binary add token: " <<
		(add.info() == toke::type::binary_function) <<
		". For input 999 and 338, returns: " <<
		(*static_cast<const std::function<double(double,double)>* const>(add.take()))(999,338) << '\n';

	return 0;
}

