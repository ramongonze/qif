#ifndef _structs

#define _structs
#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <algorithm>

using namespace std;

// Used to compare float numbers
#define EPS 1e-6

// Precision of float numbers. Used in functions toString()
#define PRECISION 5

/*
 * X -> Set of secrets. |X| = n (the same n in an object from a Distribution class)
 * W -> Set of actions. |W| = w (the same w in an object from an Actions class)
 * Y -> Set of outputs. |Y| = y (the same y in an object from a Channels class)
*/

// Test if the parameter x is equals to one, with margin of error equals
// to the consant EPS (upper or lower)
bool isOne(long double n);

class Distribution{
	public:
		Distribution();
		/*
		 * Constructor used when a probability distribution is in a file.
		 * The file format must be:
		 *
		 * n
		 * probability 1
		 * probability 2
		 * ...
		 * probability n
		 *
		 * where n is the number of elements in the distribution.
		 *
		 * @Parameter file: File's name that contains a probability distribution
		*/
		Distribution(string file);
		
		/*
		 * Constructor used to generate a uniform or a random distribution.
		 *
		 * @Parameter _n: Number of elements in the distribution.
		 * @Parameter type: 'uniform' for a uniform distribution or 'random' for a random distribution
		 * @Parameter max_prob: A float number in the interval [0,1]. It is used as an upper bound of
		 * 						any element's probability. Used to generate random distributions.
		*/
		Distribution(int _n, string type, float max_prob);
		
		// Number of elements in the distribution
		int n;

		// Elements' probabilities
		vector<long double> probability;

		// Key: Label of an element; Value: Position of this element in the vector of probabilities.
		map<string,int> label;

		// Generates a string with the probability distribution in the same format the constructor
		// Distribution(string file) reads in a file.
		string toString();
};

class Actions{
	
	public:
		Actions();

		/*
		 * Construtor used when the matrix G of gain x secret is in a file.
		 * The matrix has dimensions |W| x |X|.
		 * |W| is the number of actions, which will be read in the file.
		 *
		 * The file format must be:
		 *
		 * w,n
		 * p11,p12,...,p1n
		 * p21,p22,...,p2n
		 * ...
		 * pw1,pw2,...,pwn
		 *
		 * where w is the number of actions and n is the number of elements in the distribution D.
		 *
		 * -> The rows correspond to actions
		 * -> The columns correspond to secrets
		 *
		 * Each cell (w,x) in the matrix is a gain g(w,x) that specifies the gain that the adversary
		 * achieves by taking action w when the actual value of the secret is x.
		 *
		*/
		Actions(Distribution &_prior, string file);

		/*
		 * Constructor used to generate random gains for a distribution D.
		 *
		 * @Parameter _prior: Prior distribution
		 * @Parameter _w: Number of actions
		 * @Parameter MIN: Lower bound for the gain of an action
		 * @Parameter MAX: Upper bound for the gain of an action
		*/
		Actions(Distribution &_prior, int _w, int MIN, int MAX);

		// Number of actions
		int w;

		// A probability distribution of a set X of secrets
		Distribution *prior;
		
		// Matrix |W| x |X| representing actions and their gains for each secret.
		vector<vector<long double> > G;

		// Generates a string with the matrix G in the same format the constructor
		// Actions(Distribution &prior, string file) reads in a file.
		string toString();
};

class Channels{
	private:
		// Build teh Hyper-distribution. As a prerequisite, the 
		// joint distribution (matrix J) is also built.
		void buildHyper();

	public:
		Channels();
		
		/*
		 * Construtor used when the matrix C of secret x output is in a file.
		 * The matrix has dimensions |X| x |Y|.
		 * |Y| is the number of outputs, which will be read in the file.
		 *
		 * The file format must be:
		 *
		 * n,y
		 * p11,p12,...,p1y
		 * p21,p22,...,p2y
		 * ...
		 * pn1,pn2,...,pny
		 *
		 * where y is the number of outputs in the channel and n is the number of
		 * elements in the distribution D.
		 *
		 * -> The rows correspond to secrets
		 * -> The columns correspond to outputs
		 *
		 * All entries in the channel matrix are between 0 and 1, and each row sums to 1.
		 * The rows give the distribution on outputs corresponding to each possible input.
		 *
		*/
		Channels(Distribution &prior, string file);

		/*
		 * Constructor used to generate random channels.
		 *
		 * @Parameter _prior: Prior distribution
		 * @Parameter _y: Number of outputs
		 * @Parameter max_prob: A float number in the interval [0,1]. It is used as an upper bound of
		 * 						any outputs' probabilities. Used to generate random distributions.
		*/
		Channels(Distribution &prior, int _y, float max_prob);

		// Number of outputs
		int y;
		
		// A probability distribution of a set of X screts
		Distribution *prior;
		
		// Channel matrix C, indexed by X*Y, whose rows give the distribution on outputs
		// corresponding to each possible input. It has size n*y.
		vector<vector<long double> > C;
		
		// Joint matrix = Prior distribtion * C
		vector<vector<long double> > J;

		// --- Hyper-Distribution ---

		// Random variable for outputs in Y. Each probability in the position i represents
		// the probability of output i occurs.
		vector<long double> PY;

		// A matrix for a hyper-distribution. Each position (x,y) in the matrix is the probability
		// of a secret x occurs given the output y, that is, p(x|y).
		vector<vector<long double> > H;

		// --------------------------

		// Generates a string with the channel matrix C in the same format the constructor
		// Channels(Distribution &prior, string file) reads in a file.
		string toString();
};
#endif