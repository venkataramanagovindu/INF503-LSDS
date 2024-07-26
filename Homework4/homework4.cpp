#include <iostream>
#include "Common.h"
#include "Queries_NW.h"
#include "Queries_BL.h"
#include <cstring>


int main(int argc, const char* argv[])
{
    if (argc < 6) {
        std::cerr << "Usage: " << argv[0] << " <genomeSubjectPath> <queryGenomePath> <selectedCommand> <maxSearchRange> <searchAlgo>\n";
        return 1;
    }
    string genomeSubjectPath = argv[1],
        queryGenomePath = argv[2],
        selectedCommand = argv[3],
        searchAlgo = argv[5];

    long long int hitCount = 0;

    long long int maxSearchRange = stol(argv[4]);

    if (searchAlgo == "NW") {

        Queries_NW* NW = new Queries_NW(genomeSubjectPath, queryGenomePath);

        NW->genomeRangeToSearch = maxSearchRange;

        cout << "Human Genome Reading Started" <<  endl;
        NW->ReadFile();
        cout << "Human Genome Reading Completed" <<  endl << endl;

        cout << "Queries Reading Started" <<  endl;

        NW->ReadQueriesFile();
        cout << "Queries Reading Completed" <<  endl << endl; 

        cout << "Starting fuzzy search with NW " << (selectedCommand == "RANDOM" ? "Random " : "Completely Random ")  << endl << endl;

        hitCount = NW->fuzzysearchTheQueries(selectedCommand);

        cout << "Total hits with up to 2 missmatches " << NW->hitCount << endl;
    }
    else
    {
        Queries_BL* BL = new Queries_BL(genomeSubjectPath, queryGenomePath);

        cout << "Human Genome Reading Started" <<  endl;
        BL->ReadFile();
        cout << "Human Genome Reading Completed" <<  endl << endl;

        cout << "Queries Reading Started" <<  endl;
        BL->ReadQueriesFile();
        cout << "Queries Reading Completed" <<  endl <<  endl;

        BL->genomeRangeToSearch = maxSearchRange;

        cout << "Starting the fuzzy search with BLAST " << (selectedCommand == "RANDOM" ? "Random " : "Completely Random ") << " for " << maxSearchRange << endl;
        hitCount = BL->fuzzysearchTheQueries(selectedCommand);

        cout << "Total hits with up to 2 missmatches " << BL->hitCount << endl;
    }
    return 0;
}
