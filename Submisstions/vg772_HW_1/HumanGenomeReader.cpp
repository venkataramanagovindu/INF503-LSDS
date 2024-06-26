#include "HumanGenomeReader.h"

// Constructor initializes the HumanGenome pointer to nullptr
HumanGenomeReader::HumanGenomeReader()
{
    HumanGenome = nullptr;
    totalGenomeLength = 0; // Total length of the genome sequence
}

// Destructor deallocates memory if it was allocated
HumanGenomeReader::~HumanGenomeReader()
{
    if (HumanGenome != nullptr)
    {
        delete[] HumanGenome;
    }
}

void HumanGenomeReader::ReadFile()
{
    bool isGenomeHeader = false;
    long genomeLength = 0; // Length of the current genome sequence
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

void HumanGenomeReader::AssesGenome()
{
    long long int index = 0;
    long long int CountOfA = 0, CountOfC = 0, CountOfG = 0, CountOfT = 0;

    // Count occurrences of each nucleotide
    while (HumanGenome[index] != '\0')
    {
        switch (HumanGenome[index++])
        {
        case 'A':
            ++CountOfA;
            break;
        case 'C':
            ++CountOfC;
            break;
        case 'G':
            ++CountOfG;
            break;
        case 'T':
            ++CountOfT;
            break;
        default:
            break;
        }
    }

    // Output counts and percentages
    cout << endl << "Big O notation of the search: O(n) - Linear" << endl;
    cout << endl << "Count Of A: " << CountOfA << endl;
    cout << "Count Of C: " << CountOfC << endl;
    cout << "Count Of G: " << CountOfG << endl;
    cout << "Count Of T: " << CountOfT << endl;

    cout << endl << "Percentage of A: " << ((double)(CountOfA) / totalGenomeLength) * 100 << "%" << endl;
    cout << "Percentage of T: " << ((double)(CountOfT) / totalGenomeLength) * 100 << "%" << endl;

    // Replace 'N' with 'A' in the genome sequence
    cout << "Replacing N with A" << endl;
    ReplaceTheChar('N', 'A');
}

void HumanGenomeReader::ReplaceTheChar(char ChatToBeReplaced, char CharToBereplacedWith)
{
    long long int index = 0;

    // Replace specified character with another
    while (HumanGenome[index] != '\0')
    {
        if (HumanGenome[index] == ChatToBeReplaced)
        {
            HumanGenome[index] = CharToBereplacedWith;
        }
        ++index;
    }
}
