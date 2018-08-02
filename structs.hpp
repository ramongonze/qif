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

/*
 * X -> Set of secrets. |X| = n (the same n in an object from a Distribution class).
 * W -> Set of actions. |W| = w.
 *
*/


class Distribution{
	public:
		Distribution();
		/*
		 * Constructor used when a probability distribution is in a file.
		 * The file format must be
		 *
		 * n
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
		 * Constructor used to generate a uniform or a random distribution.
		 *
		 * @Parameter _n: Number of elements in the distribution.
		 * @Parameter type: 'uniform' for a uniform distribution or 'random' for a random distribution
		 * @Parameter max_prox: A float number in the interval [0,1]. It is used as an upper bound of
		 * 						any element's probability. Used to generate random distributions.
		*/
		Distribution(int _n, string type, float max_prob);
		
		int n; // Number of elements in the distribution
		vector<long double> probability; // Elements' probabilities
		map<string,int> label; // Key: Label of an element; Value: Position of this element in the vector 
							   // of probabilities.

		// Prints the number of elements and the probability distribution as a n-tuple.
		void toString();
};

class Actions{
	
	public:
		Actions();

		/*
		 * Construtor used when the matrix G of gain x secret is in a file.
		 * The matrix has dimensions |W| x |X|.
		 * and |W| is the number of actions, which will be read in the file.
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
		Actions(Distribution &_D, string file);

		/*
		 * Constructor used to generate random gains for a distribution D.
		 * @Parameter w: Number of actions
		 * @Parameter MIN: Lower bound for the gain of an action
		 * @Parameter MAX: Upper bound for the gain of an action
		*/
		Actions(Distribution &_D, int _w, int MIN, int MAX);

		int w; // Number of actions
		Distribution *D; // A probability distribution of a set X of secrets
		// Matrix |W| x |X| that represents actions and their gains for each secret.
		vector<vector<long double> > G;
};