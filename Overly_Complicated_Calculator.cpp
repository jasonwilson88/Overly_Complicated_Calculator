#include "Overly_Complicated_Calculator.h"
#include <sstream>
#include <cctype>
#include "Enumerator_Functions.h"
using std::string;
using std::istringstream;
using std::stack;
using std::isdigit;

//map containing precedence for binary operators
std::map<func, int> Infix_Evaluator::BINARY_OPERATORS = { {func::power, 7}, {func::multiply, 6}, 
{func::divide, 6}, {func::mod, 6}, {func::add, 5}, {func::minus, 5}, {func::greater, 4}, 
{func::greater_equal, 4}, {func::less, 4}, {func::less_equal, 4}, {func::equal, 3}, 
{func::not_equal, 3}, {func::and, 2}, {func::or, 1}, {func::open_parentheses, 9}, 
{func::close_parentheses, -1}
};

//map containing precedence for unary operators
std::map<func, int> Infix_Evaluator::UNARY_OPERATORS = { {func::not, 8 }, {func::increment, 8 }, 
{func::decrement, 8 },{func::negative, 8 } };

//string to determine if operator can have two characters
const std::string Infix_Evaluator::COMPOUND_OPERATORS = "+-<>!&|=";

//string that contains legal operator characters
const std::string Infix_Evaluator::CHAR_OPERATORS = "^*/%+-><=!&|";

//string that contains open parentheses
const std::string Infix_Evaluator::OPEN_PARENTHESES = "([{";

//string that contains closing parentheses
const std::string Infix_Evaluator::CLOSE_PARENTHESES = ")]}";

//set that contains operators with two chars
std::set<func> Infix_Evaluator::STRING_OPERATORS = { func::increment,
func::decrement, func::greater_equal, func::less_equal, func::equal, func::not_equal, func::and,
func::or };



/** Evaluates a infix expression.
@param expression The expression to be evaluated
@return The value of the expression
@throws Syntax_Error if a syntax error is detected
*/
int Infix_Evaluator::eval(const std::string& expression) {
	// Be sure the stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;
	char previous_char = 'a'; //default previous char value
	char current_char; //char to keep track of current char
	while (tokens >> next_char) {
		if (isdigit(next_char)) {
			if (isdigit(previous_char)) {
				throw Syntax_Error("Two operands in a row");
			}
			else {
				current_char = next_char; //set next_char to working char
				tokens.putback(next_char);
				int value;
				tokens >> value;
				operand_stack.push(value);
			}
		}
		// if char determine char function based on context and process that function
		else if (is_char_operator(next_char)) {
			current_char = next_char; //set char to current working char
			tokens >> next_char; //read in the next char to next_char
			switch (current_char) {
			case '!':
				//if the next char is a digit or the beginning of an expression
				//and if the there is no previous char or the previous char is an operator
				//or the beginning of an expression
				//then ! is logical not
				if ((isdigit(next_char) || is_open_parentheses(next_char) || 
					is_compound_operator(next_char)) && 
					(previous_char == 'a' || is_char_operator(previous_char) ||
						is_open_parentheses(previous_char))) {
					func func = func::not;
					process_operator(func);
					break;
				}
				//if the next char is = and the previous character is a digit 
				//or the end of an expression
				//then it is inequality comparison
				else if (next_char == '=' && 
					(isdigit(previous_char) || is_close_parentheses(previous_char))){
					func func = func::not_equal;
					process_operator(func);
					break;
				}
				//everything else is an error
				else {
					throw Syntax_Error("Invalid use of ! encountered");
				}
			case '+':
				//If the next char is + and
				//if there is no previous char or
				//the previous char is an operator or the beginning of an expression
				//then it is prefix increment
				if (next_char == '+' && 
					(previous_char == 'a' || is_char_operator(previous_char) || is_open_parentheses(previous_char))) {
					func func = func::increment;
					process_operator(func);
					break;
				}
				//if the previous char is a digit or the end of an expression
				//and the next char is a digit or the beginning of an expression
				//then it is add
				else if ((isdigit(previous_char) || is_close_parentheses(previous_char)) &&
					(isdigit(next_char) || is_open_parentheses(next_char) ||
						is_compound_operator(next_char))) {
					func func = func::add;
					process_operator(func);
					break;
				}
				//everything else is an error
				else {
					throw Syntax_Error("Invalid use of + Encountered");
				}
			case '-':
				//If the next char is - and
				//if there is no previous char or
				//the previous char is an operator or the beginning of an expression
				//then it is prefix decrement
				if (next_char == '-' && 
					(previous_char == 'a' || is_char_operator(previous_char) || is_open_parentheses(previous_char))) {
					func func = func::decrement;
					process_operator(func);
					break;
				}
				//If the next char is a digit or the beginning of an expression and
				//if there is no previous char or
				//the previous char is an operator or the beginning of an expression
				//it is a negative
				else if ((isdigit(next_char) || is_open_parentheses(next_char) || 
					is_compound_operator(next_char)) &&
					(previous_char == 'a' || is_char_operator(previous_char) || 
						is_open_parentheses(previous_char))) {
					func func = func::negative;
					process_operator(func);
					break;
				}
				//if the previous char is a digit or the end of an expression and 
				//the next char is a digit or the beginning of an expression
				//then it is a minus
				else if ((isdigit(previous_char) || is_close_parentheses(previous_char)) &&
					(isdigit(next_char) || is_open_parentheses(next_char) || 
						is_compound_operator(next_char))) {
					func func = func::minus;
					process_operator(func);
					break;
				}
				//everything else is an error
				else {
					throw Syntax_Error("Invalid use of - operator");
				}
			case '>':
				//if the next char is an equal and the previous char is either 
				//a digit or the end of an expression
				//then it is greater than or equal to
				if (next_char == '=' && 
					(isdigit(previous_char) || is_close_parentheses(previous_char))) {
					func func = func::greater_equal;
					process_operator(func);
					break;
				}
				//if the next char is a digit or the beginning of an expression and
				//the previous char is a digit or the end of an expression
				//it is greater than
				if ((isdigit(next_char) || is_compound_operator(next_char) || is_open_parentheses(next_char)) && 
					(isdigit(previous_char) || is_close_parentheses(previous_char))) {
					func func = func::greater;
					process_operator(func);
					break;
				}
				//everything else is an error
				else {
					throw Syntax_Error("Invalid Comparison Argument");
				}
			case '<':
				//if the next char is an equal and the previous char is either 
				//a digit or the end of an expression 
				//then it is less than or equal to 
				if (next_char == '=' &&
					(isdigit(previous_char) || is_close_parentheses(previous_char))) {
					func func = func::less_equal;
					process_operator(func);
					break;
				}
				//if the next char is a digit or the beginning of an expression and
				//the previous char is a digit or the end of an expression
				//then it is less than
				if ((isdigit(next_char) || is_compound_operator(next_char) || is_open_parentheses(next_char)) && 
					(isdigit(previous_char) || is_close_parentheses(previous_char))) {
					func func = func::less;
					process_operator(func);
					break;
				}
				//everything else is an error
				else {
					throw Syntax_Error("Invalid comparison argument");
				}
			case '=':
				//if the next_char is an equal and 
				//the previous char was either a digit or the end of an expression
				//then it is equality comparison
				if (next_char == '=' && 
					(isdigit(previous_char) || is_close_parentheses(previous_char))) {
					func func = func::equal;
					process_operator(func);
					break;
				}
				//everyting else is an error
				else {
					throw Syntax_Error("Two binary arguments in a row");
				}
			case '&':
				//if the next char is & and the previous char was either
				//a digit or the end of an expression 
				//then it is logical and
				if (next_char == '&' && 
					(isdigit(previous_char) || is_close_parentheses(previous_char))) {
					func func = func::and;
					process_operator(func);
					break;
				}
				//everyting else is an error
				else {
					throw Syntax_Error("Two binary operators in a row");
				}
			case '|':
				//if the next char is | and the previous char was either 
				//a digit or the end of an expression
				//then it is logical or
				if (next_char == '|' &&
					(isdigit(previous_char) || is_close_parentheses(previous_char))) {
					func func = func:: or ;
					process_operator(func);
					break;
				}
				else {
					throw Syntax_Error("Invalid or statement");
				}
				//power if the previous char was a digit or end of expression
				//and the next char is a digit or the beginning of an expression
			case '^':
				if ((is_close_parentheses(previous_char) || isdigit(previous_char)) ||
					(is_open_parentheses(next_char) || isdigit(next_char) || is_compound_operator(next_char))){
					func func = func::power;
					process_operator(func);
					break;
				}
				else {
					throw Syntax_Error("Invalid power statement");
				}
				//multiply if the previous char is a digit or the end of an expression
				//and the next char is a digit or the beginning of an expression
			case '*':
				if ((is_close_parentheses(previous_char) || isdigit(previous_char)) &&
					(is_open_parentheses(next_char) || isdigit(next_char) || is_compound_operator(next_char))) {
					func func = func::multiply;
					process_operator(func);
					break;
				}
				else {
					throw Syntax_Error("Invalid multiplication statement");
				}
				//divide if the previous char was a digit or the end of an expression
				//and the next char is a digit(not zero) or the beginning of an expression
			case '/':
				if ((is_close_parentheses(previous_char) || isdigit(previous_char)) &&
					(is_open_parentheses(next_char) || isdigit(next_char) || is_compound_operator(next_char)) &&
					next_char != '0') {
					func func = func::divide;
					process_operator(func);
					break;
				}
				else {
					throw Syntax_Error("Invalid division statement");
				}
				//mod if the previous char was a digit or the end of an expression
				//and the next char is a digit(not zero) or the beginning of an expression
			case '%':
				if ((is_close_parentheses(previous_char) || isdigit(previous_char)) &&
					(is_open_parentheses(next_char) || isdigit(next_char) || is_compound_operator(next_char)) &&
					next_char != '0') {
					func func = func::mod;
					process_operator(func);
					break;
				}
				else {
					throw Syntax_Error("Invalid mod statement");
				}
			}//end switch
			//Do some housekeeping
			//if the next char was a digit push it on the operator stack 
			if (isdigit(next_char)) {
				tokens.putback(next_char);
				int value;
				tokens >> value;
				operand_stack.push(value);
				current_char = next_char;//reset the working char to that char
			}
			//if it was a char
			else if (is_char_operator(next_char) || is_open_parentheses(next_char)) {
				if (is_open_parentheses(next_char)) { //if it was an opening parentheses push
					operator_stack.push(func::open_parentheses);
					current_char = next_char;//reset the working char to that char
				}
				//if you used the next char don't put it back
				//but set counters
				else if (is_string_operator(operator_stack.top())) {
					current_char = next_char;
				}
				//if you didn't use the next char and it could be a unary operator
				//put it back on the string
				else if (is_compound_operator(next_char)) {
					tokens.putback(next_char);
				}
				//if its a char and not a possible unary throw error
				else {
					throw Syntax_Error("Closing Parentheses/two binary operators in a row");
				}
			}
			else {
				throw Syntax_Error("Invalid Character encountered");
			}
		}//end operator search
		//if next_char wasn't a char operator
		//determine if it is a parentheses
		//get its related function and
		//process that function

		else if (is_open_parentheses(next_char)) {
			current_char = next_char; //set char to current working char
			tokens >> next_char; //read in the next char to next_char
			//check to make sure the next char is either a digit or the beginning of an expression
			//and that the previous char is not a digit
				if ((isdigit(next_char) || is_open_parentheses(next_char) || is_compound_operator(next_char))
				&& !isdigit(previous_char)){
					operator_stack.push(func::open_parentheses);//no need to process highest priority
					tokens.putback(next_char); //put next char back
				}
				else {
					throw Syntax_Error("Invalid opening parentheses");
				}
		}
		else if (is_close_parentheses(next_char)) {
			current_char = next_char; //set char to current working char
			tokens >> next_char; //read in the next char to next_char
			//check to see if previous char is a digit or the end of an expression
			//and that next char is either an operator or the end of an expression
			if ((isdigit(previous_char) || is_close_parentheses(previous_char)) &&
				(is_close_parentheses(next_char) || is_char_operator(next_char))) {
				func func = func::close_parentheses;
				process_operator(func);
				tokens.putback(next_char); //put next char back
				}
			else {
				throw Syntax_Error("Invalid closing parentheses");
			}
		}
		//any invalid char throw error
		else {
			throw Syntax_Error("Invalid character encountered");
		}
		previous_char = current_char;//set current working char to now be previous char
	}//return to while loop
	//after while loop has finished evaluate remaining operators
	while (!operator_stack.empty()) {
		eval_op(operator_stack.top());
		operator_stack.pop();
	}
	//return answer
	if (!operand_stack.empty()) {
		int answer = operand_stack.top();
		operand_stack.pop();
		if (operand_stack.empty()) {
			return answer;
		}
		else {
			throw Syntax_Error("Stack should be empty");
		}
	}
	else {
		throw Syntax_Error("Stack is empty");
	}
}

/** Function to process operators.
@param op The operator
@throws Syntax_Error
*/
void Infix_Evaluator::process_operator(func op) {
	if (operator_stack.empty() || (operator_stack.top() == func::open_parentheses)) {
		operator_stack.push(op);
		return;
	}
		//if the operator stack is not empty and the operator is not an opening parentheses
		//check if the precedence of the current operator is greater than 
		//the precedence of the top operator
		//if it is push it on the stack and keep going
	if (precedence(op) > precedence(operator_stack.top())) {
			operator_stack.push(op);
			return;
		}
	//unary operators in a row would fail last test but can't operate without operand
	if (is_unary_operator(op) && is_unary_operator(operator_stack.top())) {
		operator_stack.push(op);
		return;
	}
	else {
			//if the precedence of the operator is less than the top then
			// while the operator stack is not empty or opening parentheses,
			//and the precedence of the current operator is less than or
			//equal to the precedence of the operator on top of the stack
			// evaluate all operations
			while (!operator_stack.empty() && (operator_stack.top() != func::open_parentheses) 
				&& precedence(op) <= precedence(operator_stack.top())) {
				func top = operator_stack.top();
				operator_stack.pop();
				eval_op(top);
			}
		}//break if statement
		//if the operator was a closing parentheses get rid of the opening	
	if (op == func::close_parentheses) {
			if (!operator_stack.empty() && (operator_stack.top() == func::open_parentheses)) {
				operator_stack.pop();
			}
			else {
				throw Syntax_Error("Unmatched close parentheses");
			}
		}
	//once you are done push that operator on the stack
	else {
				operator_stack.push(op);
			}
		}

// Evaluates the current operator.
void Infix_Evaluator::eval_op(func op) {
	//evaluate unary
	if ((op == func::not) || (op == func::increment) || (op == func::decrement) || (op == func::negative)) {
		//if the operand stack is empty push
		if (operand_stack.empty()) {
			operator_stack.push(op);
		}
		//if it is not empty evaluate
		else {
			int rhs = operand_stack.top();
			operand_stack.pop();
			int result = 0;
			switch (op) {
			case func::not:
				result = !rhs;
				break;
			case func::increment:
				result = ++rhs;
				break;
			case func::decrement:
				result = --rhs;
				break;
			case func::negative:
				result = -rhs;
				break;
			}
				operand_stack.push(result);
			}
		}
	//evaluate binary operators
	else if (BINARY_OPERATORS.find(op) != BINARY_OPERATORS.end()) {
		int rhs = operand_stack.top();
		operand_stack.pop();
		if (operand_stack.empty())
			throw Syntax_Error("Stack is empty");
		int lhs = operand_stack.top();
		operand_stack.pop();
		int result = 0;
		switch (op) {
		case func::add:
			result = lhs + rhs;
			break;
		case func::minus: 
			result = lhs - rhs;
			break;
		case func::multiply: 
			result = lhs * rhs;
			break;
		case func::divide: 
			if (rhs == 0) {
				throw Syntax_Error("Division by zero");
			}
			else {
				result = lhs / rhs;
			}
			break;
		case func::mod: 
			if (rhs == 0) {
				throw Syntax_Error("Divison by zero");
			}
			else {
				result = lhs % rhs;
			}
			break;
		case func::power: 
			result = pow(lhs,rhs);
			break;
		case func::greater: 
			result = lhs > rhs;
			break;
		case func::greater_equal: 
			result = lhs >= rhs;
			break;
		case func::less: 
			result = lhs < rhs;
			break;
		case func::less_equal: 
			result = lhs <= rhs;
			break;
		case func::equal: 
			result = lhs == rhs;
			break;
		case func::not_equal: 
			result = lhs != rhs;
			break;
		case func::and: 
			result = lhs && rhs;
			break;
		case func::or: 
			result = lhs || rhs;
			break;
		}
		operand_stack.push(result);
	}
}