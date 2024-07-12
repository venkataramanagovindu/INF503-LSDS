#include <chrono>
#include "Queries_AR.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv)
{
    // Check if enough command-line arguments are provided
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <file_path> <queries_file_path> <length_to_search> <sorted|unsorted>" << endl;
        return 1;
    }

    // Retrieve file path and queries file path from command line arguments
    string filePath = argv[1];
    string queriesFilePath = argv[2];

    // Convert the third argument to a long long int
    char *endptr;
    long long int LengthToSearch = strtoll(argv[3], &endptr, 10);

    // Check if the conversion was successful
    if (*endptr != '\0') {
        cerr << "Invalid number for length to search." << endl;
        return 1;
    }

    // Retrieve the sort option from command line arguments
    string sortOrNot = argv[4];

    // Create a new Queries_AR object
    Queries_AR *queriesReader = new Queries_AR(filePath, queriesFilePath);

    // Read the genome file
    cout << "Reading the human genome file" << endl;
    queriesReader->ReadFile();
    cout << "Human genome reading completed" << endl << endl;

    // Read the queries file
    cout << "Reading the queries file" << endl;
    queriesReader->ReadQueriesFile();
    cout << "Queries file reading completed" << endl << endl;

    // Variable to store the time taken for searching
    double time_taken = 0.0;

    // Variables to measure the start and end time
    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;
    chrono::milliseconds duration;

    // Determine the fragment count to search
    long long int fragmentCount = LengthToSearch != 0 && LengthToSearch < queriesReader->totalGenomeLength ? LengthToSearch : queriesReader->totalGenomeLength;

    // Check if the search should be performed on sorted or unsorted data
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

        // Record the start time
        startTime = chrono::high_resolution_clock::now();

        // Perform the search on unsorted data
        queriesReader->SearchInGivenLength(fragmentCount, &Queries_AR::SearchInQuery);

        // Record the end time
        endTime = chrono::high_resolution_clock::now();

        // Calculate the total time taken by the search
        duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

        // Convert the duration to seconds
        time_taken = duration.count() / 1000.0;

        // Display the time taken for the search
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
        // Sort the queries dataset
        cout << "Sorting the Query Dataset" << endl;
        queriesReader->sort();
        cout << "Sorting Completed" << endl << endl;

        if (LengthToSearch == 0)
        {
            cout << "Searching entire Subject Dataset " << endl;
        }
        else
        {
            cout << "Searching first " << fragmentCount << endl;
        }

        // Record the start time
        startTime = chrono::high_resolution_clock::now();

        // Perform the search on sorted data
        queriesReader->SearchInGivenLength(fragmentCount, &Queries_AR::binarySearchInQuery);

        // Record the end time
        endTime = chrono::high_resolution_clock::now();

        // Calculate the total time taken by the search
        duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

        // Convert the duration to seconds
        time_taken = duration.count() / 1000.0;

        // Display the time taken for the search
        if (LengthToSearch == 0)
        {
            cout << "Time taken to search entire Subject Dataset : " << fixed << time_taken << " sec" << endl;
        }
        else
        {
            cout << "Time taken to search first " << fragmentCount << " : " << fixed << time_taken << " sec" << endl;
        }
    }

    // Clean up the Queries_AR object
    delete queriesReader;
    return 0;
}
