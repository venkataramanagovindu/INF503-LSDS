#ifndef HUMANGENOMEREADER_H
#define HUMANGENOMEREADER_H

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

public:
    long long int totalGenomeLength;
    // TODO:: Move to private
    char **QueriesArray;
    Queries_AR(/* args */);
    Queries_AR(string filePath, string queriesFilePath);
    ~Queries_AR();
    string FilePath;
    string QueriesFilePath;
    long long int QueriesCount;
    void ReadFile();
    void ReadQueriesFile();
    void SearchInGivenLength(long long int fargmentCount, int (Queries_AR::*searchMethod)(const char *));
    int SearchInQuery(const char *sub);
    void sort();
    void QuickSort(char **arr, long long int start, long long int end);
    long long int partition(char **arr, long long int start, long long int end);
    int binarySearchInQuery(const char *target);
};

#endif
