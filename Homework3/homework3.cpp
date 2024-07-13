#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Queries_HT.h"

int main()
{
    Queries_HT* HT = new Queries_HT("human_small.txt", "human_reads_125_32_small.fa", 10);
    // string s1 = "TTCTATTCTACAACA\0", s2 = "TTCTATTCTACAACA\0";
    // cout << s1.compare(s2) << endl;


    HT->ReadFile();


    HT->readFragments();

    cout << "Printing hashtable  " << endl << endl; 
    HT->printHashTable();
    // HT->readHumanGenomes("human_small.txt");

    HT->print();
    HT->search();

    cout << "Main Completed" << endl;
    return 1;
}