#include <stdio.h>
#include <string.h>
#include <algorithm>

#include "WordStorage.h"

WordStorage::WordStorage():numberOfNodes(0)
{
	table = new NodePtr[TABLE_SIZE];
	memset(table, 0, sizeof(NodePtr)*TABLE_SIZE);
}

WordStorage::~WordStorage()
{
	for (int i = 0;i < TABLE_SIZE;i++) 
	{
		if (table[i] != NULL) 
		{
			deleteChain(table[i]);
		}
	}

	delete[] table;
}

bool WordStorage::addWord(const char *str) 
{
	int hash = hashFunc(str);
	if (table[hash] == NULL)
	{
		//chain not exist, create new node
		table[hash] = new Node(str, 1);
		numberOfNodes++;
	}
	else 
	{
		// add word to existing chain
		return addToChain(table[hash], str);
	}
	return true;
}

// this return how many pointers were written to input array 
int WordStorage::getNMostFrequentWords(NodePtr *array , int n) 
{
	NodePtr *sortedWordTable = new NodePtr[numberOfNodes];

	//put all node pointers to one array 
	unsigned int pointersCopyed = 0;
	unsigned int sortedTablePos = 0;
	for (int i = 0;i < TABLE_SIZE;i++) 
	{
		if (table[i] != NULL) 
			copyNodesPtrToTable(table[i], sortedTablePos, sortedWordTable, pointersCopyed);
	}

	if(pointersCopyed==0)
		return 0;

	//sort this array
	std::sort(sortedWordTable, sortedWordTable + numberOfNodes, [](NodePtr n1, NodePtr n2) 
	{
		return n1->value > n2->value;
	});

	//in case if we have lass element in our structure than required number of words
	int elementsToCopy = std::min(n, numberOfNodes);
	memcpy(array, sortedWordTable, elementsToCopy*sizeof(NodePtr));

	delete[] sortedWordTable;

	return pointersCopyed;
}

// djb2  simple hash algorithm for strings 
unsigned int WordStorage::hashFunc(const char* str)
{
	unsigned int hash = 5381;   
	int c;
	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash % TABLE_SIZE;
}

bool WordStorage::addToChain(Node* head, const char *str)
{
	if (strcmp(str, head->word) == 0) 
	{
		head->value++;
		return true;
	}

	if (head->next == NULL) 
	{
		head->next = new Node(str, 1);
		numberOfNodes++;
		return true;
	}
	return addToChain(head->next, str);
}

void WordStorage::deleteChain(Node* head) 
{
	if (head->next) 
	{
		deleteChain(head->next);
	}
	delete head;
	return;
}

//copy all nodes from this chain to sortedWordTable recursively
void WordStorage::copyNodesPtrToTable(Node* head,  unsigned int &sortedTablePos, NodePtr * sortedWordTable, unsigned int &ptrCopyed) 
{
	if ((head == NULL) || (numberOfNodes==0))
		return;

	sortedWordTable[sortedTablePos++] = head;
	ptrCopyed++;

	if (head->next != NULL) 
	{
		copyNodesPtrToTable(head->next, sortedTablePos, sortedWordTable,ptrCopyed);
	}
}


