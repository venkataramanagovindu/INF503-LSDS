#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Queries_HT.h"

int main()
{
    Queries_HT* HT = new Queries_HT(50000);
    string s1 = "TTCTATTCTACAACA\0", s2 = "TTCTATTCTACAACA\0";
    cout << s1.compare(s2) << endl;
    //HT->readFragments("../human_reads_trimmed/human_reads_trimmed.fa");
    HT->readFragments("human_reads_trimmed_medium.fa");
    HT->readHumanGenomes("human_genome_small.txt");
    HT->print();
    HT->search();
    return 0;
}