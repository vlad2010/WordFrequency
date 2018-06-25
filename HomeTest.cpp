#include <cctype>
#include <stdio.h>
#include <algorithm>

#include "WordStorage.h"
#include "Node.h"

bool readFileAndSaveWords(WordStorage &ws, const char *fileName);
void printNMostCommonWords(WordStorage &ws, int numberOfWordsToPrint);

int main(int argc, char* argv[])
{
	if(argc<2) 
	{
		printf("Usage: wordcounter pathtofile\n");
		return -1;
	}

	const int numberOfWordsToPrint = 20;
	{
		WordStorage ws;
		if(readFileAndSaveWords(ws, argv[1]))
			printNMostCommonWords(ws, numberOfWordsToPrint);
	}
	return 0;
}

bool readFileAndSaveWords(WordStorage &ws, const char *fileName) 
{
	FILE *fp;
	char *buff = NULL;

	const int buffSz = 10485760;
	const int wordBufSize = 128;
	fp = fopen(fileName, "rb");  //open as binary 
	if (fp == NULL) 
	{
		printf("Can't open file %s \n", fileName);
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if(fileSize==0) {
		return true;
	}

	buff = (char*)malloc(buffSz);
	if (buff == NULL) 
	{
		printf("Can't allocate memory\n");
		return false;
	}

	size_t filePos = 0;

	char prevWordBuf[wordBufSize];
	memset(prevWordBuf, 0, sizeof(prevWordBuf));
	while (filePos < fileSize)
	{
		size_t dataBlockPointer = 0;
		size_t wordStart = 0;
		memset(buff, 0, buffSz);  

		size_t readedSz = fread(buff, 1, buffSz, fp);
		char currentWordBuf[wordBufSize];   //word buffer

		while (dataBlockPointer < readedSz)  
		{
			char current_char = buff[dataBlockPointer];

			// only A-Za-z symbols are valid
			// Any other symbol means end of word.
			if (!(((current_char >= 'A') && (current_char <= 'Z')) || ((current_char >= 'a') && (current_char <= 'z'))))
			{
				memset(currentWordBuf, 0, sizeof(currentWordBuf));

				int strBufPos = 0;
				int prevBufBlockSize = 0;
				if (prevWordBuf[0] != 0) {

					//we have previous cycle word to concat
					while (prevWordBuf[strBufPos] != 0)
					{
						currentWordBuf[strBufPos] = tolower(prevWordBuf[strBufPos]);
						strBufPos++;
					}

					prevBufBlockSize = strBufPos;
					memset(prevWordBuf, 0, sizeof(prevWordBuf));  //clear this buffer
				}

				//copy string and transform it to low register
				for (unsigned int j = 0; j < dataBlockPointer - wordStart;j++) 
				{
					currentWordBuf[strBufPos + j] = tolower(buff[wordStart + j]);
				}

				int wordSize = (dataBlockPointer - wordStart) + prevBufBlockSize;
				if (wordSize > 0) 
				{
					ws.addWord(currentWordBuf);
				}
					
				wordStart = dataBlockPointer + 1;
			
			} 
			dataBlockPointer++;
		} 

 	    // copy remains to previous word buffer
		memcpy(prevWordBuf, &buff[wordStart], dataBlockPointer - wordStart);
		filePos += readedSz;
	} 
	
	//still have something in this buff
	if( prevWordBuf[0] != 0)
		ws.addWord(prevWordBuf);	

	fclose(fp);
	return true;
}


void printNMostCommonWords(WordStorage &ws, int numberOfWordsToPrint)
{
	NodePtr *words = new NodePtr[numberOfWordsToPrint];
	int wordsReceived = std::min(ws.getNMostFrequentWords(words, numberOfWordsToPrint), numberOfWordsToPrint) ;

	for (int i = 0; i < wordsReceived; i++) 
	{
		if (words[i] != NULL) 
			printf("%6d %s \n", words[i]->value, words[i]->word);
	}
	delete[]words;
}



