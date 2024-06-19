#ifndef HUMANGENOMEREADER_H
#define HUMANGENOMEREADER_H

#include <string>

using namespace std;

class HumanGenomeReader
{
    private:
        char* HumanGenome;
    public:
        HumanGenomeReader();
        ~HumanGenomeReader();
        string FilePath;
        void ReadFile();
        void AssesGenome();
        void ReplaceTheChar(char ChatToBeReplaced, char CharToBereplacedWith);
};

#endif
