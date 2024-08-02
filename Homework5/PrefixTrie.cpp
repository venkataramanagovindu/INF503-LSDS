#include "PrefixTrie.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <cstring>

using namespace std;

PrefixTrie::PrefixTrie() {
    trieRoot = makeTrieNode(NULL);
    trieRootWithError = makeTrieNode(NULL);
    queryLength = 36;
    randomQuery = new char[queryLength + 1];
    nodeCount = 0;
    
}

PrefixTrie::PrefixTrie(long long int qSize) : PrefixTrie() {
    this->queriesSize = qSize;
    this->segmentLength = 50;
    //this->segmentLength = 50;

     
    //trieRoot = makeTrieNode(NULL);
}

void PrefixTrie::readHumanGenomes(string genomeFilePath) {
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

void PrefixTrie::getSegmentFromSubject() {
    genomeSubStr = new char[this->segmentLength + 1];

    // TO DO :: this->queryLength can be removed
    long long int startIndex = rand() % (this->totalGenomeLength - this->segmentLength - this->queryLength);

    strncpy(genomeSubStr, genomeArray + startIndex, this->segmentLength);

    genomeSubStr[this->segmentLength] = '\0';
}

void PrefixTrie::buildThePrefixTrie(char select) {
    nodeCount = 0;

    TrieNode* node;

    string s1 = "ACGTN";
    string s2 = "ACGTN";

    char string1[6], string2[6]; // add 1 for null terminator

    strcpy(string1, s1.c_str());
    strcpy(string2, s1.c_str());
    int index = 0;

    // TODO :: Update to number of queries
    for (int j = 0; j < this->queriesSize; j++) {
        randomQuery = getRandomStringFromSegment(select);

        //cout << randomQuery << endl;

        node = select == 'A' ? trieRoot : trieRootWithError;


        for (int i = 0; i < this->queryLength + 1; i++) {

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
                node->children[index] = makeTrieNode(randomQuery + i);

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
    for (int i = 0; i < this->queryLength; i++)
    {
        r = (float)rand() / (float)RAND_MAX;

        if ((float)rand() / (float)RAND_MAX < 0.05) {
            str[i] = this->GenomeChars[rand() % 5];
        }
    }
    return str;
}

void PrefixTrie::count(char select) {
    cout << "Calculating the count" << endl;
    this->countNodesInTire(select == 'A' ? this->trieRoot : this->trieRootWithError);
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
    if (!node)
        return;
    TrieNode* temp = node;
    //printf("%c -> ,  %llu \n", *temp->data, nodeCount);
    nodeCount++;
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
    char* subStr = new char[this->queryLength + 1];
    for (long long int i = 0; i < segmentLength - queryLength; i++)
    {
        strncpy(subStr, this->genomeSubStr + i, this->queryLength);
        subStr[this->queryLength] = '\0';
        //cout << subStr << " Searching for" << endl;
        if (this->fuzzySearch(select == 'A' ? this->trieRoot : this->trieRootWithError, subStr, 0, 0)) {
            searchFoundCount++;
            //cout << "Search found" << endl;
        }
    }

    cout << "Total searches upto 1 miss match " << searchFoundCount << endl;
}


char* PrefixTrie::getRandomStringFromSegment(char select) {
    long long int startIndex = rand() % (this->segmentLength - this->queryLength);

    char* randomSubStr = new char[this->queryLength + 1];

    //string randomSubStr2
        //= genomeArray.substr(1, 2);

    strncpy(randomSubStr, this->genomeSubStr + startIndex, this->queryLength);
    randomSubStr[this->queryLength] = '\0';

    if (select == 'B') {
        randomSubStr = this->generateStringWithError(randomSubStr);
    }


    return randomSubStr;
}

//TrieNode* PrefixTrie::insertIntoTrie(TrieNode node, char c) {
//    int index = 0;
//    switch (c)
//    {
//    case 'A':
//        index = 0;
//        break;
//    case 'C':
//        index = 1;
//        break;
//    case 'G':
//        index = 2;
//        break;
//    case 'T':
//        index = 3;
//        break;
//    default:
//        index = 4;
//        break;
//    }
//
//    if(node.children[index]->data == NULL)
//        node.children[index]->data = c;
//    node.children[index]->count++;
//
//    return node.children[index];
//}

TrieNode* PrefixTrie::makeTrieNode(char* c) {
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
    delete[] genomeArray;
    delete[] genomeSubStr;
    this->deleteNodesInTire(this->trieRoot);
    this->deleteNodesInTire(this->trieRootWithError);
}
