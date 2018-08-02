#define _structs
#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

// Used to compare float numbers
#define EPS 1e-6

class Distribution{
	public:
		Distribution();
		/*
		 * Constructor used when a probability distribution is in a file.
		 * The file format must be
		 *
		 * probability 1
		 * probability 2
		 * ...
		 * probability n
		 *
		 * where n is the number of elements in the distribution.
		 *
		 * @Parameter file: The name of the file that contains a probability distribution to be read
		*/
		Distribution(string file);
		
		/*
		 * Constructur used to generate a uniform or a random distribution.
		 *
		 * @Parameter _n: Number of elements in the distribution.
		 * @Parameter type: 'uniform' for a uniform distribution or 'random' for a random distribution
		 * @Parameter max_prox: A float number in the interval [0,1]. It is used as an upper bound of
		 * 						any element's probability. Used to generate random distributions.
		*/
		Distribution(int _n, string type, float max_prob);
		
		int n; // Number of elements in the distribution
		vector<long double> probability; // Elements' probabilities
		map<string,int> label; // Key: Label of an element; Value: Position of this elemnt in the vector 
							   // of probabilities.
};