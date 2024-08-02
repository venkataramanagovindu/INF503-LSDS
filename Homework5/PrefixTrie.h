#pragma once
#include <string>
using namespace std;

// Since we have 5 letters, we need
// 5 children per node
#define trieChildrenCount 5

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
		char* genomeArray;
		long long int totalGenomeLength = 0;
		TrieNode* trieRoot;
		TrieNode* trieRootWithError;
		char* genomeSubStr;
		long long int queriesSize; // Number of queries to be picked from the 50k segment, 5k, 50k etc
		long long int segmentLength; // The length of the segment from the Subject Genome - 50K
		int queryLength; // Length of each query - 36
		char* randomQuery;
		long long int nodeCount;
		int charIndex = 0;
		long long int searchFoundCount = 0;
		char GenomeChars[5] = { 'A', 'C', 'G', 'T', 'N' };

	public:
		PrefixTrie();
		PrefixTrie(long long int queriesSize);
		void readHumanGenomes(string genomeFilePath);
		void buildThePrefixTrie(char select = 'A');
		void buildThePrefixTrieWithError();
		char* generateStringWithError(char* str);
		TrieNode* makeTrieNode(char* c);
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

