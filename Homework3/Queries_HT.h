
#include <string>
using namespace std;

class Queries_HT
{
	private:
		char* genomeArray;
		long long int totalGenomeLength = 0;
		long long int m = 0;
		long long int numberOfCollisions = 0;
		int fragmentLength = 16;
		long long int numberOfHits = 0;

	public:
		struct Node {
			char *data;
			Node *Next;
		};

		Node** HashTable;	
		Queries_HT(long long int size);
		long long int getRadixHash(string key);
		void readFragments(string fragmentFilePath);
		void readHumanGenomes(string genomeFilePath);
		void print();
		void insert(string key);
		void search();
		long long int findIndex(string subStr);
		~Queries_HT();
};

 