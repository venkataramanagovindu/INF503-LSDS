#pragma once
#include <string>
using namespace std;

class Queries_BL
{
	private:
		char* genomeArray;
		long long int totalGenomeLength = 0;
		long long int queriesLineCount = 0;
		int cols;
		int matchScore = 0;
		int misMatchScore = 0;
		int gapPenalty = 0;
		int queriesLength = 16;
		int allowdMismatchLength = 2;
		char* genomeSubStr;
		

	public:
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
		long long int rows;
		char** genomeQueries;
		long long int hitCount = 0;
		long long int genomeRangeToSearch = 0;
		int** NWMatrix = NULL;
		int NWRows = 0;
		int NWCols = 0;
		Queries_BL();
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

