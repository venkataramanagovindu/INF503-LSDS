#include "Queries_AR.h"

// Default constructor for Queries_AR
Queries_AR::Queries_AR()
{
    totalGenomeLength = 0;
    QueriesCount = 0;
    HumanGenome = nullptr;
    QueriesArray = nullptr;
}

// Parameterized constructor for Queries_AR
Queries_AR::Queries_AR(string filePath, string queriesFilePath)
{
    FilePath = filePath;
    QueriesFilePath = queriesFilePath;
    totalGenomeLength = 0;
    QueriesCount = 0;
    HumanGenome = nullptr;
    QueriesArray = nullptr;
}

// Destructor for Queries_AR
Queries_AR::~Queries_AR()
{
    // Deallocate memory for HumanGenome
    delete[] HumanGenome;
    // Deallocate memory for each query in QueriesArray
    for (long long int i = 0; i < QueriesCount; i++)
    {
        delete[] QueriesArray[i];
    }
    // Deallocate memory for QueriesArray
    delete[] QueriesArray;
}

// Method to read the human genome file
void Queries_AR::ReadFile()
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

// Method to read the queries file
void Queries_AR::ReadQueriesFile()
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

// Method to search in the given length of genome using the specified search method
void Queries_AR::SearchInGivenLength(long long int fragmentCount, int (Queries_AR::*searchMethod)(const char *))
{
    int numberOfFragmentsFound = 0;
    for (long long int genomeindex = 0; genomeindex < fragmentCount - QUERIES_LENGTH + 1; genomeindex++)
    {
        char genomeFragment[QUERIES_LENGTH + 1];

        // Extract a fragment of the genome
        for (int i = 0; i < QUERIES_LENGTH; i++)
        {
            genomeFragment[i] = HumanGenome[genomeindex + i];
        }
        genomeFragment[QUERIES_LENGTH] = '\0';

        // Search for the fragment using the specified method
        if ((this->*searchMethod)(genomeFragment) == 1 && numberOfFragmentsFound < PRINT_FRAGMENTS)
        {
            cout << genomeFragment << " found at index " << genomeindex << endl;
            numberOfFragmentsFound++;
        }
    }
}

// Method to search for a fragment in the queries array using linear search
int Queries_AR::SearchInQuery(const char *subjectFragment)
{
    for (long long int queriesIndex = 0; queriesIndex < QueriesCount; queriesIndex++)
    {
        if (stringCompare(subjectFragment, QueriesArray[queriesIndex]) == 0)
        {
            return 1;
        }
    }
    return -1;
}

// Method to compare two strings of fixed length
int Queries_AR::stringCompare(const char *sub, const char *query)
{
    for (int i = 0; i < QUERIES_LENGTH; i++)
    {
        if (sub[i] < query[i])
        {
            return -1;
        }
        else if (sub[i] > query[i])
        {
            return 1;
        }
    }
    return 0;
}

// Method to search for a fragment in the queries array using binary search
int Queries_AR::binarySearchInQuery(const char *target)
{
    int left = 0;
    int right = QueriesCount - 1;

    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(QueriesArray[mid], target);

        if (cmp == 0)
        {
            return 1;
        }
        else if (cmp < 0)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    return -1;
}

// Method to sort the queries array
void Queries_AR::sort()
{
    QuickSort(QueriesArray, 0, QueriesCount - 1);
}

// Helper method to perform quicksort on the queries array
void Queries_AR::QuickSort(char **arr, long long int start, long long int end)
{
    if (start < end)
    {
        long long int p = partition(arr, start, end);
        QuickSort(arr, start, p - 1);
        QuickSort(arr, p + 1, end);
    }
}

// Helper method to partition the array for quicksort
long long int Queries_AR::partition(char **arr, long long int start, long long int end)
{
    char *pivot = arr[start];
    long long int i = start;
    long long int j = end;

    while (i < j)
    {
        while (i <= end && strcmp(arr[i], pivot) <= 0)
        {
            i++;
        }
        while (j >= start && strcmp(arr[j], pivot) > 0)
        {
            j--;
        }

        if (i < j)
        {
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[start], arr[j]);
    return j;
}