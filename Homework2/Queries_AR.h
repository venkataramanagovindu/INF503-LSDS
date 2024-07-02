#ifndef HUMANGENOMEREADER_H
#define HUMANGENOMEREADER_H

#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

const int SCAFFOLD_HEADER_LENGTH = 15;
const int QUERIES_LENGTH = 32;

class Queries_AR
{
private:
    char *HumanGenome;
    long long int totalGenomeLength;

public:
    long long int LengthsToSearch[2];
    // TODO:: Move to private
    char **QueriesArray;
    Queries_AR(/* args */);
    ~Queries_AR();
    string FilePath;
    string QueriesFilePath;
    long long int QueriesCount;
    void ReadFile();
    void ReadQueriesFile();
    void SearchInGivenLength(long long int fargmentCount);
    void Search();
    int SearchInQuery(char* subjectFragment);
    void sort();
    void QuickSort(char** arr, long long int start, long long int end);
    long long int partition(char** arr, long long int start, long long int end);
};

#endif
