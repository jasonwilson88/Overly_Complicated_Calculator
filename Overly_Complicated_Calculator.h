#ifndef OVERLY_COMPLICATED_CALCULATOR_H_
#define OVERLY_COMPLICATED_CALCULATOR_H_

#include <map>
#include <set>
#include <stack>
#include <string>
#include "Syntax_Error.h"
#include "Enumerator_Functions.h"

class Infix_Evaluator {
	public:

	/** Evaluates an infix expression.
	@param expression The expression to be evaluated
	@return The value of the expression
	@throws Syntax_Error if a syntax error is detected
	*/
	int eval(const std::string& expression);

private:

	/** Evaluates the current operator.
	This function pops the two operands off the operand
	stack and applies the operator.
	@param op A character representing the operator
	@throws Syntax_Error if top is attempted on an empty stack
	*/
	void eval_op(func op);

	/** Determines whether a character is an operator.
	@param ch The character to be tested
	@return true if the character is an operator
	*/
	//determines if char could be part of compound operator
	bool is_compound_operator(char ch) const {
		return COMPOUND_OPERATORS.find(ch) != std::string::npos;
	};
	//determines if char is a valid operator char
	bool is_char_operator(char ch) const {
		return CHAR_OPERATORS.find(ch) != std::string::npos;
	};

	//determines if char is an opening parentheses
	bool is_open_parentheses(char ch) const {
		return OPEN_PARENTHESES.find(ch) != std::string::npos;
	};
	//determines if char is closing parentheses
	bool is_close_parentheses(char ch) const {
		return CLOSE_PARENTHESES.find(ch) != std::string::npos;
	};
	//determines if function is a two char function
	bool is_string_operator(func op) const {
		if (STRING_OPERATORS.find(op) != STRING_OPERATORS.end())
			return true;
	};

	bool is_unary_operator(func op) const {
		return UNARY_OPERATORS.find(op) != UNARY_OPERATORS.end();
	}
	/** Function to process operators.
	@param op The operator
	@throws Syntax_Error
	*/
	void process_operator(func op);

	/** Determines the precedence of an operator.
	@param op The operator
	@return The precedence
	*/
	int precedence(func op) const {
		if (BINARY_OPERATORS.find(op) != BINARY_OPERATORS.end())
			return BINARY_OPERATORS[op];
		else
			return UNARY_OPERATORS[op];
	}
		// Data fields
		static std::map<func, int> UNARY_OPERATORS;
		static std::map<func, int> BINARY_OPERATORS;
		static std::set<func> STRING_OPERATORS;
		static const std::string CHAR_OPERATORS;
		static const std::string COMPOUND_OPERATORS;
		static const std::string OPEN_PARENTHESES;
		static const std::string CLOSE_PARENTHESES;
		std::stack<int> operand_stack;
		std::stack<func> operator_stack;

};
#endif

