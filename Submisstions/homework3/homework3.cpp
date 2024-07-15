#include <chrono>        // For high resolution clock and measuring time durations
#include "Queries_HT.h"  // Include the header file for the Queries_HT class

using namespace std;         // Use the standard namespace
using namespace std::chrono; // Use the chrono namespace for time functions

int main(int argc, char **argv)
{
    // Check if enough command-line arguments are provided
    if (argc < 4)
    {
        cerr << "Usage: " << argv[0] << " <file_path> <queries_file_path> <hash_table_length>" << endl;
        return 1; // Return with error code 1 if not enough arguments
    }

    // Retrieve file path and queries file path from command line arguments
    string filePath = argv[1];
    string queriesFilePath = argv[2];

    // Convert the third argument to a long long int for hash table length
    char *endptr;
    long long int hashTableLength = strtoll(argv[3], &endptr, 10);

    // Check if the conversion was successful
    if (*endptr != '\0') {
        cerr << "Invalid number for hash table length." << endl;
        return 1; // Return with error code 1 if conversion fails
    }

    // Variable to store the time taken for searching
    double time_taken = 0.0;

    // Variables to measure the start and end time
    chrono::high_resolution_clock::time_point startTime;
    chrono::high_resolution_clock::time_point endTime;
    chrono::milliseconds duration;

    // Create a new Queries_HT object with given file paths and hash table length
    Queries_HT* HT = new Queries_HT(filePath, queriesFilePath, hashTableLength);

    // Measure the time taken to read the human genome data
    cout << "Human Genome Reading started" << endl;
    startTime = chrono::high_resolution_clock::now();

    HT->ReadFile();

    endTime = chrono::high_resolution_clock::now();

    // Calculate the total time taken to read the file
    duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    time_taken = duration.count() / 1000.0; // Convert the duration to seconds

    cout << "Human Genome Reading completed" << endl;
    cout << "Time taken to Read Subject Dataset: " << time_taken << " sec" << endl << endl;

    // Measure the time taken to read the query fragments
    cout << "Query Dataset Reading started" << endl;
    startTime = chrono::high_resolution_clock::now();

    HT->readFragments();

    endTime = chrono::high_resolution_clock::now();

    // Calculate the total time taken to read the query fragments
    duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    time_taken = duration.count() / 1000.0; // Convert the duration to seconds

    cout << "Query Dataset Reading completed" << endl;
    cout << "Time taken to populate Hashtable of size " << hashTableLength << ": " << time_taken << " sec" << endl << endl;

    // Output the total number of collisions encountered
    cout << "Total number of Collisions: " << HT->numberOfCollisions << endl << endl;

    // Measure the time taken to search the query fragments in the hash table
    cout << "Searching started" << endl;
    startTime = chrono::high_resolution_clock::now();

    HT->searchFragments();

    endTime = chrono::high_resolution_clock::now();

    cout << "Searching completed" << endl << endl;

    // Output the total number of occurrences (hits) found
    cout << "Total number of Occurances: " << HT->numberOfHits << endl << endl;

    // Calculate the total time taken to search
    duration = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
    time_taken = duration.count() / 1000.0; // Convert the duration to seconds

    cout << "Time taken to Search: " << time_taken << " sec" << endl;

    // Delete the Queries_HT object to free up memory
    delete HT;

    return 0; // Return 0 for successful completion
}