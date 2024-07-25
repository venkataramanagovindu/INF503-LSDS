#include "Queries_BL.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;


Queries_BL::Queries_BL() {
    genomeArray = NULL;
    genomeQueries = NULL;
    cols = 16;
    matchScore = 2;
    misMatchScore = -1;
    gapPenalty = -1;
    NWRows = queriesLength + 1;
    NWCols = queriesLength + 1;
    genomeSubStr = NULL;
    seedSize = 11;


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

void Queries_BL::readFragments(string fragmentFilePath) {
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

void Queries_BL::readHumanGenomes(string genomeFilePath) {
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

long long Queries_BL::fuzzysearchTheQueries(string selectedCommand) {

    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);

    // BL search start

    long long int startIndex = rand() % (this->totalGenomeLength - this->genomeRangeToSearch - this->queriesLength);

    genomeSubStr = new char[this->genomeRangeToSearch + 1];

    // Copy if it's partial random
    if(selectedCommand == "RANDOM")
        strncpy(genomeSubStr, this->genomeArray + startIndex, this->genomeRangeToSearch);

    // complete random here
    else
        genomeSubStr = getCompletelyRandomSubject(this->genomeRangeToSearch);

    genomeSubStr[this->genomeRangeToSearch] = '\0';

    insertIntoHashTable(genomeSubStr);

     BLAST();



    // BL search end

    

    /*
    
    

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
    }*/

    std::ios_base::sync_with_stdio(false);
    time(&end);

    // Calculating total time taken by the program.
    double time_taken = double(end - start);
    cout << "Time taken to complete the fuzzy search with BLAST " << selectedCommand << " : " << fixed
        << time_taken;
    cout << " sec " << endl;

    return hitCount;
    

}

char* Queries_BL::getRandomStringFromSegment() {
    long long int startIndex = rand() % (this->totalGenomeLength - this->queriesLength);

    char* randomSubStr = new char[17];

    //string randomSubStr2
        //= genomeArray.substr(1, 2);

    strncpy(randomSubStr, this->genomeArray + startIndex, 16);
    randomSubStr[16] = '\0';

    return randomSubStr;
}

char* Queries_BL::getCompletelyRandomString() {
    char* completelyRandomStr = new char[17];

    char genomeChars[6] = { 'A', 'C', 'G', 'T', 'N', '\0' };

    for (int i = 0; i < this->queriesLength; i++)
    {
        int x = rand() % strlen(genomeChars);
        completelyRandomStr[i] = genomeChars[rand() % strlen(genomeChars)];
    }

    completelyRandomStr[this->queriesLength] = '\0';

    return completelyRandomStr;
}

int Queries_BL::needlemanWunsch(char* string1, char* string2) {
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

long long int Queries_BL::getRadixHash(string key) {
    int value = 0, base = 5;
    long long int radix = 0;

    for (int i = 0; i < this->seedSize; i++) {
        switch (key[i]) {
        case 'A':
            value = 0;
            break;
        case 'C':
            value = 1;
            break;
        case 'G':
            value = 2;
            break;
        case 'T':
            value = 3;
            break;
        default:
            value = 4;
            break;
        }
        radix += value * pow(base, (this->seedSize - i - 1));
    }
    return radix % this->genomeRangeToSearch;
}

// Function to populate the substring of the genome array
void Queries_BL::insertIntoHashTable(char* substr) {
    // Initializing Hash table
    HashTable = new Node * [this->genomeRangeToSearch];
    hashTableSize = this->genomeRangeToSearch;

    for (long long int index = 0; index < this->genomeRangeToSearch; index++)
        HashTable[index] = NULL;

    for (long long int i = 0; i < this->genomeRangeToSearch - this->seedSize + 1; i++) {

        char* seed = new char[this->seedSize + 1];

        strncpy(seed, substr + i, this->seedSize);

        seed[this->seedSize] = '\0';

        long long int index = this->getRadixHash(seed);


        Node* newNode = new Node;
        newNode->data = seed;
        newNode->index = i;
        newNode->Next = this->HashTable[index];
        this->HashTable[index] = newNode;


    }

    
}

Queries_BL::Node* Queries_BL::searchInHashTable(char* seed) {

    /* Time function returns the time since the
    Epoch(jan 1 1970). Returned time is in seconds. */
    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);


    long long int radixIndex = this->getRadixHash(seed);

    if (this->HashTable[radixIndex] != NULL) {
        Node* node = this->HashTable[radixIndex];
        while (node != NULL)
        {
            if (strcmp(seed, node->data) == 0)
            {
                //cout << node->data.compare(substr) << endl;
                //if (searchPrintCount++ < 10) {
                //    cout << "Index " << i << " " << node->data << endl;

                //}
                //node = node->Next;
                return node;
            }

            node = node->Next;
        }
    }


    time(&end);

    // Calculating total time taken by the program.
    double time_taken = double(end - start);
    //cout << "Time taken to search in the HashTable : " << fixed
    //    << time_taken;
    //cout << " sec " << endl;
    return NULL;
}

char* Queries_BL::getCompletelyRandomSubject(long long int length) {
    char* completelyRandomStr = new char[length + 1];

    char genomeChars[6] = { 'A', 'C', 'G', 'T', 'N', '\0' };

    for (int i = 0; i < length; i++)
    {
        completelyRandomStr[i] = genomeChars[rand() % strlen(genomeChars)];
    }

    completelyRandomStr[length] = '\0';

    return completelyRandomStr;
}


long long int Queries_BL::BLAST() {
    hitCount = 0;
    for (long long int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->queriesLength - this->seedSize + 1; j++) {
            char* seed = new char[this->seedSize + 1];

            strncpy(seed, this->genomeQueries[i] + j, this->seedSize);

            seed[this->seedSize] = '\0';

            Node* data = searchInHashTable(seed);


            if (data != NULL) {
                while (data != NULL){
                    if (strcmp(seed, data->data) == 0) {
                        long long int seedFoundIndex = data->index;
                        char* actualString = new char[this->queriesLength + 1];

                        // Extend the search
                        int thresholdScore = ((this->queriesLength - this->allowdMismatchLength) * matchScore) + (this->allowdMismatchLength * misMatchScore);


                        strncpy(actualString, genomeSubStr + data->index - j, this->queriesLength);

                        actualString[this->queriesLength] = '\0';

                        int score = needlemanWunsch(actualString, genomeQueries[i]);

                        if (score >= thresholdScore) {
                            hitCount++;
                            break;
                        }
                        
                    }
                    data = data->Next;
                }
            }
        }
    }

    return hitCount;
}

Queries_BL::~Queries_BL() {

}
