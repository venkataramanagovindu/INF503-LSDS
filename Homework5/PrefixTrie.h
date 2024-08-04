#pragma once
#include <string>
using namespace std;

// Since we have 5 letters, we need
// 5 children per node
#define trieChildrenCount 5
const int SCAFFOLD_HEADER_LENGTH = 15;
const int QUERY_LENGTH = 36;
const int PRINT_FRAGMENTS = 15;
const int MATCH_SCORE = 2;
const int MISMATCH_SCORE = -1;
const int GAP_PENALTY = -1;


typedef struct TrieNode TrieNode;

struct TrieNode {
	// The Trie Node Structure
	// Each node has N children, starting from the root
	// and a flag to check if it's a leaf node
	char* data; // Storing for printing purposes only
	TrieNode* children[trieChildrenCount];
	int is_leaf;
	int count;
};

class PrefixTrie
{
	private:
		// char* genomeArray;
		long long int totalGenomeLength = 0;
		TrieNode* trieRoot;
		TrieNode* trieRootWithError;
		char* subjectSegment;
		// long long int queriesSize; // Number of queries to be picked from the 50k segment, 5k, 50k etc
		long long int numberOfQueries; // Number of queries to be picked from the 50k segment, 5k, 50k etc
		long long int subjectSegmentLength; // The length of the segment from the Subject Genome - 50K
		int queryLength; // Length of each query - 36
		char* randomQuery;
		long long int nodeCount;
		int charIndex = 0;
		long long int searchFoundCount = 0;
		char GenomeChars[5] = { 'A', 'C', 'G', 'T', 'N' };

		string FilePath;
		char* HumanGenome;

	public:
		PrefixTrie();
		PrefixTrie(long long int queriesSize, string genomeFilePath);
		//void readHumanGenomes(string genomeFilePath);
		void ReadFile();
		void buildThePrefixTrie(char select = 'A');
		void buildThePrefixTrieWithError();
		char* generateStringWithError(char* str);
		TrieNode* makeTrieNode();
		//TrieNode* insertIntoTrie(TrieNode node, char c);
		void getSegmentFromSubject();
		char* getRandomStringFromSegment(char select);
		void countNodesInTire(TrieNode* node);
		bool fuzzySearch(TrieNode* node, char* str, int idx, int mmc);
		void search(char select = 'A');
		void count(char select = 'A');
		void deleteNodesInTire(TrieNode* node);
		~PrefixTrie();
};

