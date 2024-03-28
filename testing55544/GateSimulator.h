
#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
class LogicGateExpressionEvaluator {
private:
    int precedence(char c) {
        switch (c) {
        case '&': // 2nd highest precedence
            return 2;
        case '|': // lowest precedence
            return 1;
        case '~': // highest precedence
            return 3;
        default:
            return -1;
        }
    }

    bool isOperator(char c) { //spot operator
        return (c == '&' || c == '|' || c == '~');
    }

    //convert infix boolean expression to postfix
    string infixToPostfix(const string& infix) {
        stack<char> st; //create stack
        string postfix;

        //loop over every char in infix expression
        for (char c : infix) {
            if (isdigit(c)) { //check if the character is a digit (variable name)
                postfix += c; //add it to the postfix expression
            }
            else if (c == '(') {
                st.push(c); //push onto stack if c=(
            }
            else if (c == ')') {
                while (!st.empty() && st.top() != '(') {
                    postfix += st.top(); //add from the top of the stack to the post fix expression until you meet '('
                    st.pop();
                }
                if (!st.empty() && st.top() == '(') {
                    st.pop(); // Remove '(' from the stack
                }
            }
            else if (isOperator(c)) { //if the character is a variable
                while (!st.empty() && precedence(st.top()) >= precedence(c)) {
                    postfix += st.top();// add operators from the stack if they have higher or equal precedence
                    st.pop();
                }
                st.push(c);
            }
        }
        //add remaining operators to the postfix expression
        while (!st.empty()) {
            postfix += st.top();
            st.pop();
        }

        return postfix;
    }

    //evaluate postfix expression
    bool evaluatePostfix(const string& postfix, const vector<bool>& variables) {
        stack<bool> st; //hold variables

        //loop over every char in postfix expression
        for (char c : postfix) {
            if (c == '~') {
                //apply negation to the top variable
                if (!st.empty()) {
                    bool variable = st.top();
                    st.pop();
                    st.push(!variable);
                }
            }
            else if (isOperator(c)) {
                //evaluate the operation
                if (st.size() < 2) {
                    //insufficient number of variables in the stack
                    return false;
                }
                bool variable2 = st.top();
                st.pop();
                bool variable1 = st.top();
                st.pop();
                switch (c) {
                case '&':
                    st.push(variable1 && variable2);
                    break;
                case '|':
                    st.push(variable1 || variable2);
                    break;
                }
            }
            else if (isdigit(c)) { //check if the character is a digit (variable name)
                int idx = c - '0'; //convert to int (assuming variables are named as i1, i2, i3...)
                //subtracting by ASCII value
                if (idx < 1 || idx > variables.size()) {
                    //invalid variable index
                    return false;
                }
                st.push(variables[idx - 1]); //push variable name onto stack
            }
        }

        if (st.size() != 1) {
            return false;
        }

        return st.top();
    }

public:
    //evaluate infix boolean expression
    bool evaluateExpression(const string& expression, const vector<bool>& variables) {
        string postfix = infixToPostfix(expression);
        return evaluatePostfix(postfix, variables);
    }

};
//int main written by chatgpt to test code
//int main() {
//    LogicGateExpressionEvaluator evaluate;
//
//    string expression = "(i1&~i2)|~(i3&~i4)";
//    int num_variables = 4; // Up to i4
//
//    vector<bool> inputs(num_variables);
//    // Setting values for i1, i2, i3, and i4
//    inputs[0] = true;
//    inputs[1] = true;
//    inputs[2] = true;
//    inputs[3] = false;
//
//
//    // Test expression 1
//    expression = "(i1&~i2)|~(i3&~i4)";
//
//    cout << "Testing expression 1: " << expression << endl;
//    cout << "With inputs:";
//    for (bool val : inputs) {
//        cout << " " << val;
//    }
//    cout << endl;
//    bool result1 = evaluate.evaluateExpression(expression, inputs);
//    cout << "Result: " << (result1 ? "true" : "false") << endl;
//
//    // Test expression 2
//    expression = "(i1|i2)&~(i3|i4)";
//    cout << "Testing expression 2: " << expression << endl;
//    cout << "With inputs:";
//    for (bool val : inputs) {
//        cout << " " << val;
//    }
//    cout << endl;
//    bool result2 = evaluate.evaluateExpression(expression, inputs);
//    cout << "Result: " << (result2 ? "true" : "false") << endl;
//
//    // Test expression 3
//    expression = "i1&i2&i3&i4";
//    cout << "Testing expression 3: " << expression << endl;
//    cout << "With inputs:";
//    for (bool val : inputs) {
//        cout << " " << val;
//    }
//    cout << endl;
//    bool result3 = evaluate.evaluateExpression(expression, inputs);
//    cout << "Result: " << (result3 ? "true" : "false") << endl;
//
//    // Test expression 4
//    expression = "~i1&~i2&~i3&~i4";
//
//    cout << "Testing expression 4: " << expression << endl;
//    cout << "With inputs:";
//    for (bool val : inputs) {
//        cout << " " << val;
//    }
//    cout << endl;
//    bool result4 = evaluate.evaluateExpression(expression, inputs);
//    cout << "Result: " << (result4 ? "true" : "false") << endl;
//
//    return 0;
//}
