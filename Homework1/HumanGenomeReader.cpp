#include "HumanGenomeReader.h"

HumanGenomeReader::HumanGenomeReader()
{
    HumanGenome = nullptr;
}

HumanGenomeReader::~HumanGenomeReader()
{
    if (HumanGenome != nullptr)
    {
        delete[] HumanGenome;
    }
}

void HumanGenomeReader::ReadFile()
{
    bool isHeader = false;
    totalGenomeLength  = 0;
    long genomeLength = 0;
    char longScaffoldName[15], headerCharArray[15];
    long int scaffoldCount = 0, longScaffoldLength = 0, headerCharidx = 0;
    char ch = ' ';

    ifstream inputFile(FilePath, ios::binary);

    if (!inputFile.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    inputFile.seekg(0, ios::end);
    long long int fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    HumanGenome = new char[fileSize + 1];

    if (HumanGenome == nullptr)
    {
        cerr << "Failed to allocate memory for HumanGenome." << endl;
        inputFile.close();
        return;
    }

    long long int charArridx = 0;

    // Read characters into the HumanGenome array
    for (int i = 0; i < fileSize; ++i)
     {
         ch = inputFile.get();
        // cout << charArridx;
        if (ch == '>')
        {
            isHeader = true;

            if (genomeLength > 0)
            {
                if (genomeLength > longScaffoldLength)
                {
                    longScaffoldLength = genomeLength;
                    strcpy(longScaffoldName, headerCharArray);
                    longScaffoldName[14] = '\0';
                }
                totalGenomeLength += genomeLength;
                genomeLength = 0;
            }
        }

        if (isHeader)
        {
            if (ch == '\n')
            {
                isHeader = false;
                ++scaffoldCount;
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
            if (charArridx < fileSize)
            {
                HumanGenome[charArridx++] = ch;
                genomeLength++;
            }
            else
            {
                cerr << "Attempted to write out of bounds in HumanGenome array." << endl;
                break;
            }
        }
    }

    inputFile.close();
    totalGenomeLength += genomeLength;

    // Null-terminate the HumanGenome array
    HumanGenome[charArridx] = '\0';

    cout << "Scaffold Count: " << scaffoldCount << endl;
    cout << "Longest Scaffold Name: " << longScaffoldName << endl;
    cout << "Longest Scaffold Length: " << longScaffoldLength << endl;
    cout << "Average Scaffold Length: " << (scaffoldCount > 0 ? totalGenomeLength / scaffoldCount : 0) << endl;
    cout << "Total Scaffod Length: " << totalGenomeLength << endl;

}


void HumanGenomeReader::AssesGenome()
{
    long long int index = 0;
    long long int CountOfA = 0;
    long long int CountOfC = 0;
    long long int CountOfG = 0;
    long long int CountOfT = 0;
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

    cout << endl << "Big O notation of the search: O(n) - Linear" << endl;
    cout << endl << "CountOfA: " << CountOfA << endl;
    cout << "CountOfC: " << CountOfC << endl;
    cout << "CountOfG: " << CountOfG << endl;
    cout << "CountOfT: " << CountOfT << endl;

    cout << endl << "Percentage of A: " << ((double)(CountOfA) / totalGenomeLength) * 100 << "%" << endl;
    cout << "Percentage of T: " << ((double)(CountOfT) / totalGenomeLength) * 100 << "%" << endl;

    cout << "Replacing N with A" << endl;
    ReplaceTheChar('N', 'A');
}

void HumanGenomeReader::ReplaceTheChar(char ChatToBeReplaced, char CharToBereplacedWith)
{
    long long int index = 0;
    while (HumanGenome[index] != '\0')
    {
        if(HumanGenome[index] == ChatToBeReplaced )
        {
            HumanGenome[index] = CharToBereplacedWith;
        }
        ++index;
    }
}
