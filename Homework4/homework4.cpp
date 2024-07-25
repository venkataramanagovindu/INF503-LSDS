// HumanGenomeReaderNWBL.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Queries_NW.h"
#include "Queries_BL.h"
#include <cstring>


int main(int argc, const char* argv[])
{
    std::cout << "Hello World!\n";

    string genomeSubjectPath = argv[1],
        queryGenomePath = argv[2],
        selectedCommand = argv[3],
        searchAlgo = argv[5];

    long long int hitCount = 0;

    long long int maxSearchRange = stol(argv[4]);

    if (searchAlgo == "NW") {
        

        Queries_NW* NW = new Queries_NW();

        NW->genomeRangeToSearch = maxSearchRange;

        /*    for (int i = 0; i < 5; i++)
                cout << rand() % 180422589383 << " ";
            cout << endl;*/

            //NW->readHumanGenomes("human_genome.txt");
        NW->readHumanGenomes(genomeSubjectPath);
        //"human_genome_verysmall.txt"

        ///*NW->readHumanGenomes("human.txt");*/
        //NW->readHumanGenomes("human_medium.txt");

        NW->readFragments(queryGenomePath);
        //NW->readFragments("human_reads_trimmed_medium.fa");
        //human_reads_trimmed_small.fa

        string s1 = "GTCGACGCAAACAC";
        string s2 = "GTCGACGCAAACAC";

        char string1[15], string2[15]; // add 1 for null terminator

        strcpy(string1, s1.c_str());
        strcpy(string2, s1.c_str());

        //char  string1[] = { 'T','T','C','T','A','T','T','C','T','A','C','A','A','C','A','G', '\0'};
        //char string2[] = { 'T','T','C','T','A','T','T','C','T','A','C','A','A','C','A','G', '\0' };
        //NW->needlemanWunsch(string1, string2);
        cout << "Starting fuzzy search with NW " << selectedCommand << endl;
        //try
        //{
            hitCount = NW->fuzzysearchTheQueries(selectedCommand);
        //}
        //catch (const std::exception er)
        //{
        //    if (1) {

        //    }
        //}
            cout << "Total hits with up to 2 missmatches " << NW->hitCount << endl;
    }
    else
    {
        Queries_BL* BL = new Queries_BL();

        BL->readHumanGenomes(genomeSubjectPath);
        BL->readFragments(queryGenomePath);
        BL->genomeRangeToSearch = maxSearchRange;

        cout << "Starting fuzzy search with BLAST " << selectedCommand << endl;
        hitCount = BL->fuzzysearchTheQueries(selectedCommand);

        cout << "Total hits with up to 2 missmatches " << BL->hitCount << endl;
    }
}
