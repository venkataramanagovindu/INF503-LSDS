#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Queries_HT.h"

int main(int argc, char **argv)
{

// Check if enough command-line arguments are provided
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <file_path> <queries_file_path> <hash_table_length>" << endl;
        return 1;
    }

    // Retrieve file path and queries file path from command line arguments
    string filePath = argv[1];
    string queriesFilePath = argv[2];

    // Convert the third argument to a long long int
    char *endptr;
    long long int hashTableLength = strtoll(argv[3], &endptr, 10);

    // Check if the conversion was successful
    if (*endptr != '\0') {
        cerr << "Invalid number for length to search." << endl;
        return 1;
    }

    Queries_HT* HT = new Queries_HT(filePath, queriesFilePath, hashTableLength);
    // string s1 = "TTCTATTCTACAACA\0", s2 = "TTCTATTCTACAACA\0";
    // cout << s1.compare(s2) << endl;


    cout << "Human Genome Reading started" <<  endl;
    HT->ReadFile();
    cout << "Human Genome Reading completed" <<  endl;


    cout << "Fragments Reading started" <<  endl;

    HT->readFragments();

    cout << "Fragments Reading completed" <<  endl;


    // cout << "Printing hashtable  " << endl << endl; 
    // HT->printHashTable();
    // HT->readHumanGenomes("human_small.txt");


    HT->print();

        cout << "Searching started" <<  endl;

    HT->search();

        cout << "Searching completed" <<  endl;

    cout << "Main Completed" << endl;
    return 1;
}