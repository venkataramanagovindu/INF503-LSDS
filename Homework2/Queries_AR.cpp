#include "Queries_AR.h"

Queries_AR::Queries_AR(/* args */)
{
            LengthsToSearch[0] = 5000;
        LengthsToSearch[1] = 10000;
}

Queries_AR::~Queries_AR()
{
}

void Queries_AR::ReadFile()
{
    bool isGenomeHeader = false;
    long genomeLength = 0;                                                                     // Length of the current genome sequence
    char longestScaffoldName[SCAFFOLD_HEADER_LENGTH], headerCharArray[SCAFFOLD_HEADER_LENGTH]; // Buffers for scaffold names and headers
    long int genomeScfCount = 0, longestScaffoldLength = 0, headerCharidx = 0;
    char ch = ' '; // Character buffer for reading the file

    // Open the file
    ifstream inputFile(FilePath, ios::binary);

    if (!inputFile.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    // Determine the size of the file
    inputFile.seekg(0, ios::end);
    long long int fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    // Allocate memory for the HumanGenome array
    HumanGenome = new char[fileSize + 1];

    if (HumanGenome == nullptr)
    {
        cerr << "Failed to allocate memory for HumanGenome." << endl;
        inputFile.close();
        return;
    }

    long long int charArridx = 0; // Index for HumanGenome array

    // Read characters into the HumanGenome array
    for (long long int i = 0; i < fileSize; ++i)
    {
        ch = inputFile.get();

        // Check for header line starting with '>'
        if (ch == '>')
        {
            isGenomeHeader = true;

            // Update longest scaffold details if current genome length is greater
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

        // If currently reading a header
        if (isGenomeHeader)
        {
            if (ch == '\n')
            {
                isGenomeHeader = false;
                ++genomeScfCount;
                headerCharArray[headerCharidx] = '\0';
                headerCharidx = 0;
            }
            else if (ch != '>' && headerCharidx < 14) // Prevent buffer overflow
            {
                headerCharArray[headerCharidx++] = ch;
            }
        }
        else if (ch != '\n')
        {
            // Read genome data into HumanGenome array
            if (charArridx < fileSize)
            {
                HumanGenome[charArridx++] = ch;
                genomeLength++;
            }
        }
    }

    inputFile.close();
    totalGenomeLength += genomeLength;

    if (genomeLength > longestScaffoldLength)
    {
        longestScaffoldLength = genomeLength;
        strcpy(longestScaffoldName, headerCharArray);
        longestScaffoldName[14] = '\0';
    }

    // Null-terminate the HumanGenome array
    HumanGenome[charArridx] = '\0';

    // Output scaffold details
    cout << "Scaffold Count: " << genomeScfCount << endl;
    cout << "Longest Scaffold Name: " << longestScaffoldName << endl;
    cout << "Longest Scaffold Length: " << longestScaffoldLength << endl;
    cout << "Average Scaffold Length: " << (genomeScfCount > 0 ? totalGenomeLength / genomeScfCount : 0) << endl;
    cout << "Total Scaffold Length: " << totalGenomeLength << endl;
}

void Queries_AR::ReadQueriesFile()
{
    ifstream QueriesFile(QueriesFilePath); // Open the file
    long long int lineCount = 0;
    string line;

    while (getline(QueriesFile, line))
    { // Read the file line by line
        cout << line << endl;
        ++lineCount;
    }
QueriesFile.clear(); // Clear the EOF flag
    QueriesFile.seekg(0, ios::beg);

    lineCount = lineCount / 2;  
    QueriesArray = new char *[lineCount];

    QueriesCount = lineCount;

// getline(QueriesFile, line);
//         cout << line << endl;


    // for(long long int rows = 0; rows < lineCount; rows++)
    // {

    // }
    long long int rowIndex = 0;
    while (getline(QueriesFile, line))
    { // Read the file line by line
        // ++lineCount;
cout << line << endl; 
        if (line[0] != '>')
        {
            QueriesArray[rowIndex] = new char[QUERIES_LENGTH + 1];
            for (int queryChar = 0; queryChar < QUERIES_LENGTH; queryChar++)
            {
                QueriesArray[rowIndex][queryChar] = line[queryChar];
            }
            QueriesArray[rowIndex][QUERIES_LENGTH] = '\0';
            rowIndex++;
            cout << "rowIndex " << rowIndex << endl;


            // QueriesArray[rowIndex] = new char[QUERIES_LENGTH + 1];
            // strcpy(QueriesArray[rowIndex], line.c_str());
            // QueriesArray[rowIndex][QUERIES_LENGTH] = '\0';
            // rowIndex++;
        }
    }

    cout << "rowIndex " << rowIndex << endl;
    cout << "lineCount " << lineCount << endl;
}

void Queries_AR:: Search()
{
    cout << totalGenomeLength << endl;
    for(int i = 0; i <  2; i++)
    {
        cout << "Searching in the first " << LengthsToSearch[i] << endl;
        long long int fargmentCount = LengthsToSearch[i] < totalGenomeLength ? LengthsToSearch[i] : totalGenomeLength;

            time_t start_t, end_t;
            time(&start_t);

            // Assess the genome
            // reader->AssesGenome();

        SearchInGivenLength(fargmentCount);
            time(&end_t);


            // Calculating total time taken by the program.
            double time_taken = double(end_t - start_t);
            cout << "Time taken to read the file : " << fixed
                << time_taken;
            cout << " sec " << endl;

    }
}


void Queries_AR::SearchInGivenLength(long long int fargmentCount)
{
    for(long long int genomeindex = 0; genomeindex < fargmentCount - QUERIES_LENGTH + 1; genomeindex++)
    {
        int numberOfFragmentsFound = 0;
        char genomeFragment[QUERIES_LENGTH + 1];

        for(int i = 0; i < QUERIES_LENGTH; i++)
        {
            genomeFragment[i] = HumanGenome[genomeindex + i];
        }
        genomeFragment[QUERIES_LENGTH] = '\0';

        // cout << "at " << genomeindex << " " <<  genomeFragment << endl;
        if(SearchInQuery(genomeFragment) == 1 && numberOfFragmentsFound < PRINT_FRAGMENTS)
        {
                cout << "found at " <<  genomeindex << " " << genomeFragment << endl;
        }

    }

    // return -1;
}

int Queries_AR::SearchInQuery(char subjectFragment[])
{
        for(long long int queriesIndex = 0; queriesIndex < QueriesCount; queriesIndex++)
        {
            if(strcmp(QueriesArray[queriesIndex], subjectFragment) == 0)
            {
                // cout << "found at " <<  genomeindex << " " << QueriesArray[queriesIndex] << endl;
                // return genomeindex;
                return 1;
            }
            // else
            //     cout << "Not found " <<  genomeFragment << " " << QueriesArray[queriesIndex]  << endl;
        }
    return 0;
}

void Queries_AR::sort() {
    // cout << "Quick Sort 225" << endl;
    QuickSort(QueriesArray, 0, this->QueriesCount - 1 );
}

void Queries_AR::QuickSort(char** arr, long long int start, long long int end) {
    // cout << "Quick Sort 230" << endl;
    if (start < end) {
        long long int p = partition(arr, start, end);

        QuickSort(arr, start, p - 1);
        QuickSort(arr, p + 1, end);
    }
}

// long long int Queries_AR::partition(char** arr, long long int start, long long int end) {
//     cout << "Quick Sort 240" << endl;
//     cout << "start " << start << "end " << end << endl;
//     char* pivot = arr[start];

//     long long int i = start;
//     long long int j = end;
//     while (i < j) {
//         while (strcmp(arr[i], pivot) <= 0) {
//             i++;
//         }
//         while (strcmp(arr[j], pivot) > 0 ) {
//             j--;
//         }

//         if (i < j) {
//             swap(arr[i], arr[j]);
//         }
//     }
//     swap(arr[start], arr[j]);
//     return j;
// }




long long int Queries_AR::partition(char** arr, long long int start, long long int end) {
    char* pivot = arr[start];
    long long int i = start;
    long long int j = end;

    while (i < j) {
        while (i <= end && strcmp(arr[i], pivot) <= 0) {
            i++;
        }
        while (j >= start && strcmp(arr[j], pivot) > 0) {
            j--;
        }

        if (i < j) {
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[start], arr[j]);
    return j;
}
