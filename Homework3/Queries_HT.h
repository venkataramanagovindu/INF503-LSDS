#ifndef QUERIES_HT_H
#define QUERIES_HT_H

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>

#define FRAGMENT_LENGTH 32
#define SCAFFOLD_HEADER_LENGTH 100

using namespace std;

class Queries_HT
{
private:
    struct Node {
        char *data;
        Node *Next;
    };

    Node **HashTable;
    char *HumanGenome;
    long long int totalGenomeLength;
    long long int m;
    string FilePath;
    string QueriesFilePath;

public:
    long long int numberOfHits;
    long long int numberOfCollisions;
    Queries_HT(long long int size);
    Queries_HT(string filePath, string queriesFilePath, long long int size);
    ~Queries_HT();

    long long int getRadixHash(string key);
    void readFragments();
    void ReadFile();
    void searchFragments();
	Node* searchLinkedList(long long int radixIndex, char* substr);
};

#endif // QUERIES_HT_H