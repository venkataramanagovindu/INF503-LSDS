#include "PrefixTrie.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

PrefixTrie::PrefixTrie() : top(nullptr) {
    trieRoot = makeTrieNode();
    trieRootWithError = makeTrieNode();
    randomQuery = new char[QUERY_LENGTH + 1];
    nodeCount = 0;
}

PrefixTrie::PrefixTrie(long long int qSize, string genomeFilePath) : PrefixTrie() {
    this->numberOfQueries = qSize;
    this->subjectSegmentLength = 50000;
    FilePath = genomeFilePath;
}

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

    inputFile.seekg(0, ios::end);
    long long int fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    HumanGenome = new char[fileSize + 1];
    if (HumanGenome == nullptr) {
        cerr << "Failed to allocate memory for HumanGenome." << endl;
        inputFile.close();
        return;
    }

    cout << "Reading the human genome file..." << endl;

    long long int charArridx = 0;
    for (long long int i = 0; i < fileSize; ++i) {
        ch = inputFile.get();

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
    long long int startIndex = rand() % (this->totalGenomeLength - this->subjectSegmentLength);
    cout << "Subject Random Starting Index " << startIndex << endl;
    strncpy(subjectSegment, HumanGenome + startIndex, this->subjectSegmentLength);
    subjectSegment[this->subjectSegmentLength] = '\0';
}

void PrefixTrie::buildThePrefixTrie(int selectedPart) {
    nodeCount = 1;
    TrieNode* node;
    string s1 = "ACGTN";
    char string1[6], string2[6];
    strcpy(string1, s1.c_str());
    strcpy(string2, s1.c_str());
    int index = 0;

    for (int j = 0; j < this->numberOfQueries; j++) {
        randomQuery = getRandomStringFromSegment(selectedPart);
        node = selectedPart == BASIC_PREFIX ? trieRoot : trieRootWithError;

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

            if (index == -1) {
                node->is_leaf = 1;
                break;
            }

            if (node->children[index] == NULL) {
                nodeCount++;
                node->children[index] = makeTrieNode();
            }   

            // TO DO :: Check if the data exist and replace only if not
            node->children[index]->data = randomQuery + i ;
            node->children[index]->count++;
            node = node->children[index];
        }
    }
}


char* PrefixTrie::generateStringWithError(char* str) {
    float r;
    for (int i = 0; i < QUERY_LENGTH; i++) {
        r = (float)rand() / (RAND_MAX + 1.0);
        if (r < 0.05) {
            char temp = str[i];
            str[i] = this->GenomeChars[rand() % 5];
        }
    }
    return str;
}



void PrefixTrie::countNodesInTire(TrieNode* node) {
    if (!node) return;
    TrieNode* temp = node;
    nodeCount++;
    for (int i = 0; i < trieChildrenCount; i++) {
        countNodesInTire(temp->children[i]);
    }
}

void PrefixTrie::deleteNodesInTire(TrieNode* node) {
    if (node == NULL) return;
    TrieNode* temp = node;
    for (int i = 0; i < trieChildrenCount; i++) {
        deleteNodesInTire(temp->children[i]);
        delete temp->children[i];
    }
}

void PrefixTrie::push(TrieNode* node, int misMatchCount, const string& followedPath, const string& remainingPath) {
    Stack* newNode = new Stack(node, misMatchCount, followedPath, remainingPath);
    newNode->next = top;
    top = newNode;
}

Stack* PrefixTrie::pop() {
    if (top == nullptr) return nullptr;
    Stack* temp = top;
    top = top->next;
    return temp;
}

bool PrefixTrie::fuzzySearch(TrieNode* node, char* str, int idx, int mmc) {
    // Implement the fuzzy search algorithm with the custom stack
    push(node, mmc, "", string(str));
    
    while (top != nullptr) {
        Stack* current = pop();
        TrieNode* currentNode = current->node;
        int mismatchCount = current->misMatchCount;
        string followedPath = current->followedPath;
        string remainingPath = current->remainingPath;

        // cout << current->remainingPath << endl;

        if (remainingPath.empty()) {
            if (currentNode->is_leaf) {
                searchFoundCount++;
            }
            delete current;
            continue;
        }

        char nextChar = remainingPath[0];
        remainingPath = remainingPath.substr(1);
        
        for (int i = 0; i < trieChildrenCount; i++) {
            if (currentNode->children[i]) {
                char childChar = GenomeChars[i];
                if (childChar == nextChar) {
                    push(currentNode->children[i], mismatchCount, followedPath + childChar, remainingPath);
                } else if (mismatchCount + 1 <= 2) {
                    push(currentNode->children[i], mismatchCount + 1, followedPath + childChar, remainingPath);
                }
            }
        }
        delete current;
    }

    return searchFoundCount > 0;
}
void PrefixTrie::search(int selectedPart) {
    char* subStr = new char[QUERY_LENGTH + 1];
    for (long long int i = 0; i < subjectSegmentLength - QUERY_LENGTH; i++)
    {
        strncpy(subStr, this->subjectSegment + i, QUERY_LENGTH);
        subStr[QUERY_LENGTH] = '\0';
        //cout << subStr << " Searching for" << endl;

        this->fuzzySearch(selectedPart == BASIC_PREFIX ? this->trieRoot : this->trieRootWithError, subStr, 0, 0);
    }

    cout << "Total searches upto 1 miss match " << searchFoundCount << endl;
}

char* PrefixTrie::getRandomStringFromSegment(int selectedPart) {
    long long int startIndex = rand() % (this->subjectSegmentLength - QUERY_LENGTH);

    // cout << "Random string starting index " << startIndex << endl;

    char* randomSubStr = new char[QUERY_LENGTH + 1];

    //string randomSubStr2
        //= genomeArray.substr(1, 2);

    strncpy(randomSubStr, this->subjectSegment + startIndex, QUERY_LENGTH);
    randomSubStr[QUERY_LENGTH] = '\0';

    if (selectedPart == ERROR_PREFIX) {
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
    deleteNodesInTire(trieRoot);
    delete[] HumanGenome;
    delete[] randomQuery;
}