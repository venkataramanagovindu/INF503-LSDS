#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>
#include <cmath>
using namespace std;

class Queries_BL
{
	private:
		int cols;


		char* genomeSubStr;


	    char *HumanGenome;
		char **QueriesArray;
		long long int QueriesCount;
		string FilePath;
		string QueriesFilePath;
			

	public:

	    long long int totalGenomeLength = 0;
    long long int queriesLineCount = 0;
    long long int rows = 0;
    long long int hitCount = 0;
    long long int genomeRangeToSearch = 0;
    int allowdMismatchLength = 2;

	    void ReadFile();
    void ReadQueriesFile();


		struct Node {
			char* data;
			int index;
			Node* Next;
		};
		Node** HashTable;
		long long int getRadixHash(string key);
		long long int hashTableSize = 0;
		void insertIntoHashTable(char* substr);
		int seedSize = 11;


		Node* searchInHashTable(char* seed);

		int** NWMatrix = NULL;
		int NWRows = 0;
		int NWCols = 0;
Queries_BL(string genomeFilePath, string queriesFilePath);
		~Queries_BL();
		void readFragments(string fragmentFilePath);
		void readHumanGenomes(string genomeFilePath);
		int needlemanWunsch(char* string1, char* string2);
		long long int BLAST();
		long long int fuzzysearchTheQueries(string selectedCommand);
		char* getCompletelyRandomSubject(long long int length);
		char* getRandomStringFromSegment();
		char* getCompletelyRandomString();
};

