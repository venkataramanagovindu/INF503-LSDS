// HumanGenomeReaderPrefixTrie.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "PrefixTrie.h"
#include <iostream>
#include <random>
#include <cstring>

int main(int argc, const char* argv[])
{

    // TO DO :: Uncomment
    // srand(static_cast<unsigned int>(time(0)));


    //float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    //// Create a random number engine and distribution
    //std::random_device rd;
    //std::default_random_engine eng(rd());
    //std::uniform_real_distribution<double> distr(0.0, 1.0);

    //// Generate a random number between 0 and 1
    //double random_number = distr(eng);

    std::cout << "Hello World!\n";

    string genomeSubjectPath = argv[1];
    //string noOfFragments = argv[2];
    long long int noOfFragments = stol(argv[2]);

    char selectedPart = argv[3][0];

    PrefixTrie* PT = new PrefixTrie(noOfFragments, genomeSubjectPath);
    //PrefixTrie* PT = new PrefixTrie(2);

    PT->ReadFile();
    PT->getSegmentFromSubject();

    if (selectedPart == 'A' || selectedPart == 'B') {
        
        cout << "Executing the Part " << selectedPart << endl;
        cout << "Calling buildThePrefixTrie" << endl;
        PT->buildThePrefixTrie(selectedPart);
        PT->count(selectedPart);
        PT->search(selectedPart);
        cout << "Completed Part " << selectedPart << endl;
    }

    // Accepting user input as C to run Part A and Part B using same segment
    if (selectedPart == 'C') {
        char parts[] = "AB";
        for (int i = 0; i < strlen(parts); i++)
        {
            cout << "Executing the Part " << parts[i] << endl;
            cout << "Calling buildThePrefixTrie" << endl;
            PT->buildThePrefixTrie(parts[i]);
            PT->count(parts[i]);
            PT->search(parts[i]);
            cout << "Completed Part " << parts[i] << endl;
            cout << endl << endl;
        }
    }



    //cout << "Press 'B' to continue with the same segment and 5% error" << endl;

    //

    //cin >> selectedPart;

    //if (selectedPart == 'B') {
    //    cout << "Executing the Part B" << endl;
    //    PT->buildThePrefixTrie('B');
    //    PT->count('B');
    //    PT->search('B');
    //    cout << "Completed Part B" << endl;
    //}

    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

//wsl g++ HumanGenomeReaderPrefixTrie.cpp PrefixTrie.cpp
//wsl ./a.out human_medium.txt

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
