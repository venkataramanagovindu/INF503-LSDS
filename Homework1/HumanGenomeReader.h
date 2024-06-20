#ifndef HUMANGENOMEREADER_H
#define HUMANGENOMEREADER_H


#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

class HumanGenomeReader
{
    private:
        char* HumanGenome;
        long long int totalGenomeLength;
    public:
        HumanGenomeReader();
        ~HumanGenomeReader();
        string FilePath;
        void ReadFile();
        void AssesGenome();
        void ReplaceTheChar(char ChatToBeReplaced, char CharToBereplacedWith);
};

#endif