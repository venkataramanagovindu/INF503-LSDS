#include <chrono>
#include "Queries_AR.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    // Check if a file path and sub-program were provided
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <file_path> <queries_file_path> <length_to_search> <sorted|unsorted>" << endl;
        return 1;
    }

    // Retrieve file path and sub-program from command line arguments
    string filePath = argv[1];
    string queriesFilePath = argv[2];
    // Convert third argument to long long int
    char *endptr;
    long long int LengthToSearch = strtoll(argv[3], &endptr, 10);

    if (*endptr != '\0') {
        cerr << "Invalid number for length to search." << endl;
        return 1;
    }

    string sortOrNot = argv[4];

    Queries_AR *queriesReader = new Queries_AR(filePath, queriesFilePath);

    // Read the genome file
    cout << "Reading the human genome file" << endl;
    queriesReader->ReadFile();
    cout << "Human genome reading completed" << endl
         << endl;
    cout << "Reading the queries file" << endl;
    queriesReader->ReadQueriesFile();
    cout << "Queries file reading completed" << endl
         << endl;

    double time_taken = 0.0;

    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;
    chrono::milliseconds duration;

    long long int fragmentCount = LengthToSearch != 0 && LengthToSearch < queriesReader->totalGenomeLength ? LengthToSearch : queriesReader->totalGenomeLength;

    if (sortOrNot == "unsorted")
    {
        cout << "Searching Started" << endl;

        if (LengthToSearch == 0)
        {
            cout << "Searching entire Subject Dataset " << endl;
        }
        else
        {
            cout << "Searching first " << fragmentCount << endl;
        }

        startTime = chrono::high_resolution_clock::now();

        queriesReader->SearchInGivenLength(fragmentCount, &Queries_AR::SearchInQuery);

        endTime = chrono::high_resolution_clock::now();

        // Calculating total time taken by the program.
        duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

        time_taken = duration.count() / 1000.0;

        if (LengthToSearch == 0)
        {
            cout << "Time taken to search entire Subject Dataset : " << time_taken << " sec" << endl;
        }
        else
        {
            cout << "Time taken to search first " << fragmentCount << " : " << time_taken << " sec" << endl;
        }
    }
    else if (sortOrNot == "sorted")
    {
        cout << "Sorting the Query Dataset" << endl;
        queriesReader->sort();
        cout << "Sorting Completed" << endl
             << endl;

        if (LengthToSearch == 0)
        {
            cout << "Searching entire Subject Dataset " << endl;
        }
        else
        {
            cout << "Searching first " << fragmentCount << endl;
        }

        startTime = chrono::high_resolution_clock::now();

        queriesReader->SearchInGivenLength(fragmentCount, &Queries_AR::binarySearchInQuery);

        endTime = chrono::high_resolution_clock::now();

        duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

        time_taken = duration.count() / 1000.0;

        if (LengthToSearch == 0)
        {
            cout << "Time taken to search entire Subject Dataset : " << fixed << time_taken << " sec" << endl;
        }
        else
        {
            cout << "Time taken to search first " << fragmentCount << " : " << fixed << time_taken << " sec" << endl;
        }
    }

    delete queriesReader;
    return 0;
}
