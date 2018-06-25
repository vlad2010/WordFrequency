#ifndef __NODE_HEADER_
#define __NODE_HEADER_


#include <string.h>
#include <stdlib.h> 

class Node 
{
public:
    Node(const char* wordToAdd, const int &value):value(value), next(NULL) 
	{
		word = strdup(wordToAdd);
    }

	~Node() 
	{
		free((void*)word);
	}

public:
    const char* word;
    int value;
    Node *next;
};

typedef Node* NodePtr;

#endif