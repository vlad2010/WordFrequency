#ifndef __WORDSTORAGE_HEADER_
#define __WORDSTORAGE_HEADER_

#define TABLE_SIZE 10240

#include "Node.h"

class WordStorage
{
public:
	WordStorage();
	~WordStorage();

	bool addWord(const char *str);
	int getNMostFrequentWords(NodePtr *array, int n);
private:

	NodePtr *table;
	unsigned int hashFunc(const char* str);
	int numberOfNodes;

	WordStorage(const WordStorage&);  //disable class copying 

	bool addToChain(Node* head, const char *str);
	void deleteChain(Node* head);
	void copyNodesPtrToTable(Node* head, unsigned int & sortedTablePos, NodePtr * sortedWordTable, unsigned int &ptrCopyed);

};

#endif