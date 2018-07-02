#include "Overly_Complicated_Calculator.h"
#include <iostream>

int main() {
	Infix_Evaluator eval;
	int result = eval.eval("(++3 + --4) - ++(3 + -2) ^ (5 -3^++1) + ++-3^2");
	std::cout << result << std::endl;

	return 0;
}