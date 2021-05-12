/*
Project Description: Program reads a mathematical expression inputted with infix notation, 
                     converts to postfix notation using shunting yard algorithm,
                     uses postfix notation to create binary expression tree, 
                     allows user to output expression in infix, prefix, or postfix notation,
                     and prints expression using the binary expression tree.
Name: Aammya Sapra
Date: 05-07-2021
*/


#include <iostream>
#include <cstring>
#include <stack>
#include "BTnode.h"

using namespace std;



struct Node {
	char data;
	Node* next;
};




void intoPost(char* input, Node* &top, Node *&front, Node* &back);
void printList(Node* start, char output[], Node* &front, Node* &end);
void push(char value, Node* &top);
int peek(Node* &top);
bool isOperator(char c);
int getPrecedence(char c);
BTnode* constructTree(char output[]);
void enqueue(char value, Node* &front, Node* &back);
void dequeue(Node *&front, Node *&back);
void postfix(BTnode* head);
void prefix(BTnode* head);
void infix(BTnode* head);



int main() {

	//pointer to top of stack
	Node* top = NULL;

	//pointers to front and end of queue
	Node* front = NULL;
	Node* end = NULL;

	//take input in from user
	char* input = new char();
	
	char output[100];
	memset(output, '\0', 100);

	cout << "Enter a mathmatical expression in infix notation. Use a space between each character." << endl;

	cin.getline(input, 80, '\n');
	//use a postfix function to translate expression into postfix notation
	intoPost(input, top, front, end);
	cout << "Inputted expression in postfix notation: " << endl;
	//print output
	printList(front, output, front, end);
	cout << output << endl;
	bool running = true;
    // creates a binary expression tree based on the postfix expression
	BTnode* head = constructTree(output);

	while (running) {
		cout << "\nWhat would you like to do? Type 1, 2, 3, or 4 (1 - Convert to infix, 2 - Convert to prefix, 3 - Convert to postfix, 4 - QUIT)" << endl;
		cin.getline(input, 80, '\n');
		if (strcmp(input, "1") == 0) {
            infix(head);
        }
        else if (strcmp(input, "2") == 0) {
            prefix(head);
        }
        else if (strcmp(input, "3") == 0) {
            postfix(head);
        }
        else {
            cout << "Thanks for hanging! See you next time!\n";
            running = false;
        }

	}
}	

//Push, which adds an element to the collection
void push(char value, Node* &top) {
	Node* node = new Node();
	node->data = value;
	node->next = top;
	top = node;
}

//Pop, which removes the most recently added element that was not yet removed.
void pop(Node* &top) {
	Node* temp = top;
	if(temp == NULL) {
		cout << "Stack is empty." << endl;
	}
	else {
		//pops the current top to the next top
		top = top->next;
		delete temp;
	}
}

//peek at the top of the stack
int peek(Node* &top) {
	return top->data;
}

bool isOperator(char c) {
  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
        return true;
    }
    else { 
    	return false; 
    }
}

// returns the precedence of each operator
int getPrecedence(char c) {
    if (c == '^') {
        return 4;
    }
    else if (c == '*' || c == '/') {
        return 3;
    }
    else if (c == '+' || c == '-') {
        return 2;
    }
    else {
        return 0;
    }
}

BTnode* constructTree(char output[]) {
	stack<BTnode*> stack;
	BTnode *head, *left, *right;

	for (int i = 0; i < strlen(output); i++) {
		if (output[i] != ' ') {
			//if its an operand, then push into stack
			if(!isOperator(output[i])) {
				head = new BTnode(output[i]);
				stack.push(head);
			}
			else { //otherwise if its a operator 
				head = new BTnode(output[i]);
				//pop the two top nodes
				right = stack.top(); //store top
				stack.pop();
				left = stack.top(); 
				stack.pop();

				// make them children
				head->right = right;
				head->left = left;

				//add this subexpression to stack
				stack.push(head);
				
			}
		}
	}
	head = stack.top();
    stack.pop();
    // returns the head of the  tree
    return head;
}

// creates the postfix expression from the initial infix given
// using shunting yard algorithm found on wikipedia
void intoPost(char* input, Node* &top, Node* &front, Node* &end) { 
    for (int c = 0; input[c] != '\0'; c++) {
        char i = input[c];
        if (i != ' ') {
        	// push if operator into stack 
            if (isOperator(i)) {
                if (top != NULL) {
                    // orders by the precedence of the operators
                    while (top != NULL && getPrecedence(top->data) > getPrecedence(i) || getPrecedence(top->data) == getPrecedence(i) && top->data != '(') {
                        enqueue(top->data, front, end);
                        pop(top);
                        if (top == NULL) {
                            break;
                        }
                    }
                }
                push(i, top);
            }
            // pushes left parenthesis into the stack
            else if (i == '(') {
                push(i, top);
            }
            else if (i == ')') {
            	// empties into stack until left paranthesis is found
                while (top->data != '(') {
                    enqueue(top->data, front, end);
                    pop(top);
                }
                if (top->data == '(') {
                    pop(top);
                } 
            }
            //numbers are put into the queue
            else {
                enqueue(i, front, end);
            }
        }
    }
    // push anything left to the queue
    while (top != NULL) {
        enqueue(top->data, front, end);
        pop(top);
    }

}


//add to the queue
void enqueue(char value, Node* &front, Node* &back) {
	Node *node = new Node();
	node->data = value;

	if (back == NULL) {
		front = node;
		back = node;
	}
	else {
		back->next = node;
		back = node;
	}
}

//removes from queue
void dequeue(Node *&front, Node *&back) {
	if (front == NULL) {
		return;
	}
	Node *temp = front;
	front = front->next;
	if (front == NULL) {
		back == NULL;
	}
	delete temp;
}

//created with help from pranav:
// takes the head of the binary expression tree and prints the postfix expression
void postfix(BTnode* head) {
    if (head) {
        postfix(head->left);
        postfix(head->right);
        cout << head->data << " ";
    }
}
// takes the head of the binary expression tree and prints the prefix expression
void prefix(BTnode* head) {
    if (head) {
        cout << head->data << " ";
        prefix(head->left);
        prefix(head->right);
    }
}
// takes the head of the binary expression tree and prints the infix expression
void infix(BTnode* head) {
    if (head) {
        infix(head->left);
        cout << head->data << " ";
        infix(head->right);
    }
}

// prints the queue while assigning it to an output cstring
void printList(Node* start, char output[], Node* &front, Node* &end) {
    Node* ptr = front;
    int i = 0;
    while (ptr != NULL) {
    	//output data
        output[i] = ptr->data;
        //add space
        output[i + 1] = ' ';
        //skip over
        i += 2;
        //move to next char
        ptr = ptr->next;
        //remove from queue
        dequeue(front, end);
    }
}
    