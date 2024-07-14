#include <chrono>

#include "Queries_HT.h"

using namespace std;
using namespace std::chrono;


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
        cerr << "Invalid number for hash table length." << endl;
        return 1;
    }

    // Variable to store the time taken for searching
    double time_taken = 0.0;

    // Variables to measure the start and end time
    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;
    chrono::milliseconds duration;

    Queries_HT* HT = new Queries_HT(filePath, queriesFilePath, hashTableLength);

    cout << "Human Genome Reading started" <<  endl;
        startTime = chrono::high_resolution_clock::now();

    HT->ReadFile();

        endTime = chrono::high_resolution_clock::now();

                // Calculate the total time taken by the search
        duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

        // Convert the duration to seconds
        time_taken = duration.count() / 1000.0;



    cout << "Human Genome Reading completed" <<  endl;

    cout << "Time taken to Read Subject Dataset : " << time_taken << " sec" << endl  <<  endl;

    cout << "Query Dataset Reading started" <<  endl;

        startTime = chrono::high_resolution_clock::now();

    HT->readFragments();

        endTime = chrono::high_resolution_clock::now();


                    // Calculate the total time taken by the search
        duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    cout << "Query Dataset Reading completed" <<  endl;
        // Convert the duration to seconds
        time_taken = duration.count() / 1000.0;

    cout << "Time taken to populate Hashtable of size " << hashTableLength <<  " : " << time_taken << " sec" << endl  << endl;



    cout << "Total number of Collisions: " << HT->numberOfCollisions << endl << endl;


    cout << "Searching started" <<  endl;



        startTime = chrono::high_resolution_clock::now();

    HT->searchFragments();

        endTime = chrono::high_resolution_clock::now();

    cout << "Searching completed" <<  endl << endl;

    cout << "Total number of Occurances: " << HT->numberOfHits << endl << endl;


                    // Calculate the total time taken by the search
        duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

        // Convert the duration to seconds
        time_taken = duration.count() / 1000.0;

    cout << "Time taken to Search : " << time_taken << " sec" << endl;





    delete HT;

    return 0; // Return 0 for successful completion
}
