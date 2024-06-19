#include "HumanGenomeReader.h"
#include <iostream>
#include <fstream>

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

// void HumanGenomeReader::ReadFile()
// {
//     bool isHeader = false;
//     long long int totalGenomeLength = 0;
//     long genomeLength = 0;
//     char longScaffoldName[14], headerCharArray[14];
//     int scaffoldCount = 0, longScaffoldLength = 0, headerCharidx = 0;
//     char* HumanGenome;
//     char ch = ' ';

//     ifstream inputFile(FilePath, ios::binary);

//     if (!inputFile.is_open())
//     {
//         cerr << "Failed to open the file." << endl;
//         return;
//     }

//     inputFile.seekg(0, ios::end);
//     streamsize fileSize = inputFile.tellg();
//     inputFile.seekg(0, ios::beg);

//     cout << fileSize;

//     HumanGenome = new char[fileSize + 1];

//     cout << HumanGenome[0] ;

//     // if (inputFile.read(HumanGenome, fileSize)) {
//     //     HumanGenome[fileSize] = '\0';
//     //     cout << HumanGenome << endl;
//     // } else {
//     //     cerr << "Error reading the file." << endl;
//     //     delete[] HumanGenome;
//     //     HumanGenome = nullptr;
//     // }

//     // Read characters into the HumanGenome array
//     for (int charArridx = 0; charArridx < fileSize; ++charArridx)
//     {
//         ch = inputFile.get();
//         // cout << "char" << ch << '\n';
//         // 62 is > >
//         if (ch == 62) {
//             isHeader = true;

//             /*if (genomeLength != 0) {
//                 cout << "Length of scaffold " << headerCharArray << "\n" << genomeLength << "\n";
//             }*/

//             if (genomeLength > longScaffoldLength) {
//                 longScaffoldLength = genomeLength;
//                 strcpy(longScaffoldName, headerCharArray);
//             }

//             totalGenomeLength += genomeLength;
//             genomeLength = 0;
//         }

//         if (isHeader) {
//             if (ch == 10) {
//                 isHeader = false;
//                 ++scaffoldCount;
//                 headerCharArray[headerCharidx] = '\0';
//                 //std::cout << strlen(headerCharArray);
//                 headerCharidx = 0;
//             }
//             else
//             {
//                 headerCharArray[headerCharidx++] = ch;
//             }
//         }
//         else if(ch != 10)
//         {
//             HumanGenome[charArridx++] = ch;
//             genomeLength++;
//         }
//     }

//     cout << HumanGenome[0] << endl;
//     HumanGenome[fileSize] = '\0'; // Null-terminate the array

//     cout << "Came out";

//     inputFile.close();
// }

void HumanGenomeReader::ReadFile()
{
    bool isHeader = false;
    long long totalGenomeLength = 0;
    long genomeLength = 0;
    char longScaffoldName[15], headerCharArray[15];
    int scaffoldCount = 0, longScaffoldLength = 0, headerCharidx = 0;
    char ch = ' ';

    ifstream inputFile(FilePath, ios::binary);

    if (!inputFile.is_open())
    {
        cerr << "Failed to open the file." << endl;
        return;
    }

    inputFile.seekg(0, ios::end);
    streamsize fileSize = inputFile.tellg();
    inputFile.seekg(0, ios::beg);

    this->HumanGenome = new char[fileSize + 1];
    // this->HumanGenome[0] = 'a';

    cout << this->HumanGenome[0];

    int charArridx = 0;

    // Read characters into the HumanGenome array
    for (int i = 0; i < fileSize; ++i)
    {
        ch = inputFile.get();

        if (ch == '>')
        {
            isHeader = true;

            if (genomeLength > 0)
            {
                if (genomeLength > longScaffoldLength)
                {
                    longScaffoldLength = genomeLength;
                    strcpy(longScaffoldName, headerCharArray);
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
            this->HumanGenome[charArridx] = ch;
            charArridx++;
            ++genomeLength;
        }
    }

    // Null-terminate the HumanGenome array
    this->HumanGenome[charArridx] = '\0';

    // cout << this->HumanGenome << '\n';

    cout << "Long Scoffold Name " << longScaffoldName << '\n';
    cout << "longScaffoldLength " << longScaffoldLength << '\n';
    cout << "scaffoldCount " << scaffoldCount << '\n';
    cout << "Avg Scaffold " << totalGenomeLength / scaffoldCount << '\n';

    inputFile.close();
}

void HumanGenomeReader::AssesGenome()
{
    long long int index = 0;
    long long int CountOfA = 0;
    long long int CountOfC = 0;
    long long int CountOfG = 0;
    long long int CountOfT = 0;
    while (this->HumanGenome[index] != '\0')
    {
        // cout << this->HumanGenome[index++] << '\n';
        switch (this->HumanGenome[index++])
        {
        case 'A':
            CountOfA++;
            break;
        case 'C':
            CountOfC++;
            break;
        case 'G':
            CountOfG++;
            break;
        case 'T':
            CountOfT++;
            break;

        default:
            break;
        }
    }

    cout << "CountOfA " << CountOfA << '\n';
    cout << "CountOfC " << CountOfC << '\n';
    cout << "CountOfG " << CountOfG << '\n';
    cout << "CountOfT " << CountOfT << '\n';
    this->ReplaceTheChar('N', 'A');
}

void HumanGenomeReader::ReplaceTheChar(char ChatToBeReplaced, char CharToBereplacedWith)
{
    // cout << this->HumanGenome << '\n';
    long long int index = 0;
    while (this->HumanGenome[index] != '\0')
    {
        if(this->HumanGenome[index] == ChatToBeReplaced )
        {
            this->HumanGenome[index] = CharToBereplacedWith;
        }
        ++index;
    }
    // cout << this->HumanGenome << '\n';
}

