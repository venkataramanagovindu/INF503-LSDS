#include <chrono>
#include "HumanGenomeReader.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv)
{
    // Check if a file path and sub-program were provided
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <file_path> <sub_program>" << endl;
        return 1;
    }

    // Retrieve file path and sub-program from command line arguments
    string filePath = argv[1];
    string subProgram = argv[2];

    // Create a HumanGenomeReader object
    HumanGenomeReader* reader = new HumanGenomeReader();

    // Check if the sub-program is either "all" or "1"
    if(subProgram == "all" || subProgram == "1")
    {
        // Set the file path for the reader
        reader->FilePath = filePath;

        // Read the genome file
        cout << "Reading the file" << endl << endl;
        reader->ReadFile();

        // If the sub-program is "all", assess the genome and measure the time taken
        if(subProgram == "all")
        {

            time_t start_t, end_t;
            time(&start_t);

            // Assess the genome
            reader->AssesGenome();

            time(&end_t);

            // Calculating total time taken by the program.
            double time_taken = double(end_t - start_t);
            cout << "Time taken to read the file : " << fixed
                << time_taken;
            cout << " sec " << endl;
        }
    }
    else
    {
        // Inform the user if the sub-program argument is incorrect
        cout << "Please enter the correct filepath and sub program to run" << endl;
    }

    // Properly delete the dynamically allocated object
    delete reader;
    return 0;
}
