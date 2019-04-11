#ifndef _gain
#define _gain

#include "distribution.hpp"

/**
 * \brief Class used to represent a gain function.
 *
 * A gain function is a function from a set of secrets and a set of adversary actions to a real number.
 *
 * A gain makes reference to an adversary's gain. The adversary can take some action, and for each secret, he or she achieves a gain.
 *
 * As a gain function makes reference to a set of secrets, one of the attributes is a pointer to a @ref Distribution, which represents the prior probability distribution on the set of secrets.
 */
class Gain{
	
	public:
		/**
		 * \brief Default constructor
		 *
		 * It instances a @ref Gain object with @ref num_act = 0, @ref prior = NULL and an empty gain @ref matrix.
		 */

		Gain();

		/**
		 * \brief Construtor used when the gain function matrix is in a file.
		 *
		 * The file must be in the following format:
		 *
		 * 	w n
		 * 	p11 p12 ... p1n
		 * 	p21 p22 ... p2n
		 * 	...
		 * 	pw1 pw2 ... pwn
		 *
		 * where \c w is the number of actions, and \c n is the number of secrets (\c n = @ref Distribution::num_el in the @ref prior).
		 * The rows correspond to actions and the columns correspond to secrets.
		 * Each probability \c pij is the gain of taking action \c i when the secret value is \c j. Each two numbers
		 * must be separated by a " " (space).
		 * 
		 * \param prior: Prior distribution on a set of secrets
		 * \param file: File's name which contains a gain function matrix.
		 *
		 * \warning The number of rows in the gain function matrix must be as same as the number of elements in the @ref prior distribution.
		 */
		Gain(Distribution &prior, std::string file);

		/**
		 * \brief Constructor used when there is already a gain function matrix in a variable. 
		 *
		 * \param prior Prior distribution on a set of secrets
		 * \param matrix Matrix W x X, where W is the number of adversary's actions and X is the number of secrets.
		 * 
		 * \warning The number of cloumns in the gain function must be the same as the number of elements in prior attribute.
		 */
		Gain(Distribution &prior, std::vector<std::vector<long double> > &matrix);

		/**
		 * \brief Constructor used to generate random gain function to a set of secrets.
		 *
		 * The set of secrets can be described by a @ref Distribution, so it is the first parameter.
		 *
		 * \param prior: Prior distribution on a set of secrets.
		 * \param num_act: Number of actions.
		 * \param MIN: Lower bound for the gain of an action.
		 * \param MAX: Upper bound for the gain of an action.
		 *
		 * The parameters MIN and MAX must form an interval. All the gains generated randomly will be in the interval [MIN,MAX].
		 * Each gain is a \c long \c double value.
		 *
		 */
		Gain(Distribution &prior, int num_act, int MIN, int MAX);

		/**
		 * \brief Number of distinct actions that an adversary can take.
		 */
		int num_act;

		/**
		 * \brief A pointer to a prior probability distribution on a set of secrets.
		 */
		Distribution *prior = NULL;
		
		/**
		 * \brief Gain function matrix.
		 *
		 * The lines are the actions, the columns are the secrets and each cell
		 * (\c w,\c x) in the matrix is the gain that an adversary achieves by taking action \c w when the actual value of the secret is \c x. 
		 *
		 * The matrix has dimensions W x N, where W is the number of actions (@ref num_act) and N is the number of secrets (num_el of the @ref prior).
		 */
		std::vector<std::vector<long double> > matrix;

		/**
		 * \brief Returns a string with the gain function matrix.
		 *
		 * The string's format is the following:
		 *
		 * 	p11 p12 ... p1n
		 * 	p21 p22 ... p2n
		 * 	...
		 * 	pw1 pw2 ... pwn
		 *
		 * Each probability \c pij is the gain of taking action \c i when the secret value is \c j. Each two numbers
		 * are separated by a " " (space).
		 *
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		std::string toString(int precision);

		/**
		 * \brief Print the gain function matrix in a file.
		 *
		 * The gain function matrix is printed in the following format:
		 *
		 * 	w n
		 * 	p11 p12 ... p1n
		 * 	p21 p22 ... p2n
		 * 	...
		 * 	pw1 pw2 ... pwn
		 *
		 * where \c w is the number of actions, and \c n is the number of secrets.
		 * Each probability \c pij is the gain of taking action \c i when the secret value is \c j. Each two numbers
		 * are separated by a " " (space).
		 *
		 * \param file A file's name to print the gain function matrix.
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string file, int precision);
};

#endif