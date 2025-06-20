#ifndef Node_type
#define Node_type

#include	"LibStudent.hpp"

typedef LibStudent type;

struct Node{
	type item;
	Node *next;
	Node(type);
};


#endif