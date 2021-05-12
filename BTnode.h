//tree_node header file
//class for binary expression tree node with right and left pointer and a data field of type char


#ifndef BTNODE_H
#define BTNODE_H

#include <iostream>

using namespace std;
//BINARY TREE NODE CLASS WITH A LEFT AND RIGHT POINTER.
class BTnode 
{
public:
	BTnode(char value); //constuctor 
	char data;
	BTnode* right;
	BTnode* left;

};

#endif