#include <iostream>
#include <fstream>
#include <string>
#include "HumanGenomeReader.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char** argv)
{
    HumanGenomeReader* reader = new HumanGenomeReader();
    reader->FilePath = "human_small.txt";
    cout << "Hello World \n";
    // Get starting timepoint
    reader->ReadFile();
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

    delete reader; // Properly delete the dynamically allocated object
    return 0;
}
