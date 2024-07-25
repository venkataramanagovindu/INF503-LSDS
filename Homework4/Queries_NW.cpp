#include "Queries_NW.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

Queries_NW::Queries_NW() {
    HumanGenome = NULL;
    QueriesArray = NULL;
    cols = 32;
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

Queries_NW::Queries_NW(string genomeFilePath, string queriesFilePath) {
    HumanGenome = NULL;
    QueriesArray = NULL;
    cols = 33;
    matchScore = 2;
    misMatchScore = -1;
    gapPenalty = -1;
    NWRows = queriesLength + 1;
    NWCols = queriesLength + 1;

    FilePath = genomeFilePath;
    QueriesFilePath = queriesFilePath;



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

//**** New Code Start **** //


// Function to read the human genome file
void Queries_NW::ReadFile() {
    bool isGenomeHeader = false;
    long genomeLength = 0;
    char longestScaffoldName[SCAFFOLD_HEADER_LENGTH], headerCharArray[SCAFFOLD_HEADER_LENGTH];
    long int genomeScfCount = 0, longestScaffoldLength = 0, headerCharidx = 0;
    char ch = ' ';

    ifstream inputFile(FilePath, ios::binary);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return;
    }

    // Get the size of the file
    inputFile.seekg(0, ios::end);
    long long int fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    // Allocate memory for the human genome data
    HumanGenome = new char[fileSize + 1];
    if (HumanGenome == nullptr) {
        cerr << "Failed to allocate memory for HumanGenome." << endl;
        inputFile.close();
        return;
    }

    long long int charArridx = 0;
    for (long long int i = 0; i < fileSize; ++i) {
        ch = inputFile.get();

        // Check for genome header
        if (ch == '>') {
            isGenomeHeader = true;
            if (genomeLength > 0) {
                if (genomeLength > longestScaffoldLength) {
                    longestScaffoldLength = genomeLength;
                    strcpy(longestScaffoldName, headerCharArray);
                    longestScaffoldName[14] = '\0';
                }
                totalGenomeLength += genomeLength;
                genomeLength = 0;
            }
        }

        if (isGenomeHeader) {
            if (ch == '\n') {
                isGenomeHeader = false;
                ++genomeScfCount;
                headerCharArray[headerCharidx] = '\0';
                headerCharidx = 0;
            } else if (ch != '>' && headerCharidx < SCAFFOLD_HEADER_LENGTH) {
                headerCharArray[headerCharidx++] = ch;
            }
        } else if (ch != '\n') {
            if (charArridx < fileSize) {
                HumanGenome[charArridx++] = ch;
                genomeLength++;
            }
        }
    }

    inputFile.close();
    totalGenomeLength += genomeLength;

    if (genomeLength > longestScaffoldLength) {
        longestScaffoldLength = genomeLength;
        strcpy(longestScaffoldName, headerCharArray);
        longestScaffoldName[14] = '\0';
    }

    HumanGenome[charArridx] = '\0';
}

// Method to read the queries file
void Queries_NW::ReadQueriesFile()
{
    ifstream QueriesFile(QueriesFilePath);
    long long int lineCount = 0;
    string line;

    // Count the number of lines in the queries file
    while (getline(QueriesFile, line))
    {
        ++lineCount;
    }
    QueriesFile.clear();
    QueriesFile.seekg(0, ios::beg);

    // Allocate memory for QueriesArray based on the number of queries
    lineCount = lineCount / 2;

    rows = lineCount;

    QueriesArray = new char *[lineCount];

    if (QueriesArray == nullptr) {
        cerr << "Failed to allocate memory for QueriesArray." << endl;
        return;
    }

    QueriesCount = lineCount;

    long long int rowIndex = 0;
    // Read each query from the file
    while (getline(QueriesFile, line))
    {
        if (line[0] != '>')
        {
            QueriesArray[rowIndex] = new char[QUERIES_LENGTH + 1];
            if (QueriesArray[rowIndex] == nullptr) {
                cerr << "Failed to allocate memory for QueriesArray row." << endl;
                return;
            }
            for (int queryChar = 0; queryChar < QUERIES_LENGTH; queryChar++)
            {
                QueriesArray[rowIndex][queryChar] = line[queryChar];
            }
            QueriesArray[rowIndex][QUERIES_LENGTH] = '\0';
            rowIndex++;
        }
    }
}


//**** New Code Start **** //


long long Queries_NW::fuzzysearchTheQueries(string selectedCommand) {
    
    
    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);

    hitCount = 0;
    //for (long long int i = 0; i < this->rows; i++) {
    int thresholdScore = ((this->queriesLength - this->allowdMismatchLength) * matchScore) + (this->allowdMismatchLength * misMatchScore);
    cout << "Some 322 some" << endl;

    cout << "threshold " << thresholdScore <<  endl; 

    cout << "genomeRangeToSearch " << genomeRangeToSearch <<  endl;


    for (long long int i = 0; i < this->genomeRangeToSearch; i++) {
        char* randomString = selectedCommand == "RANDOM" ? this->getRandomStringFromSegment() : this->getCompletelyRandomString();

        cout << "this->rows " << this->rows <<  endl;
        for (long long int j = 0; j < this->rows; j++)
        {
           int score = this->needlemanWunsch(randomString, this->QueriesArray[j]);

           cout <<  "score " << score <<  endl;

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
    long long int startIndex = rand() % (this->totalGenomeLength - this->queriesLength + 1);

    cout << "totalGenomeLength " << this->totalGenomeLength <<  endl;
    cout << "queriesLength " << this->queriesLength <<  endl;
    cout << "startIndex " << startIndex <<  endl;

    // Ensure randomSubStr can hold 32 characters + the null terminator
    char* randomSubStr = new char[32 + 1];

    // Copy 32 characters from genomeArray starting at startIndex
    strncpy(randomSubStr, this->HumanGenome + startIndex, 32);

    randomSubStr = "ATCATTCTCAACAACTACTTTGTGATGTGTGC";
    // Ensure the string is null-terminated
    randomSubStr[32] = '\0';

    cout << "randomSubStr " << randomSubStr <<  endl;
    return randomSubStr;
}

char* Queries_NW::getCompletelyRandomString() {
    char* completelyRandomStr = new char[this->queriesLength + 1];

    char genomeChars[6] = { 'A', 'C', 'G', 'T', 'N', '\0'};

    for (int i = 0; i < this->queriesLength; i++)
    {
        int x = rand() % strlen(genomeChars);
        cout << "Completely random index " << x << endl;
        completelyRandomStr[i] = genomeChars[rand() % strlen(genomeChars)];
    }

    completelyRandomStr[this->queriesLength] = '\0';

    return completelyRandomStr;
}

int Queries_NW::needlemanWunsch(char* string1, char* string2) {
    int rows = strlen(string1) + 1;
    int cols = strlen(string2) + 1;

    cout << "rows " <<  rows <<  endl; 
    cout << "string1 " <<  string1 <<  endl; 
    
    cout << "cols " <<  cols <<  endl; 
    cout << "string2 " <<  string2 <<  endl; 

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

    cout << "NWRows " << NWRows << endl;
    cout << "NWCols " << NWCols << endl;
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

    // for (int i = 0; i < rows; i++) {
    //    for (int j = 0; j < cols; j++) {
    //        cout << NWMatrix[i][j] << ' ';
    //    }
    //    cout << endl;

    // }


    return NWMatrix[this->NWRows - 1][this->NWCols - 1];
}

Queries_NW::~Queries_NW() {

}