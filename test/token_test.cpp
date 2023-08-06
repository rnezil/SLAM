#include "token.hpp"
#include <iostream>
#include <cmath>

using toke = slam::token;

int main(){
	toke val(69);
	std::cout << "Number token: " <<
		(val.info() == toke::type::number) <<
		". Has value: " << val.value() << '\n';


	toke log([](double d){return std::log10(d);});
	std::cout << "Function token: " <<
		(log.info() == toke::type::function) <<
		". For input=1000, returns: " <<
		log.function()(1000) << '\n';


	toke add(toke::type::plus);
	std::cout << "Addition operator: " <<
		(add.info() == toke::type::plus) << '\n';

	return 0;
}

