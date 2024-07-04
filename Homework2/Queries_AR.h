#ifndef QUERIES_AR_H
#define QUERIES_AR_H

#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

const int SCAFFOLD_HEADER_LENGTH = 15;
const int QUERIES_LENGTH = 32;
const int PRINT_FRAGMENTS = 15;

class Queries_AR
{
private:
    char *HumanGenome;
    char **QueriesArray;
    long long int QueriesCount;
    string FilePath;
    string QueriesFilePath;

public:
    long long int totalGenomeLength;
    Queries_AR();
    Queries_AR(string filePath, string queriesFilePath);
    ~Queries_AR();
    void ReadFile();
    void ReadQueriesFile();
    void SearchInGivenLength(long long int fragmentCount, int (Queries_AR::*searchMethod)(const char *));
    int SearchInQuery(const char *sub);
    void sort();
    void QuickSort(char **arr, long long int start, long long int end);
    long long int partition(char **arr, long long int start, long long int end);
    int binarySearchInQuery(const char *target);
    int stringCompare(const char *sub, const char *query);
};

#endif
