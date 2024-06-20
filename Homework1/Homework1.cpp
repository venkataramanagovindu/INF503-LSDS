#include <chrono>
#include "HumanGenomeReader.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv)
{



    // Check if a file path was provided
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <file_path> <subprogrm>" << endl;
        return 1;
    }

    string filePath = argv[1];
    string subProgram = argv[2];

    HumanGenomeReader* reader = new HumanGenomeReader();
    if(subProgram == "all" || subProgram == "1")
    {
        // reader->FilePath = "/common/contrib/classroom/inf503/genomes/human.txt";
        reader->FilePath = filePath;
        // Get starting timepoint
        cout << "Reading the file" << endl << endl;
        reader->ReadFile();

        if(subProgram == "all")
        {
            auto start = high_resolution_clock::now();
            reader->AssesGenome();
            // Get ending timepoint
            auto stop = high_resolution_clock::now();

            // Get duration. Substart timepoints to 
            // get duration. To cast it to proper unit
            // use duration cast method
            auto duration = duration_cast<seconds>(stop - start);
        
            cout << "Time taken by function: "
                << duration.count() << " Seconds" << endl;
        }
    }
    else{
        cout << "Please enter the correct filepath and sub program to run";
    }

    // delete reader; // Properly delete the dynamically allocated object
    return 0;
}