#include "Queries_HT.h"

Queries_HT::Queries_HT(long long int size) {
    HashTable = new Node*[size];
    m = size;

    for (long long int index = 0; index < size; index++)
        HashTable[index] = nullptr;
}

// Parameterized constructor for Queries_HT
Queries_HT::Queries_HT(string filePath, string queriesFilePath, long long int size)
{
    FilePath = filePath;
    QueriesFilePath = queriesFilePath;
    totalGenomeLength = 0;
    HumanGenome = nullptr;
    m = size;
    numberOfCollisions = 0;

    HashTable = new Node*[size];

    for (long long int index = 0; index < size; index++)
        HashTable[index] = nullptr;
}

Queries_HT::~Queries_HT() {
    // Deallocate the HashTable
    for (long long int i = 0; i < m; i++) {
        Node* node = HashTable[i];
        while (node) {
            Node* temp = node;
            node = node->Next;
            delete[] temp->data;
            delete temp;
        }
    }
    delete[] HashTable;
    delete[] HumanGenome;
}

long long int Queries_HT::getRadixHash(string key) {
    int value = 0, base = 5;
    unsigned long long int radix = 0;

    for (int i = 0; i < FRAGMENT_LENGTH; i++) {
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
        radix += value * pow(base, ((FRAGMENT_LENGTH - i - 1) / 2));
    }
    return radix % m;
}

void Queries_HT::readFragments() {
    long long int queriesLineCount = 0;
    string line;

    ifstream file(QueriesFilePath);
    while (getline(file, line))
        queriesLineCount++;

    file.close();

    ifstream fin(QueriesFilePath);
    char* char_array;

    for (long long int i = 0; i < queriesLineCount; i++) {
        getline(fin, line);
        if (i % 2 == 0)
            continue;

        long long int index = getRadixHash(line);
        if (HashTable[index] != NULL) {
            numberOfCollisions++;
        }

        char_array = new char[FRAGMENT_LENGTH + 1];
        strncpy(char_array, line.c_str(), FRAGMENT_LENGTH);
        char_array[FRAGMENT_LENGTH] = '\0';

        Node* newNode = new Node;
        newNode->data = char_array;
        newNode->Next = HashTable[index];
        HashTable[index] = newNode;
    }
}

// Method to read the human genome file
void Queries_HT::ReadFile() {
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
}

void Queries_HT::searchFragments() {
    char subjectFragment[FRAGMENT_LENGTH + 1];
    long long int searchPrintCount = 0;

    for (long long int i = 0; i < totalGenomeLength - FRAGMENT_LENGTH + 1; i++) {
        strncpy(subjectFragment, HumanGenome + i, FRAGMENT_LENGTH);
        subjectFragment[FRAGMENT_LENGTH] = '\0';

        long long int radixIndex = getRadixHash(subjectFragment);
        // cout << "at " << radixIndex << endl; // Added missing semicolon here

        if (HashTable[radixIndex] != NULL) {


            Node* node = searchLinkedList(radixIndex, subjectFragment);

            if(node != NULL)
            {
                if (searchPrintCount < 10) { // Simplified condition
                        cout << node->data << " at " << i << endl;
                    }
                    searchPrintCount++;
                    numberOfHits++;
            }
        }
    }
}

Queries_HT::Node* Queries_HT::searchLinkedList(long long int radixIndex, char* substr)
{
    if (HashTable[radixIndex] != nullptr) {
        Node* node = HashTable[radixIndex];
        while (node != nullptr) {
            if (strcmp(substr, node->data) == 0) {
                return node;  // Return the node if found
            }
            node = node->Next;
        }
    }
    return nullptr;
}