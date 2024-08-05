// HumanGenomeReaderPrefixTrie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <chrono>        // For high resolution clock and measuring time durations
#include <iostream>
#include "PrefixTrie.h"
#include <iostream>
#include <random>
#include <cstring>

int main(int argc, const char* argv[])
{

    // TO DO :: Uncomment
    srand(static_cast<unsigned int>(time(0)));


    string genomeSubjectPath = argv[1];
    //string noOfFragments = argv[2];
    long long int noOfFragments = stol(argv[2]);

        // char selectedPart = argv[3][0];


    int selectedPart = stoi(argv[3]);

    if(selectedPart > 3 && selectedPart < 1)
    {
        cout << "Please give proper input" << endl;
    }

    PrefixTrie* PT = new PrefixTrie(noOfFragments, genomeSubjectPath);
    //PrefixTrie* PT = new PrefixTrie(2);



    // Variable to store the time taken for searching
    double time_taken = 0.0;

    // Variables to measure the start and end time
    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;
    chrono::milliseconds duration;

    string executingPart = "";

    switch (selectedPart)
    {
    case 1:
        executingPart = "Basic ";
        break;
    case 2:
        executingPart = "Error ";
        break;
    default:
        executingPart = "Both ";
        break;
    }



    PT->ReadFile();
    PT->getSegmentFromSubject();

    if (selectedPart <= 2) {
        
        cout << "Part " << executingPart << " is executing" << endl << endl;
        cout << "Building PrefixTrie Started" << endl;
            startTime = chrono::high_resolution_clock::now();
        PT->buildThePrefixTrie(selectedPart);
            endTime = chrono::high_resolution_clock::now();

                // Calculate the total time taken to read the file
    duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    time_taken = duration.count() / 1000.0; // Convert the duration to seconds



        cout << "PrefixTrie building Completed" << endl << endl;

        cout << "Time taken to build PrefixTrie: " << time_taken << " sec" << endl << endl;


            cout << "Total nodes in the PrefixTrie " << PT->nodeCount << endl << endl;

        cout << "Fuzzy search started" << endl;
        PT->search(selectedPart);
        cout << "Fuzzy search completed" << endl << endl;
        cout << "Completed " << executingPart << endl;
    }

    // Accepting user input as C to run Part A and Part B using same segment
    if (selectedPart == 3) {
        for (int i = 1; i < 3; i++)
        {
            cout << "Part " << executingPart  << " is executing" << endl << endl;
        cout << "Building PrefixTrie Started" << endl;
            startTime = chrono::high_resolution_clock::now();

            PT->buildThePrefixTrie(i);
            endTime = chrono::high_resolution_clock::now();

                    cout << "Time taken to build PrefixTrie: " << time_taken << " sec" << endl << endl;


                    cout << "PrefixTrie building Completed" << endl;

                        cout << "Total nodes in the PrefixTrie " << PT->nodeCount << endl  << endl;

        cout << "Fuzzy search started" << endl;

            PT->search(i);
        cout << "Fuzzy search completed" << endl  << endl;

        cout << "Completed " << executingPart << endl;
            cout << endl << endl;
        }
    }


    
}
