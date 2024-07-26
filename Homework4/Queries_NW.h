#pragma once
#include <string>
using namespace std;


class Queries_NW
{
private:
    char *HumanGenome;
    char **QueriesArray;
    long long int QueriesCount;
    string FilePath;
    string QueriesFilePath;

    int** NWMatrix = NULL;
    int NWRows = 0;
    int NWCols = 0;

public:
    long long int totalGenomeLength = 0;
    long long int queriesLineCount = 0;
    long long int rows = 0;
    long long int hitCount = 0;
    long long int genomeRangeToSearch = 0;
    int queriesLength = 32;
    int allowdMismatchLength = 2;

    Queries_NW();
    Queries_NW(string genomeFilePath, string queriesFilePath);
    ~Queries_NW();

    void ReadFile();
    void ReadQueriesFile();
    void readFragments(string fragmentFilePath);
    void readHumanGenomes(string genomeFilePath);
    int needlemanWunsch(char* string1, char* string2);
    long long int fuzzysearchTheQueries(string selectedCommand);
    char* getRandomStringFromSegment();
    char* getCompletelyRandomString();
};
