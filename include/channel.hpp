#ifndef _channel
#define _channel

#include "../include/distribution.hpp"

/**
 * \brief Class used to represent an informational channel.
 *
 * A channel can be described as a system that takes as input a secret - from a set of secrets - and whose only observable behavior is to produce an output - from a set of possible outputs.
 * So a channel is a function from a set of secrets to a set of possible outputs. 
 *
 * We can use a matrix to represent the entire channel. In this library, a channel matrix is a matrix whose rows give the distribution on outputs corresponding to each possible input. 
 * Hence each cell is the conditional probability p(\c y|\c x), for every secret \c x in the set of secrets and every \c y in the set of possible outputs.
 *
 * As a channel makes reference to a set of secrets, one of the attributes is a pointer to a @ref Distribution, which represents the prior probability distribution on the set of secrets.
 *
 */
class Channel{
	public:
		/**
		 * \brief Default constructor
		 *
		 * It instances a @ref Channel object with @ref num_out = 0, @ref prior = NULL and an empty @ref matrix.
		 */
		Channel();
		
		/**
		 * \brief Construtor used when the channel matrix is in a file.
		 * 
		 * The file format must be:
		 *
		 * 	n y
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets (\c n = @ref Distribution::num_el in the @ref prior) and \c y is the number of outputs.
		 * The rows correspond to secrets and the columns correspond to outputs. Each \c pij is the conditional probability p(\c j|\c i) of the 
		 * output \c j happens when the current secret is \c i.
		 * Each two numbers must be separated by a " " (space).
		 * 
		 * The rows give the distribution on outputs corresponding to each possible input.
		 *
		 * \param prior Prior distribution on a set of secrets
		 * \param file File's name which contains a channel matrix.
		 *
		 * \warning The number of rows in the channel matrix must be as same as the number of elements in the @ref prior distribution.
		 *
		 */
		Channel(Distribution &prior, std::string file);

		/**
		 * \brief Constructor used when there is already a channel matrix in a variable.
		 *
		 * \param prior: Prior distribution on a set of secrets.
		 * \param matrix: A channel matrix related to the distribution @ref prior.
		 * 
		 * \warning The number of rows in the channel matrix must be as same as the number of elements in the @ref prior distribution.
		 */
		Channel(Distribution &prior, std::vector<std::vector<long double> > &matrix);

		/**
		 * \brief Constructor used to generate random channels.
		 *
		 * Given a set of secrets in the pointer @ref prior, the constructor builds a random channel matrix.
		 * The number of the channel outputs is passed as an argument.
		 *
		 * \param prior: Prior distribution on a set of secrets.
		 * \param num_out: Number of outputs.
		 * \param max_prob: A float number in the interval [0,1]. It is used as an upper bound of any outputs' probabilities.
		 * For example, when \c max_prob is \c 0.4, all the probabilities in the matrix will be less equal to \c 0.4.
		 * 
		 * \warning We must have \c max_prob * \c num_out >= 1, otherwise a row in the channel matrix would not represent a probability distribution.
		 * \warning The number of rows in the channel matrix must be as same as the number of elements in the @ref prior distribution.
		 */
		Channel(Distribution &prior, int num_ac, long double max_prob);

		/**
		 * \brief Number of channel outputs.
		 */
		int num_out;
		
		/**
		 * \brief A probability distribution on a set of secrets. 
		 */
		Distribution *prior;
		
		/**
		 * \brief Channel matrix.
		 *
		 * The rows give the distribution on outputs corresponding to each possible input.
		 *
		 * The matrix has dimensions N*Y, where N = @ref Distribution::num_el in the @ref prior and Y = @ref num_out.
		 */
		std::vector<std::vector<long double> > matrix;

		/**
		 * \brief Verify if a matrix corresponds to a valid channel matrix.
		 * 
		 * A channel matrix is a valid one if each row is a probability distribution.
		 *
		 * \param matrix A channel matrix
		 */
		static bool isChannel(std::vector<std::vector<long double> > &matrix);
		
		/**
		 * \brief Returns a string with the channel matrix.
		 *
		 * The string's format is the following:
		 *
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets (\c n = @ref Distribution::num_el in the @ref prior) and \c y is the number of outputs of the channel.
		 * Each \c pij is the conditional probability p(\c j|\c i) of the output \c j happens when the current secret is \c i.
		 * Each two numbers are separated by a " " (space).
		 *
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		std::string toString(int precision);

		/**
		 * \brief Print the channel matrix in a file.
		 *
		 * The channel matrix is printed in the following format:
		 *
		 * 	n y
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets (\c n = @ref Distribution::num_el in the @ref prior) and \c y is the number of outputs of the channel.
		 * Each \c pij is the conditional probability p(\c j|\c i) of the output \c j happens when the current secret is \c i.
		 * Each two numbers are separated by a " " (space).
		 *
		 * \param file A file's name to print the channel matrix.
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string, int precision);
};

#endif