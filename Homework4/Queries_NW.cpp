#include "Queries_NW.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

Queries_NW::Queries_NW() {
    genomeArray = NULL;
    genomeQueries = NULL;
    cols = 16;
    matchScore = 2;
    misMatchScore = -1;
    gapPenalty = -1;
    NWRows = queriesLength + 1;
    NWCols = queriesLength + 1;



    // Moving initiazing to the constructor
    this->NWMatrix = new int* [NWRows];
    for (int i = 0; i < NWRows; i++) {
        this->NWMatrix[i] = new int[NWCols];
    }

    this->NWMatrix[0][0] = 0;

    // FIll first row with gap penalty
    for (int j = 1; j < NWCols; j++)
        this->NWMatrix[0][j] = this->NWMatrix[0][j - 1] + this->gapPenalty;

    // FIll first col with gap penalty
    for (int i = 1; i < NWRows; i++)
        this->NWMatrix[i][0] = this->NWMatrix[i - 1][0] + this->gapPenalty;
}

void Queries_NW::readFragments(string fragmentFilePath) {
    queriesLineCount = 0;
    string line;

    /* Time function returns the time since the
    Epoch(jan 1 1970). Returned time is in seconds. */
    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);

    /* Creating input filestream */
    ifstream file(fragmentFilePath);
    while (getline(file, line))
        queriesLineCount++;

    file.close();

    /* Creating input filestream */
    ifstream fin(fragmentFilePath);

    this->rows = queriesLineCount / 2;

    this->genomeQueries = new char* [this->rows];
    char c;

    long long int genomeQueryIdx = 0;
    for (long long int i = 0; i < queriesLineCount; i++)
    {
        getline(fin, line);
        if (i % 2 == 0)
            continue;


        this->genomeQueries[genomeQueryIdx] = new char[this->cols];
        int j = 0;
        for (; j < this->cols; j++) {
            this->genomeQueries[genomeQueryIdx][j] = line[j];
        }
        this->genomeQueries[genomeQueryIdx][j] = '\0';
        genomeQueryIdx++;
    }

    time(&end);

    // Calculating total time taken by the program.
    double time_taken = double(end - start);
    cout << "Time taken to read the queries file : " << fixed
        << time_taken;
    cout << " sec " << endl;

    cout << "Numbers of lines in the queries file : " << queriesLineCount << endl;
}

void Queries_NW::readHumanGenomes(string genomeFilePath) {
    // read file char by char
    char ch;
    fstream fin(genomeFilePath, fstream::in);
    char* headerCharArray;

    /* Time function returns the time since the
    Epoch(jan 1 1970). Returned time is in seconds. */
    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);

    // Calculating the size of the file
    fin.seekg(0, std::ios::end);
    long long int size = fin.tellg();
    fin.seekg(0, std::ios::beg);

    // Dynamically allocating memory for the array
    genomeArray = new char[size];
    bool isHeader = false;
    long long int charArridx = 0;
    long long int headerCharidx = 0;
    //long long int genomeLength = 0;

    while (fin >> noskipws >> ch) {

        // 62 = >
        if (ch == 62) {
            isHeader = true;

            /*totalGenomeLength += genomeLength;
            genomeLength = 0;*/
        }

        if (isHeader) {
            if (ch == 10) {
                isHeader = false;
            }
        }
        else if (ch != 10)
        {
            genomeArray[charArridx++] = ch;
            totalGenomeLength++;
        }
    }
    fin.close();

    genomeArray[charArridx] = '\0';

    std::ios_base::sync_with_stdio(false);
    time(&end);

    // Calculating total time taken by the program.
    double time_taken = double(end - start);
    cout << "Time taken to read the genome file : " << fixed
        << time_taken;
    cout << " sec " << endl;
}

long long Queries_NW::fuzzysearchTheQueries(string selectedCommand) {
    
    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);

    hitCount = 0;
    //for (long long int i = 0; i < this->rows; i++) {
    int thresholdScore = ((this->queriesLength - this->allowdMismatchLength) * matchScore) + (this->allowdMismatchLength * misMatchScore);

    for (long long int i = 0; i < this->genomeRangeToSearch; i++) {
        char* randomString = selectedCommand == "RANDOM" ? this->getRandomStringFromSegment() : this->getCompletelyRandomString();

        for (long long int j = 0; j < this->rows; j++)
        {
           int score = this->needlemanWunsch(randomString, this->genomeQueries[j]);

           if (score >= thresholdScore) {
               hitCount++;
               break;
           }
        }

        

        //int score = needlemanWunsch("jjs", "dasdasd");
    }

    std::ios_base::sync_with_stdio(false);
    time(&end);

    // Calculating total time taken by the program.
    double time_taken = double(end - start);
    cout << "Time taken to complete the fuzzy search with "<< selectedCommand <<" : "  << fixed
        << time_taken;
    cout << " sec " << endl;

    return hitCount;
}

char* Queries_NW::getRandomStringFromSegment() {
    long long int startIndex = rand() % (this->totalGenomeLength - this->queriesLength);

    char* randomSubStr = new char[17];

    //string randomSubStr2
        //= genomeArray.substr(1, 2);
     
    strncpy(randomSubStr, this->genomeArray + startIndex, 16);
    randomSubStr[16] = '\0';

    return randomSubStr;
}

char* Queries_NW::getCompletelyRandomString() {
    char* completelyRandomStr = new char[17];

    char genomeChars[6] = { 'A', 'C', 'G', 'T', 'N', '\0'};

    for (int i = 0; i < this->queriesLength; i++)
    {
        int x = rand() % strlen(genomeChars);
        completelyRandomStr[i] = genomeChars[rand() % strlen(genomeChars)];
    }

    completelyRandomStr[this->queriesLength] = '\0';

    return completelyRandomStr;
}

int Queries_NW::needlemanWunsch(char* string1, char* string2) {
    int rows = strlen(string1) + 1;
    int cols = strlen(string2) + 1;

    //cout << rows << endl;
    //cout << cols << endl;

    //int x = strlen(string1);
    //rows = cols = 16 + 1;

    //int** NWMatrix = new int* [rows];
    //for (int i = 0; i < rows; i++) {
    //    NWMatrix[i] = new int[this->cols];
    //}

    //NWMatrix[0][0] = 0;

    //// FIll first row with gap penalty
    //for (int j = 1; j < cols; j++)
    //    NWMatrix[0][j] = NWMatrix[0][j - 1] + this->gapPenalty;

    //// FIll first col with gap penalty
    //for (int i = 1; i < rows; i++)
    //    NWMatrix[i][0] = NWMatrix[i - 1][0] + this->gapPenalty;


    // //  V2
    //this->NWMatrix = new int* [rows];
    //for (int i = 0; i < rows; i++) {
    //    this->NWMatrix[i] = new int[this->cols];
    //}

    //this->NWMatrix[0][0] = 0;

    //// FIll first row with gap penalty
    //for (int j = 1; j < cols; j++)
    //    this->NWMatrix[0][j] = this->NWMatrix[0][j - 1] + this->gapPenalty;

    //// FIll first col with gap penalty
    //for (int i = 1; i < rows; i++)
    //    this->NWMatrix[i][0] = this->NWMatrix[i - 1][0] + this->gapPenalty;

    // Fill the NWMatrix
    for (int i = 1; i < this->NWRows; i++) {
        for (int j = 1; j < this->NWCols; j++) {
            int leftVal = this->NWMatrix[i][j - 1] + this->gapPenalty;
            int rightVal = this->NWMatrix[i - 1][j] + this->gapPenalty;
            int matchScore = string1[i - 1] == string2[j - 1] ? this->matchScore : this->misMatchScore;
            int diagonalval = matchScore + this->NWMatrix[i - 1][j - 1];

            int finalScore = leftVal > rightVal ? 
                            leftVal > diagonalval ? leftVal : diagonalval :
                            rightVal > diagonalval ? rightVal : diagonalval;
            this->NWMatrix[i][j] = finalScore;
        }
    }

    //for (int i = 0; i < rows; i++) {
    //    for (int j = 0; j < cols; j++) {
    //        cout << NWMatrix[i][j] << ' ' << ' ' << ' ' << ' ';
    //    }
    //    cout << endl;

    //}

    return NWMatrix[this->NWRows - 1][this->NWCols - 1];
}

Queries_NW::~Queries_NW() {

}