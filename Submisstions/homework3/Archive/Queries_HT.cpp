#include "Queries_HT.h"

// Constructor to initialize the hash table with a given size
Queries_HT::Queries_HT(long long int size) {
    HashTable = new Node*[size]; // Allocate memory for the hash table
    m = size; // Set the size of the hash table

    // Initialize each entry in the hash table to nullptr
    for (long long int index = 0; index < size; index++)
        HashTable[index] = nullptr;
}

// Parameterized constructor to initialize the hash table with file paths and size
Queries_HT::Queries_HT(string filePath, string queriesFilePath, long long int size) {
    FilePath = filePath;
    QueriesFilePath = queriesFilePath;
    totalGenomeLength = 0;
    HumanGenome = nullptr;
    m = size;
    numberOfCollisions = 0;

    HashTable = new Node*[size]; // Allocate memory for the hash table

    // Initialize each entry in the hash table to nullptr
    for (long long int index = 0; index < size; index++)
        HashTable[index] = nullptr;
}

// Destructor to clean up the allocated memory
Queries_HT::~Queries_HT() {
    // Deallocate the hash table and its contents
    for (long long int i = 0; i < m; i++) {
        Node* node = HashTable[i];
        while (node) {
            Node* temp = node;
            node = node->Next;
            delete[] temp->data; // Deallocate the fragment data
            delete temp; // Deallocate the node
        }
    }
    delete[] HashTable; // Deallocate the hash table
    delete[] HumanGenome; // Deallocate the human genome data
}

// Function to compute the radix hash of a given key
long long int Queries_HT::getRadixHash(string key) {
    int value = 0, base = 5;
    unsigned long long int radix = 0;

    // Compute the radix hash using the base value and key characters
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
    return radix % m; // Return the hash value modulo the table size
}

// Function to read the query fragments from the file and insert them into the hash table
void Queries_HT::readFragments() {
    long long int queriesLineCount = 0;
    string line;

    // Count the number of lines in the queries file
    ifstream file(QueriesFilePath);
    while (getline(file, line))
        queriesLineCount++;
    file.close();

    ifstream fin(QueriesFilePath);
    char* char_array;

    // Read the query fragments and insert them into the hash table
    for (long long int i = 0; i < queriesLineCount; i++) {
        getline(fin, line);
        if (i % 2 == 0)
            continue; // Skip every other line

        long long int index = getRadixHash(line); // Compute the hash index
        if (HashTable[index] != NULL) {
            numberOfCollisions++; // Increment the collision count
        }

        // Allocate memory for the fragment data
        char_array = new char[FRAGMENT_LENGTH + 1];
        strncpy(char_array, line.c_str(), FRAGMENT_LENGTH);
        char_array[FRAGMENT_LENGTH] = '\0';

        // Create a new node and insert it into the hash table
        Node* newNode = new Node;
        newNode->data = char_array;
        newNode->Next = HashTable[index];
        HashTable[index] = newNode;
    }
}

// Function to read the human genome file
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

// Function to search for the query fragments in the human genome
void Queries_HT::searchFragments() {
    char subjectFragment[FRAGMENT_LENGTH + 1];
    long long int searchPrintCount = 0;

    // Iterate over the human genome and search for each fragment
    for (long long int i = 0; i < totalGenomeLength - FRAGMENT_LENGTH + 1; i++) {
        strncpy(subjectFragment, HumanGenome + i, FRAGMENT_LENGTH);
        subjectFragment[FRAGMENT_LENGTH] = '\0';

        long long int radixIndex = getRadixHash(subjectFragment);

        if (HashTable[radixIndex] != NULL) {
            Node* node = searchLinkedList(radixIndex, subjectFragment);

            if (node != NULL) {
                if (searchPrintCount < 10) { // Print up to 10 search results
                    cout << node->data << " at " << i << endl;
                }
                searchPrintCount++;
                numberOfHits++;
            }
        }
    }
}

// Function to search for a fragment in the linked list at the given hash index
Queries_HT::Node* Queries_HT::searchLinkedList(long long int radixIndex, char* substr) {
    if (HashTable[radixIndex] != nullptr) {
        Node* node = HashTable[radixIndex];
        while (node != nullptr) {
            if (strcmp(substr, node->data) == 0) {
                return node;  // Return the node if found
            }
            node = node->Next;
        }
    }
    return nullptr; // Return nullptr if the fragment is not found
}