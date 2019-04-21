#include <iostream>
#include <string>
#include <queue>
#include <stack>



using namespace std;


// GLOBAL VARIABLES
double uservariable;






//
//
//
// HELPER FUNCTIONS
//
//
//



// RETURNS true IF op1 >> op2  IN PRECEDENCE
bool higher_precedence(string op1, string op2) {


	if (op1 == "*" || op1 == "/") {
		if (op2 == "+" || op2 == "-") {
			return true;
		}
	}
	return false; // op1 lower or equal precedence
}




// CONVERTS A QUEUE OF INFIX TOKENS TO QUEUE OF POSTFIX TOKENS
queue <string> infix_postfix(queue <string> infix) {

	string operations = "-+*/";
	stack <string> operatorstack; // INTERMEDIARY OPERATOR STACK
	queue <string> postfix;




	// CONVERTS INFIX TO POSTFIX USING AN INTERMEDIARY STACK
	while (!infix.empty()) {

		// (OPERATOR) FROM INFIX TO POSTFIX
		if (infix.front().length() == 1 &&
			operations.find(infix.front(), 0) < 4) {

			//IF NO OPERATORS, WE ADD TO STACK
			if (operatorstack.empty()) {
				operatorstack.push(infix.front());
				infix.pop();
			}
			else { // WE HAVE TO COMPARE THE GIVEN OPERATOR TO PRECEDENCE ON STACK

				// CURRENT OPERATOR HAS HIGHER PRECEDENCE
				if (higher_precedence(infix.front(), operatorstack.top())) {
					operatorstack.push(infix.front());
					infix.pop();
				}
				// CURRENT OPERATOR HAS EQUAL OR LOWER PRECEDENCE
				else {
					// POPS HIGHER PRECEDENCE OPERATORS
					while (!operatorstack.empty() && !higher_precedence(infix.front(), operatorstack.top())) {
						postfix.push(operatorstack.top());
						operatorstack.pop();
					}
					// ADDS THE OPERATOR TO THE STACK
					operatorstack.push(infix.front());
					infix.pop();
				}
			}
		}

		// (NUMBER) FROM INFIX TO POSTFIX
		else {
			if (infix.front().length() >0) {
				postfix.push(infix.front());
				infix.pop();
			}
			else {
				infix.pop();
			}
		}

	}

	// PUTTING REMAINING OPERATORS TO POSTFIX QUEUE
	while (!operatorstack.empty()) {
			postfix.push(operatorstack.top());
			operatorstack.pop();
		}


	return postfix;
}



// TAKES A STRING REPRESENTING A MATH EXPRESSION
queue <string> process_exp(string expression) {

	string whitespace = " \n\r\t\v\f";
	string operations = "-+*/";
	string accepted = " \n\r\t\v\f-+*/0123456789x"; // ALL ACCEPTED CHARS

	queue <string> infix;
	int length;
	int count;
	int tempcount;
	bool inputerror;
	bool lastOP;


	//INITIALIZE VARIABLES
	length = expression.length();
	count = 0;
	inputerror = false;
	lastOP = true;



	// ITERATES THROUGH EVERY CHAR IN EXPRESSION STRING
	// CHECKS FOR USER ERRORS
	while (count < length && !inputerror) {


		// CHECKS IF USER-DEFINED VARIABLE IS BEING USED
		if (expression[count] == 'x') {
			count++;
			infix.push(to_string(uservariable));
		}

		// ENDS PROCESSING IF WRONG CHARACTER INPUTTED
		if (count < length && accepted.find(expression[count], 0) >= 21) {
			cout << "Error! Wrong Input Given. Try Again." << endl;
			inputerror = true;
		}


		// REMOVES WHITESPACES
		while (count < length && whitespace.find(expression[count], 0) < 6) {
			count++;
		}


		// FINDS NEGATIVE NUMBERS
		if (count < length && expression[count] == '-' && isdigit(expression[count+1]) && lastOP) {
			tempcount = count+1;
			while (isdigit(expression[tempcount])) {
				tempcount++;
			}

			infix.push(expression.substr(count, tempcount-count));
			count = count + tempcount-count;
			lastOP = false;
		}


		// FINDS + - * / OPERATIONS IN EXPRESSION
		if (count < length && operations.find(expression.substr(count,1), 0) < 4) {

			infix.push(expression.substr(count,1));
			count++;
			lastOP = true;
		}

		// FINDS NUMBERS AND DIGITS (NON-NEGATIVE)
		if (count < length && isdigit(expression[count])) {
			tempcount = count;
			while (isdigit(expression[tempcount])) {
				tempcount++;
			}

			infix.push(expression.substr(count, tempcount-count));
			count = count + tempcount-count;
			lastOP = false;
		}

		// DEALS WITH SPECIAL CASE OF PARENTHESES
		if (expression[count] == '(' || expression[count] == ')') {
			cout << "Parentheses not supported..." << endl;
			inputerror = true;
		}



	}

	// RETURNS EMPTY QUEUE IF USER ERROR OCCURRED
	if (inputerror){
		queue <string> bad;
		return bad;
	}

	return infix;

}



// CHECKS THAT A STACK HAS MINIMUM OF TWO DOUBLES ON TOP
bool two_operands (stack <double> results) {

	double m;
	double n;

	if (!results.empty()) {
		m = results.top();
		results.pop();
	}
	else {
		return false;
	}

	if (!results.empty()) {
		n = results.top();
		results.pop();
	}
	else {
		return false;
	}


	// ADDS THEM BACK TO THE STACK
	results.push(n);
	results.push(m);
	return true;
}





//
//
// MAIN
//
//
int main()
{

	queue <string> infix;
	queue <string> postfix;
	stack <double> results;
	double a;
	double b;
	string expression;



	// ON FIRST RUN, USER VARIABLE 'x' WILL BE EQUAL TO 1
	uservariable = 1;

	// USER INPUT
	cout << "WELCOME TO THE CALCULATOR" << endl;
	cout << "Operators Include: + - * /" << endl;
	cout << "Operands must be integers!" << endl;
	cout << "Results will be to precision (double)" << endl;
	cout << "Each time an expression is evaulated," << endl;
	cout << "the results is saved to variable 'x'" << endl;
	cout << "Initially, x = 1" << endl;
	cout << "Type 'q' to exit\n\n" << endl;
	cout << "Sample Input: 3 * 3" << endl;
	cout << "Sample Output: 9" << endl;
	cout << "Input: x /2 " << endl;
	cout << "Output: 4.5" << endl;
	cout << "Give me an expression!" << endl;
	getline(cin, expression);


	// WHILE USER DOESN'T QUIT THE PROGRAM
	while (expression.length() >= 1 && expression.substr(0,1) != "q") {

		// PROCESSES INPUT STRING INTO TOKENS
		infix = process_exp(expression);

		// CONVERTS INFIX QUEUE TO POSTFIX QUEUE
		postfix = infix_postfix(infix);



		// THE FOLLOWING USES THE POSTFIX AND OPERAND STACK TO EVALUATE
		// THE POSTFIX EXPRESSION
		bool abort = false;
		while (!postfix.empty() && !abort) {
			if (postfix.front() == "+") {
				if (!two_operands(results)) {
					abort = true;
					break;
				}
				a = results.top();
				results.pop();
				b = results.top();
				results.pop();
				results.push(a + b);

			}
			else if (postfix.front() == "-") {
				if (!two_operands(results)) {
					abort = true;
					break;
				}
				a = results.top();
				results.pop();
				b = results.top();
				results.pop();
				results.push(b - a);
			}
			else if (postfix.front() == "*") {
				if (!two_operands(results)) {
					abort = true;
					break;
				}
				a = results.top();
				results.pop();
				b = results.top();
				results.pop();
				results.push(a * b);
			}
			else if (postfix.front() == "/") {
				if (!two_operands(results)) {
					abort = true;
					break;
				}
				a = results.top();
				results.pop();
				b = results.top();
				results.pop();

				if (a != 0)
					results.push(b / a);
				else {
					abort = true;
					cout << "Division by Zero! Error!" << endl;
					break;
				}
			}
			else {
				results.push(stod(postfix.front()));
			}
			postfix.pop();
		}

		// ONLY PRINTS ANSWER IF NO ERRORS HAPPENED
		if (!results.empty() && !abort) {
			cout << results.top() << endl;
			uservariable = results.top(); // SETS NEW USER VARIABLE
		}
		else {
			cout << "There was an error. Please try again :)" << endl;
		}


		// CLEARS THE OPERAND STACK
		while (!results.empty()){
			results.pop();
		}


		// ASKS USER FOR NEW EXPRESSION TO EVALUATE
		// EVERYTHING RUNS AGAIN
		cout << "\n\nGive me an expression!" << endl;
		getline(cin, expression);

	}



	return 0;

}
