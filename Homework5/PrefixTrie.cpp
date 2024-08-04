#include "PrefixTrie.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

PrefixTrie::PrefixTrie() {
    trieRoot = makeTrieNode();
    trieRootWithError = makeTrieNode();
    randomQuery = new char[QUERY_LENGTH + 1];
    nodeCount = 0;
    
}

PrefixTrie::PrefixTrie(long long int qSize, string genomeFilePath) : PrefixTrie() {
    this->numberOfQueries = qSize;
    // this->subjectSegmentLength = 50000;
    this->subjectSegmentLength = 50;
    FilePath = genomeFilePath;
     
    //trieRoot = makeTrieNode();
}



// void PrefixTrie::readHumanGenomes(string genomeFilePath) {
//     // read file char by char
//     char ch;
//     fstream fin(genomeFilePath, fstream::in);
//     char* headerCharArray;

//     /* Time function returns the time since the
//     Epoch(jan 1 1970). Returned time is in seconds. */
//     time_t start, end;
//     std::time(&start);
//     std::ios_base::sync_with_stdio(false);

//     // Calculating the size of the file
//     fin.seekg(0, std::ios::end);
//     long long int size = fin.tellg();
//     fin.seekg(0, std::ios::beg);

//     // Dynamically allocating memory for the array
//     genomeArray = new char[size];
//     bool isHeader = false;
//     long long int charArridx = 0;
//     long long int headerCharidx = 0;
//     //long long int genomeLength = 0;

//     while (fin >> noskipws >> ch) {

//         // 62 = >
//         if (ch == 62) {
//             isHeader = true;

//             /*totalGenomeLength += genomeLength;
//             genomeLength = 0;*/
//         }

//         if (isHeader) {
//             if (ch == 10) {
//                 isHeader = false;
//             }
//         }
//         else if (ch != 10)
//         {
//             genomeArray[charArridx++] = ch;
//             totalGenomeLength++;
//         }
//     }
//     fin.close();

//     genomeArray[charArridx] = '\0';

//     std::ios_base::sync_with_stdio(false);
//     time(&end);

//     // Calculating total time taken by the program.
//     double time_taken = double(end - start);
//     cout << "Time taken to read the genome file : " << fixed
//         << time_taken;
//     cout << " sec " << endl;
// }

void PrefixTrie::ReadFile() {
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

    cout << "Total Subject Length " << totalGenomeLength << endl;
}
 


void PrefixTrie::getSegmentFromSubject() {
    subjectSegment = new char[this->subjectSegmentLength + 1];

    // TO DO :: this->QUERY_LENGTH can be removed
    long long int startIndex = rand() % (this->totalGenomeLength - this->subjectSegmentLength);

    cout << "Subject Random Starting Index " << startIndex <<  endl;

    strncpy(subjectSegment, HumanGenome + startIndex, this->subjectSegmentLength);

    subjectSegment[this->subjectSegmentLength] = '\0';
}

void PrefixTrie::buildThePrefixTrie(char select) {
    nodeCount = 1;

    TrieNode* node;

    string s1 = "ACGTN";
    string s2 = "ACGTN";

    char string1[6], string2[6]; // add 1 for null terminator

    strcpy(string1, s1.c_str());
    strcpy(string2, s1.c_str());
    int index = 0;

    // TODO :: Update to number of queries
    for (int j = 0; j < this->numberOfQueries; j++) {
        randomQuery = getRandomStringFromSegment(select);

        //cout << randomQuery << endl;

        node = select == 'A' ? trieRoot : trieRootWithError;


        for (int i = 0; i < QUERY_LENGTH + 1; i++) {

            //node = insertIntoTrie(node, string1[i]);


            switch (randomQuery[i])
            {
            case 'A':
                index = 0;
                break;
            case 'C':
                index = 1;
                break;
            case 'G':
                index = 2;
                break;
            case 'T':
                index = 3;
                break;
            case 'N':
                index = 4;
                break;
            default:
                index = -1;
                break;
            }

            if (index == -1)
            {
                node->is_leaf = 1;
                break;
            }

            if (node->children[index] == NULL)
            {
                nodeCount++;
                node->children[index] = makeTrieNode();
            }   

            // TO DO :: Check if the data exist and replace only if not
            node->children[index]->data = randomQuery + i ;
            node->children[index]->count++;

            node = node->children[index];
        }
    }
    cout << "Completed building the trie" << endl;
}

void PrefixTrie::buildThePrefixTrieWithError() {

}

char* PrefixTrie::generateStringWithError(char* str) {
    float r;
    for (int i = 0; i < QUERY_LENGTH; i++)
    {
        r = (float)rand() / (RAND_MAX + 1.0);

        // cout << "Random float " << (float)rand() << endl;
        // cout << "(float)RAND_MAX " << (float)RAND_MAX << endl;

        // cout << "Error rate random number " << r << endl;

        if (r < 0.05) {
            char temp = str[i];
            str[i] = this->GenomeChars[rand() % 5];

            // cout << "Updated from " << temp << " to " << str[i] << endl;
        }
    }
    return str;
}

void PrefixTrie::count(char select) {
    cout << "Calculating the count" << endl;
    // this->countNodesInTire(select == 'A' ? this->trieRoot : this->trieRootWithError);
    cout << "Total nodes in the trie " << nodeCount << endl;
}

void PrefixTrie::countNodesInTire(TrieNode* node) {
    // Prints the nodes of the trie
    if (!node)
        return;
    TrieNode* temp = node;
    //printf("%c -> ,  %llu \n", *temp->data, nodeCount);
    nodeCount++;
    for (int i = 0; i < trieChildrenCount; i++) {
        countNodesInTire(temp->children[i]);
    }
}


void PrefixTrie::deleteNodesInTire(TrieNode* node) {
    // Prints the nodes of the trie
    if (node ==  NULL)
        return;
    TrieNode* temp = node;
    //printf("%c -> ,  %llu \n", *temp->data, nodeCount);
    // nodeCount++;
    for (int i = 0; i < trieChildrenCount; i++) {
        deleteNodesInTire(temp->children[i]);
        delete temp->children[i];
    }
}

// node, string - string to be searched
// idx - indexof the string, mmc - miss match count
bool PrefixTrie::fuzzySearch(TrieNode* node, char* str, int idx, int mmc) {
    if (node == NULL)
        return false;
    if (mmc >= 2)
        return false;
    if (node != NULL&& node->is_leaf > 0 && str[idx] == '\0')
        return true;

    
    switch (str[idx])
    {
    case 'A':
        charIndex = 0;
        break;
    case 'C':
        charIndex = 1;
        break;
    case 'G':
        charIndex = 2;
        break;
    case 'T':
        charIndex = 3;
        break;
    default:
        charIndex = 4;
        break;
    }

    if (node->children[charIndex] != NULL) 
        return this->fuzzySearch(node->children[charIndex], str, idx + 1, mmc);
    else
        return this->fuzzySearch(node->children[charIndex], str, idx + 1, mmc + 1);

}

void PrefixTrie::search(char select) {
    char* subStr = new char[QUERY_LENGTH + 1];
    for (long long int i = 0; i < subjectSegmentLength - QUERY_LENGTH; i++)
    {
        strncpy(subStr, this->subjectSegment + i, QUERY_LENGTH);
        subStr[QUERY_LENGTH] = '\0';
        //cout << subStr << " Searching for" << endl;
        if (this->fuzzySearch(select == 'A' ? this->trieRoot : this->trieRootWithError, subStr, 0, 0)) {
            searchFoundCount++;
            //cout << "Search found" << endl;
        }
    }

    cout << "Total searches upto 1 miss match " << searchFoundCount << endl;
}


char* PrefixTrie::getRandomStringFromSegment(char select) {
    long long int startIndex = rand() % (this->subjectSegmentLength - QUERY_LENGTH);

    cout << "Random string starting index " << startIndex << endl;

    char* randomSubStr = new char[QUERY_LENGTH + 1];

    //string randomSubStr2
        //= genomeArray.substr(1, 2);

    strncpy(randomSubStr, this->subjectSegment + startIndex, QUERY_LENGTH);
    randomSubStr[QUERY_LENGTH] = '\0';

    if (select == 'B') {
        randomSubStr = this->generateStringWithError(randomSubStr);
    }


    return randomSubStr;
}


TrieNode* PrefixTrie::makeTrieNode() {
    // Allocate memory for node
    TrieNode* node = new TrieNode;

    for (int i = 0; i < trieChildrenCount; i++) {
        node->children[i] = NULL;
    }
    node->count = 0;
    node->data = NULL;
    node->is_leaf = 0;
    return node;
}

PrefixTrie::~PrefixTrie() {
    delete[] HumanGenome;
    delete[] subjectSegment;
    this->deleteNodesInTire(this->trieRoot);
    this->deleteNodesInTire(this->trieRootWithError);
}
