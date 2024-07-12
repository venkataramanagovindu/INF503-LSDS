
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

long long int Queries_HT::getRadixHash(string key) {
	int value = 0, base = 5;
    int queryFragmentSize = 16;
    long long int radix = 0;

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
		radix += value * pow(base, (queryFragmentSize - i - 1));
	}
	return radix % m;
}

void Queries_HT::readFragments(string fragmentFilePath) {
    long long int queriesLineCount = 0;
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

    char c;

    int genomeQueryIdx = 0;
    for (long long int i = 0; i < queriesLineCount; i++)
    {
        getline(fin, line);
        if (i % 2 == 0)
            continue;

        long long int index = this->getRadixHash(line);

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

        char* char_array = new char[17];
        strcpy(char_array, line.c_str());
        char_array[16] = '\0';


            Node* newNode = new Node;
            newNode->data = char_array;
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

void Queries_HT::readHumanGenomes(string genomeFilePath) {
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
    long long size = fin.tellg();
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
    cout << "Human Genome Count " << totalGenomeLength << endl;
    cout << "Time taken to read the genome file : " << fixed
        << time_taken;
    cout << " sec " << endl;
}

void Queries_HT::print() {
    cout << "numberOfCollisions " << this->numberOfCollisions<< endl;
    //for (int i = 0; i < 1; i++)
    //{
    //    if (this->HashTable[i] != NULL) {
    //        //cout << this->HashTable[i]->data << endl;
    //        Node *node = this->HashTable[i];
    //        while (node != NULL)
    //        {
    //            cout << node->data << endl;
    //            node = node->Next;
    //        }       
    //    }
    //}
}

void Queries_HT::search() {
    char substr[17];
    int searchPrintCount = 0;
    for (long long int i = 0; i <= this->totalGenomeLength - this->fragmentLength; i++)
    {
        strncpy(substr, this->genomeArray + i, this->fragmentLength);
        substr[fragmentLength] = '\0';

        //this->findIndex(substr);

        long long int radixIndex = this->getRadixHash(substr);

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
                        cout << node->data << endl;

                    this->numberOfHits++;
                }

                node = node->Next;
            }
        }
    }

    cout << "numberOfHits " << this->numberOfHits << endl;
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


