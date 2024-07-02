#include <chrono>
#include "Queries_AR.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv)
{
    // Check if a file path and sub-program were provided
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file_path> <sub_program>" << endl;
        return 1;
    }

    // Retrieve file path and sub-program from command line arguments
    string filePath = argv[1];
    string queriesFilePath = argv[2];
    // string subProgram = argv[2];

    // Create a Queries_AR object
    Queries_AR* queriesReader = new Queries_AR();

    cout << queriesReader << endl;

    // Check if the sub-program is either "all" or "1"
    // if(subProgram == "all" || subProgram == "1")
    // {
        // Set the file path for the reader
        queriesReader->FilePath = filePath;
        queriesReader->QueriesFilePath = queriesFilePath;

        // Read the genome file
        cout << "Reading the file" << endl << endl;
        queriesReader->ReadFile();
        queriesReader->ReadQueriesFile();

        cout << "Subject file reading completed" << endl;

        cout << queriesReader->QueriesArray[0] << endl;

        cout << "Searching Started" << endl;

        queriesReader->Search();

        cout << "Searching Completed" << endl;

        cout << "Before Sort" << endl;
        for(int i = 0; i < 5;i++){
            cout << queriesReader->QueriesArray[i] << endl;
        }
        queriesReader->sort();
        cout << "After Sort" << endl;
                for(int i = 0; i < 5;i++){
            cout << queriesReader->QueriesArray[i] << endl;
        }

        queriesReader->Search();




        // If the sub-program is "all", assess the genome and measure the time taken
        // if(subProgram == "all")
        // {

        //     time_t start_t, end_t;
        //     time(&start_t);

        //     // Assess the genome
        //     reader->AssesGenome();

        //     time(&end_t);

        //     // Calculating total time taken by the program.
        //     double time_taken = double(end_t - start_t);
        //     cout << "Time taken to read the file : " << fixed
        //         << time_taken;
        //     cout << " sec " << endl;
        // }
    // }
    // else
    // {
    //     // Inform the user if the sub-program argument is incorrect
    //     cout << "Please enter the correct filepath and sub program to run" << endl;
    // }

    // Properly delete the dynamically allocated object
    delete queriesReader;
    return 0;
}
