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
#include <string>
#include <stack>
#include "BTnode.h"

using namespace std;


//Node struct needed for infix/postfix conversion
struct Node {
	char data;
	Node* next;
};



//function prototypes
bool isOperator(char c);
int getPrecedence(char c);
void sya(char* input, Node* &top, Node *&front, Node* &back);
void sya_output(Node* start, char output[], Node* &front, Node* &end);

BTnode* constructTree(char output[]);

void push(char value, Node* &top);
void pop(Node* &top);
int peek(Node* &top);

void enqueue(char value, Node* &front, Node* &back);
void dequeue(Node *&front, Node *&back);


void output_infix(tree_node* head);
void output_prefix(tree_node* head);
void output_postfix(tree_node* head);




//--------------------------------------MAIN FUNCTION--------------------------------------



int main() {

	
    Node* top = NULL; //pointer to top of stack
    Node* front = NULL; //pointer to front of queue
    Node* end = NULL; //pointer to rear of queue

	//take input in from user
	char* input = new char();
	
	char output[100];
	memset(output, '\0', 100);

	cout << "Enter a mathmatical expression in infix notation. Use a space between each character." << endl;
	cin.getline(input, 80, '\n');

	sya(input, top, front, end); //convert to postfix notation
	cout << "Inputted expression in postfix notation: " << endl;
	sya_output(front, output, front, end);
	cout << output << endl;

	bool running = true;
    // creates a binary expression tree based on the postfix expression
	BTnode* head = constructTree(output);

	while (running) {
		cout << "\nTo output your inputted expression in infix notation, type \'1\'" << endl;
        cout << "To output your inputted expression in prefix notation, type \'2\'" << endl;
        cout << "To output your inputted expression in postfix notation, type \'3\'" << endl;
        cout << "To quit, type \'4\'" << endl;

        cin.getline(input, 80, '\n');

		if (strcmp(input, "1") == 0) {
            output_infix(head);
        }
        else if (strcmp(input, "2") == 0) {
            output_prefix(head);
        }
        else if (strcmp(input, "3") == 0) {
            output_postfix(head);
        }
        else if (strcmp(input, "4") == 0) {
            cout << "Bye." << endl;
            running = false
        }
        else {
            cout << "You entered something invalid x_x bye.\n";
            running = false;
        }

	}

    return 0;
}	




//----------------------------------------FUNCTIONS----------------------------------------



//function that returns whether or not passed char is a mathematical operator
bool isOperator(char c) {
  if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
        return true;
    }
    else { 
        return false; 
    }
}




//returns the precedence of each operator according to order of operations
int getPrecedence(char c) {
    if (c == '^') {
        return 4; //highest precedence
    }
    else if (c == '*' || c == '/') {
        return 3;
    }
    else if (c == '+' || c == '-') {
        return 2; //lowest precedence
    }
    else {
        return 0;
    }
}




//function to convert infix notation to postfix notation using shunting yard algorithm
void sya(char* input, Node* &top, Node* &front, Node* &end) {

    for (int c = 0; input[c] != '\0'; c++) {
        char i = input[c];

        if (i != ' ') {
            if (isOperator(i) == true) { //push onto stack if character is an operator
                if (top != NULL) {
                    //order by precedence of operators
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
            
            else if (i == '(') {
                push(i, top); //push '(' onto stack
            }

            else if (i == ')') {
                while (top->data != '(') { //find paired '(' 
                    enqueue(top->data, front, end);
                    pop(top);
                }
                if (top->data == '(') {
                    pop(top);
                } 
            }

            else {
                enqueue(i, front, end); //add numbers to queue
            }
        }
    }

    while (top != NULL) {
        enqueue(top->data, front, end);
        pop(top);
    }

    return;

}






//function to build binary expression tree based on postfix notation
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
    
    return head; //return head of binary expression tree
}









//function to add element to top of stack
void push(char value, Node* &top) {
	Node* node = new Node();
	node->data = value;
	node->next = top;
	top = node;
}



//function to remove element at top of stack
void pop(Node* &top) {
	Node* temp = top;
	if(temp == NULL) {
		cout << "Stack is empty." << endl;
	}
	else {
        top = top->next; //make the node under top the new top
        delete temp; //delete top
	}
}



//returns top of stack
int peek(Node* &top) {
	return top->data;
}








//function to add element to queuee
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



//function to remove element from queue
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








//function to output expression in infix notation using binary expression tree
void output_infix(BTnode* head) {
    if (head) {
        output_infix(head->left);
        cout << head->data << " ";
        output_infix(head->right);
    }

    return;
}



//function to output expression in prefix notation using binary expression tree
void output_prefix(BTnode* head) {
    if (head) {
        cout << head->data << " ";
        output_prefix(head->left);
        output_prefix(head->right);
    }

    return;
}



// takes the head of the binary expression tree and prints the postfix expression
void output_postfix(BTnode* head) {
    if (head) {
        output_postfix(head->left);
        output_postfix(head->right);
        cout << head->data << " ";
    }
}





//puts queue to an output cstring
void sya_output(Node* start, char output[], Node* &front, Node* &end) {
    Node* n = front;
    int i = 0;
    while (n != NULL) {
        output[i] = n->data;
        output[i + 1] = ' '; //add space
        i += 2;
        n = n->next; //move to next character
        dequeue(front, end); //remove from queue
    }
}
    