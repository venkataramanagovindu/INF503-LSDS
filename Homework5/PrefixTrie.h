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
const int BASIC_PREFIX = 1;
const int ERROR_PREFIX = 2;
const int BOTH = 3;


// Trie Node Structure
typedef struct TrieNode TrieNode;

struct TrieNode {
    char* data; // Storing for printing purposes only
    TrieNode* children[trieChildrenCount];
    int is_leaf;
    int count;
};

// Custom Stack Structure
struct Stack {
    TrieNode* node;
    int misMatchCount;
    string followedPath;
    string remainingPath;
    Stack* next;

    Stack(TrieNode* n, int mmc, const string& path, const string& remPath)
        : node(n), misMatchCount(mmc), followedPath(path), remainingPath(remPath), next(nullptr) {}
};

class PrefixTrie
{
private:
    long long int totalGenomeLength = 0;
    TrieNode* trieRoot;
    TrieNode* trieRootWithError;
    char* subjectSegment;
    long long int numberOfQueries;
    long long int subjectSegmentLength;
    int queryLength;
    char* randomQuery;
    int charIndex = 0;
    long long int searchFoundCount = 0;
    char GenomeChars[5] = { 'A', 'C', 'G', 'T', 'N' };

    string FilePath;
    char* HumanGenome;

    // Custom stack for DFS
    Stack* top;

public:
    long long int nodeCount;

    PrefixTrie();
    PrefixTrie(long long int queriesSize, string genomeFilePath);
    void ReadFile();
    void buildThePrefixTrie(int selectedPart);
    void buildThePrefixTrieWithError();
    char* generateStringWithError(char* str);
    TrieNode* makeTrieNode();
    void getSegmentFromSubject();
    char* getRandomStringFromSegment(int selectedPart);
    void countNodesInTire(TrieNode* node);
    bool fuzzySearch(TrieNode* node, char* str, int idx, int mmc);
    void search(int selectedPart);
    void deleteNodesInTire(TrieNode* node);
    void push(TrieNode* node, int misMatchCount, const string& followedPath, const string& remainingPath);
    Stack* pop();
    ~PrefixTrie();
};
