
#include <iostream>
#include <string.h>
#include <fstream>
#include <chrono>
//#include <bits/stdc++.h>

#include "Queries_HT.h"

using namespace std;

Queries_HT::Queries_HT(long long int size) {
	HashTable = new Node*[size];
    m = size;

	for (long long int index = 0; index < size; index++)
		HashTable[index] = NULL;
}

// Parameterized constructor for Queries_AR
Queries_HT::Queries_HT(string filePath, string queriesFilePath, long long int size)
{
    FilePath = filePath;
    QueriesFilePath = queriesFilePath;
    totalGenomeLength = 0;
    // QueriesCount = 0;
    HumanGenome = nullptr;
    // QueriesArray = nullptr;


    	HashTable = new Node*[size];
    m = size;

	for (long long int index = 0; index < size; index++)
		HashTable[index] = NULL;
}


long long int Queries_HT::getRadixHash(string key) {
	int value = 0, base = 5;
    int queryFragmentSize = 32;
    unsigned long long int radix = 0;

	for (int i = 0; i < queryFragmentSize; i++) {
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

        // cout << (queryFragmentSize - i - 1) / 2 << endl;
		radix += value * pow(base, ((queryFragmentSize - i - 1) / 2));
        // cout << "each radix " << radix << endl;
	}
    // cout << "radix " << radix << endl;
	return radix % m;
}

void Queries_HT::readFragments() {
    cout << endl;
    long long int queriesLineCount = 0;
    string line;

    /* Time function returns the time since the
    Epoch(jan 1 1970). Returned time is in seconds. */
    time_t start, end;
    std::time(&start);
    std::ios_base::sync_with_stdio(false);

    /* Creating input filestream */
    ifstream file(QueriesFilePath);
    while (getline(file, line))
        queriesLineCount++;

    cout << "queriesLineCount" << endl;
    cout << queriesLineCount << endl;

    file.close();

    /* Creating input filestream */
    ifstream fin(QueriesFilePath);

    char c;

    int genomeQueryIdx = 0;
    char* char_array;
    for (long long int i = 0; i < queriesLineCount; i++)
    {
        getline(fin, line);
        if (i % 2 == 0)
            continue;

        // cout << line << endl;
        long long int index = this->getRadixHash(line);

        // cout << line << " " << index << endl;

        if (this->HashTable[index] != NULL) {
            this->numberOfCollisions++;
        }
        

        //if ( this->HashTable[index] != NULL)
        //{
        //    Node* newNode = new Node;
        //    newNode->data = line;
        //    newNode->Next = NULL;
        //    this->HashTable[index]->Next = newNode;
        //}
        //else
        //{

        char_array = new char[33];
        strcpy(char_array, line.c_str());
        char_array[32] = '\0';

        


            Node* newNode = new Node;
            newNode->data = char_array;
            // cout << "134 char_array    " << char_array << endl;
            // cout << "135 newNode->data " << newNode->data << endl;

            newNode->Next = this->HashTable[index];
            this->HashTable[index] = newNode;
        //}

        //int j = 0;
        //for (; j < 32; j++) {
        //    this->genomeQueries[genomeQueryIdx][j] = line[j];

        //this->genomeQueries[genomeQueryIdx][j] = '\0';
        //genomeQueryIdx++;
    }

    time(&end);

    // Calculating total time taken by the program.
    double time_taken = double(end - start);
    cout << "Time taken to read the queries file : " << fixed
        << time_taken;
    cout << " sec " << endl;

    cout << "Numbers of lines in the file : " << queriesLineCount << endl;
}

// Method to read the human genome file
void Queries_HT::ReadFile()
{
    bool isGenomeHeader = false;
    long genomeLength = 0;
    char longestScaffoldName[SCAFFOLD_HEADER_LENGTH], headerCharArray[SCAFFOLD_HEADER_LENGTH];
    long int genomeScfCount = 0, longestScaffoldLength = 0, headerCharidx = 0;
    char ch = ' ';

    ifstream inputFile(FilePath, ios::binary);

    // Check if the file opened successfully
    if (!inputFile.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    // Determine the size of the file
    inputFile.seekg(0, ios::end);
    long long int fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    // Allocate memory for HumanGenome
    HumanGenome = new char[fileSize + 1];

    if (HumanGenome == nullptr)
    {
        cerr << "Failed to allocate memory for HumanGenome." << endl;
        inputFile.close();
        return;
    }

    long long int charArridx = 0;

    // Read the file character by character
    for (long long int i = 0; i < fileSize; ++i)
    {
        ch = inputFile.get();

        // If a header line is found, process it
        if (ch == '>')
        {
            isGenomeHeader = true;

            // Update the longest scaffold information if needed
            if (genomeLength > 0)
            {
                if (genomeLength > longestScaffoldLength)
                {
                    longestScaffoldLength = genomeLength;
                    strcpy(longestScaffoldName, headerCharArray);
                    longestScaffoldName[14] = '\0';
                }
                totalGenomeLength += genomeLength;
                genomeLength = 0;
            }
        }

        // Process the header line
        if (isGenomeHeader)
        {
            if (ch == '\n')
            {
                isGenomeHeader = false;
                ++genomeScfCount;
                headerCharArray[headerCharidx] = '\0';
                headerCharidx = 0;
            }
            else if (ch != '>' && headerCharidx < 14)
            {
                headerCharArray[headerCharidx++] = ch;
            }
        }
        else if (ch != '\n')
        {
            if (charArridx < fileSize)
            {
                HumanGenome[charArridx++] = ch;
                genomeLength++;
            }
        }
    }

    inputFile.close();
    totalGenomeLength += genomeLength;

    // Update the longest scaffold information if needed
    if (genomeLength > longestScaffoldLength)
    {
        longestScaffoldLength = genomeLength;
        strcpy(longestScaffoldName, headerCharArray);
        longestScaffoldName[14] = '\0';
    }

    HumanGenome[charArridx] = '\0';
}


void Queries_HT::print() {
    cout << "numberOfCollisions " << this->numberOfCollisions<< endl;
}

void Queries_HT::printHashTable()
{
    for(int i = 0; i < 10; i++)
    {
        if(HashTable[i] != NULL)
        {
            Node* node = this->HashTable[i];

            cout << "at " << i << endl;
            while (node != NULL)
            {
                cout << node->data << endl;
                node = node->Next;
            }
            
        }
    }
}

void Queries_HT::search() {
    char substr[32];
    int searchPrintCount = 0;
    for (long long int i = 0; i <= this->totalGenomeLength - this->fragmentLength; i++)
    {
        strncpy(substr, this->HumanGenome + i, this->fragmentLength);
        substr[fragmentLength] = '\0';

        //this->findIndex(substr);

        long long int radixIndex = this->getRadixHash(substr);

        // cout << substr << "  " << radixIndex << " " << this->HashTable[radixIndex] << endl;

        if (this->HashTable[radixIndex] != NULL) {
            Node* node = this->HashTable[radixIndex];
            while (node != NULL)
            {
                //string s(substr);
                //
                //remove(node->data.begin(), node->data.end(), ' ');
                string s2(node->data);
                //if (node->data == s) {
                //    cout << substr << endl;
                //    cout << node->data << endl;
                //}

                //if (s == "TTCTATTCTACAACAG") {
                //    remove(node->data.begin(), node->data.end(), ' ');
                //    //if (node->data == "TTCTATTCTACAACAG") {
                //        cout << substr << endl;
                //        cout << node->data << endl;
                //    //}
                //        
                //}
                //if (s == node->data)
                //{
                //    //cout << node->data.compare(substr) << endl;
                //    if (searchPrintCount++ < 10) 
                //        cout << node->data << endl;

                //    this->numberOfHits++;
                //}

                if (strcmp(substr, node->data) == 0)
                {
                    //cout << node->data.compare(substr) << endl;
                    if (searchPrintCount++ < 10)
                        cout << node->data << " at " << i << endl;

                    this->numberOfHits++;
                }

                node = node->Next;
            }
        }
    }

    cout << "numberOfHits " << this->numberOfHits << endl;
    
    cout << "Search Completed " << endl;


}

long long int Queries_HT::findIndex(string subStr) {
    long long int radixIndex = this->getRadixHash(subStr);

    if (this->HashTable[radixIndex] != NULL) {
        Node *node = this->HashTable[radixIndex];
        while (node != NULL)
        {
            //if (node->data.compare(subStr) == 0)
            //{
            //    cout << node->data << endl;
            //    this->numberOfHits++;
            //}
            
            node = node->Next;
        }       
    }
    return 0;
}

Queries_HT::~Queries_HT() {
    for (long long int i = 0; i < m; i++)
        delete[] this->HashTable[i];

    delete[] this->HashTable;
}


