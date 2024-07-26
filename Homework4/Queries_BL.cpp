#include "Queries_BL.h"
#include "Common.h"


using namespace std;


Queries_BL::Queries_BL(string genomeFilePath, string queriesFilePath): FilePath(genomeFilePath), QueriesFilePath(queriesFilePath) {
    HumanGenome = NULL;
    QueriesArray = NULL;
    cols = 32;
    NWRows = QUERIES_LENGTH + 1;
    NWCols = QUERIES_LENGTH + 1;
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
        this->NWMatrix[0][j] = this->NWMatrix[0][j - 1] + GAP_PENALTY;

    // FIll first col with gap penalty
    for (int i = 1; i < NWRows; i++)
        this->NWMatrix[i][0] = this->NWMatrix[i - 1][0] + GAP_PENALTY;


}



void Queries_BL::ReadFile() {
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

void Queries_BL::ReadQueriesFile() {
    ifstream QueriesFile(QueriesFilePath);
    long long int lineCount = 0;
    string line;

    // Count the number of lines in the queries file
    while (getline(QueriesFile, line)) {
        ++lineCount;
    }
    QueriesFile.clear();
    QueriesFile.seekg(0, ios::beg);

    // Allocate memory for QueriesArray based on the number of queries
    lineCount = lineCount / 2;

    rows = lineCount;

    QueriesArray = new char*[lineCount];

    if (QueriesArray == nullptr) {
        cerr << "Failed to allocate memory for QueriesArray." << endl;
        return;
    }

    QueriesCount = lineCount;

    long long int rowIndex = 0;
    // Read each query from the file
    while (getline(QueriesFile, line)) {
        if (line[0] != '>') {
            QueriesArray[rowIndex] = new char[QUERIES_LENGTH + 1];
            if (QueriesArray[rowIndex] == nullptr) {
                cerr << "Failed to allocate memory for QueriesArray row." << endl;
                return;
            }
            for (int queryChar = 0; queryChar < QUERIES_LENGTH; queryChar++) {
                QueriesArray[rowIndex][queryChar] = line[queryChar];
            }
            QueriesArray[rowIndex][QUERIES_LENGTH] = '\0';
            rowIndex++;
        }
    }
}


long long Queries_BL::fuzzysearchTheQueries(string selectedCommand) {

    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);

    // BL search start

    long long int startIndex = rand() % (this->totalGenomeLength - this->genomeRangeToSearch - QUERIES_LENGTH);

    genomeSubStr = new char[this->genomeRangeToSearch + 1];

    // Copy if it's partial random
    if(selectedCommand == "RANDOM")
        strncpy(genomeSubStr, this->HumanGenome + startIndex, this->genomeRangeToSearch);

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
    long long int startIndex = rand() % (this->totalGenomeLength - QUERIES_LENGTH);

    char* randomSubStr = new char[QUERIES_LENGTH + 1];

    //string randomSubStr2
        //= HumanGenome.substr(1, 2);

    strncpy(randomSubStr, this->HumanGenome + startIndex, QUERIES_LENGTH);
    randomSubStr[QUERIES_LENGTH] = '\0';

    return randomSubStr;
}

char* Queries_BL::getCompletelyRandomString() {
    char* completelyRandomStr = new char[QUERIES_LENGTH + 1];

    char genomeChars[6] = { 'A', 'C', 'G', 'T', 'N', '\0' };

    for (int i = 0; i < QUERIES_LENGTH; i++)
    {
        completelyRandomStr[i] = genomeChars[rand() % strlen(genomeChars)];
    }

    completelyRandomStr[QUERIES_LENGTH] = '\0';

    return completelyRandomStr;
}

int Queries_BL::needlemanWunsch(char* string1, char* string2) {
    for (int i = 1; i < this->NWRows; i++) {
        for (int j = 1; j < this->NWCols; j++) {
            int leftVal = this->NWMatrix[i][j - 1] + GAP_PENALTY;
            int rightVal = this->NWMatrix[i - 1][j] + GAP_PENALTY;
            int matchScore = string1[i - 1] == string2[j - 1] ? MATCH_SCORE : MISMATCH_SCORE;
            int diagonalval = matchScore + this->NWMatrix[i - 1][j - 1];

            int finalScore = leftVal > rightVal ?
                leftVal > diagonalval ? leftVal : diagonalval :
                rightVal > diagonalval ? rightVal : diagonalval;
            this->NWMatrix[i][j] = finalScore;
        }
    }

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

    double time_taken = double(end - start);

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
        for (int j = 0; j < QUERIES_LENGTH - this->seedSize + 1; j++) {
            char* seed = new char[this->seedSize + 1];

            strncpy(seed, this->QueriesArray[i] + j, this->seedSize);

            seed[this->seedSize] = '\0';

            Node* data = searchInHashTable(seed);


            if (data != NULL) {
                while (data != NULL){
                    if (strcmp(seed, data->data) == 0) {
                        long long int seedFoundIndex = data->index;
                        char* actualString = new char[QUERIES_LENGTH + 1];

                        // Extend the search
                        int thresholdScore = ((QUERIES_LENGTH - this->allowdMismatchLength) * MATCH_SCORE) + (this->allowdMismatchLength * MISMATCH_SCORE);


                        strncpy(actualString, genomeSubStr + data->index - j, QUERIES_LENGTH);

                        actualString[QUERIES_LENGTH] = '\0';

                        int score = needlemanWunsch(actualString, QueriesArray[i]);

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
