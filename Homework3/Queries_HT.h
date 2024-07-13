
#ifndef QUERIES_HT_H
#define QUERIES_HT_H

#include <string>
#include <stdio.h>
using namespace std;

const int SCAFFOLD_HEADER_LENGTH = 15;
const int QUERIES_LENGTH = 32;
const int PRINT_FRAGMENTS = 15;

class Queries_HT
{
	private:
		// char* genomeArray;
            char *HumanGenome;
		long long int totalGenomeLength = 0;
		long long int m = 0;
		long long int numberOfCollisions = 0;
		int fragmentLength = 32;
		long long int numberOfHits = 0;
        string FilePath;
        string QueriesFilePath;

	public:
		struct Node {
			char *data;
			Node *Next;
		};

		Node** HashTable;	
		Queries_HT(long long int size);
        Queries_HT(string filePath, string queriesFilePath, long long int size);

		long long int getRadixHash(string key);
		void readFragments();
        void ReadFile();



		// void readHumanGenomes(string genomeFilePath);
		void print();
		void insert(string key);
		void search();
        void printHashTable();
		long long int findIndex(string subStr);
		~Queries_HT();
};

#endif


 