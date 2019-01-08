/** 
 * \mainpage
 * \author Ramon Gonze
 * \version 1.0
 * \date 2019-01
 */

#ifndef _structs

#define _structs

#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <algorithm>

/**
 * \brief Used to compare float numbers.
 *
 * As computers have limit to store float numbers, a comparision between two float numbers is not well defined (precision erros can happen).
 *
 * For this library, we consider two numbers as equals if their difference is less than EPS,
 * which the default value is 1e-6.
 */
#define EPS 1e-6

/**
 * \brief Class used to represent a probability distribution.
 * 
 * A probability distribution is on a set of elements, so the class has the attribute @ref num_el to keep the number of elements in the set
 * and the attribute @ref prob to keep the probability of each element from the set occurs.
 */
class Distribution{

	public:
		/**
		 * \brief Default constructor.
		 *
		 * It instances an @ref Distribution object with @ref num_el = 0 and an empty vector @ref prob.
		 */
		Distribution();
		
		/** 
		 * \brief Constructor used when the probability distribution is in a file.
		 *
		 * The file format must be the following:
		 *
		 * 	n
		 * 	p1
		 * 	p2
		 * 	...
		 * 	pn
		 *
		 * where \c n is the number of elements in the distribution and \c pi is the probability of the ith element occurs.
		 *
		 * \param file File's name which contains a probability distribution.
		 */
		Distribution(std::string file);
		
		/**
		 * \brief Constructor used when there is already a probability distribution in a vector.
		 *
		 * \param prob A vector of numbers which make up a probability distribution.
		 */
		Distribution(std::vector<long double> &prob);

		/**
		 * \brief Constructor used to generate a uniform or a random probability distribution.
		 *
		 * \param num_el Number of elements in the set of elements.
		 * \param type Must be "uniform" for a uniform distribution or "random" for a random distribution.
		 * \param max_prob A number in the interval [0,1]. When \c type is "uniform", \c max_prob is ignored (any value can be passed).
		 * When \c type is "random" it is used as an upper bound for generating the elements' probabilities in the distribution.
		 * For example, when \c max_prob is \c 0.4, all the probabilities in the new probability distribution are less equal to \c 0.4.
		 */
		Distribution(int num_el, std::string type, long double max_prob);
		
		/** \brief Number of elements in the distribution */
		int num_el;

		/**
		 * \brief A vector of elements' probabilities.
		 * 
		 * It is a vector which contains all the elements' probabilities. Each position \c i in the vector is the probability of the ith element occurs.
		 * The vector is indexed from the position 0 (1st element) to the position @ref num_el - 1 (last element). So the vector's size is @ref num_el.
		 */
		std::vector<long double> prob;

	    /** 
	     * \brief Check if a vector of numbers make up a probability distribution.
	     *
		 * - For every probability \c p in the vector, \c 0 <= \c p <= \c 1;
		 * - The sum of all probabilities in the vector is 1 , with margin of error equals 
		 *   to the consant EPS (upper or lower) - see the definition of constant @ref EPS. 
		 *
		 * \return It returns \c true if the vector contains a probability distribution or \c false otherwise.
		 */
		static bool isDistribution(std::vector<long double> &prob);

		/**
		 * \brief Returns a string with the probability distribution.
		 * 
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 *
		 * \return It returns a string in the following format:
		 * 
		 * 	p1 p2 p3 ... pn
		 *
		 * \return where \c pi is the probability of the ith element occurring. Each two values are separated by a " " (space).
		 */  
		std::string toString(int precision);

		/**
		 * \brief Print the probability distribution in a file.
		 *
		 * The probability distribution is printed in the following format:
		 *
		 * 	n
		 * 	p1
		 * 	p2
		 * 	...
		 * 	pn
		 *
		 * where \c n is the number of elements in the probability distribution and \c pi is the ith element in the vector of probabilities.
		 *
		 * \param file A file's name to print the distribution.
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string file, int precision);
};

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
		 * It instances a @ref Gain object with @ref num_ac = 0, @ref prior = NULL and an empty matrix @ref gain.
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
		 * \brief Constructor used to generate random gain function to a set of secrets.
		 *
		 * The set of secrets can be described by a @ref Distribution, so it is the first parameter.
		 *
		 * \param prior: Prior distribution on a set of secrets.
		 * \param num_ac: Number of actions.
		 * \param MIN: Lower bound for the gain of an action.
		 * \param MAX: Upper bound for the gain of an action.
		 *
		 * The parameters @ref MIN and @ref MAX must form an interval. All the gains generated randomly will be in the interval [@ref MIN,@ref MAX].
		 * Each gain is a \c long \c double value.
		 *
		 * \warning The number of rows in the gain function matrix must be as same as the number of elements in the @ref prior distribution.
		 */
		Gain(Distribution &prior, int num_ac, int MIN, int MAX);

		/**
		 * \brief Number of distinct actions that an adversary can take.
		 */
		int num_ac;

		/**
		 * \brief A pointer to a prior probability distribution on a set of secrets.
		 */
		Distribution *prior;
		
		/**
		 * \brief Gain function matrix.
		 *
		 * The lines are the actions, the columns are the secrets and each cell
		 * (_w_,_x_) in the matrix is the gain that an adversary achieves by taking action _w_ when the actual value of the secret is _x_. 
		 *
		 * The matrix has dimensions W x N, where W is the number of actions (@ref num_ac) and N is the number of secrets (@ref num_el of the @ref prior).
		 */
		std::vector<std::vector<long double> > gain;

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


/**
 * \brief Class used to represent a hyper-distribution.
 *
 * A hyper-distribution is a distribution on distributions on a set of secrets. Each output in a informational channel is a possible "world",
 * and each possible world is a new distribution on the set of secrets. We call the possible worlds the *inner distributions*. 
 * Each possible world has a probability to occur, and we call the distribution on the possible worlds as the *outer distribution*.
 *
 * We can calculate the outer and the inners distributions using the joint distribution between the set of secrets and the set of outputs.
 */
class Hyper{
	private:
		void buildHyper(Distribution &prior, Channel &channel, std::vector<std::vector<long double> > &joint, Distribution &outer, std::vector<std::vector<long double> > &inners);

	public:

		/**
		 * \brief Default constructor.
		 *
		 * It instances a @ref Hyper object with @ref prior = NULL, @ref channel = NULL, and empty @ref joint, @ref outer and @ref inners.
		 */
		Hyper();

		/**
		 * \brief Constructor used when the prior distribution on the set of secrets and the channel are in a file.
		 *
		 * \param prior_file File's name which contains a prior distribution on a set of secrets.
		 * \param channel_file File's name which contains a channel matrix.
		 *
		 * \warning The number of rows in the channel matrix must be as same as the number of elements in the prior distribution.
		 */
		Hyper(std::string prior_file, std::string channel_file);

		/**
		 * \brief Constructor used when the prior distribution on the set of secrets and the channel are already instanced in variables.
		 *
		 * \param prior Prior distribution on a set of secrets
		 * \param channel Channel matrix
		 *
		 * \warning The number of rows in the channel matrix must be as same as the number of elements in the prior distribution.
		 */
		Hyper(Distribution &prior, Channel &channel);
		
		/**
		 * \brief A pointer to a probability distribution on a set of secrets.
		 */
		Distribution *prior;

		/**
		 * \brief A pointer to a channel.
		 */
		Channel *channel;

		/**
		 * \brief Joint distribution matrix.
		 *
		 * The joint distribution is build multiplying the prior distribution and each row of the channel matrix. 
		 */
		std::vector<std::vector<long double> > joint;

		/**
		 * \brief The distribution on the set of outputs.
		 */
		Distribution outer;

		/**
		 * \brief A matrix for the inners distributions.
		 *
		 * Each position (\c x,\c y) in the matrix is the conditional
		 * probability p(\c x|\c y) which is the probability of the secret being \c x when the output is \c y.
		 * Each column is a posterior distribution.
		 */
		std::vector<std::vector<long double> > inners;

		/** \brief Returns a string with the joint matrix, the outer distribution or the inners distributions.
		 *
		 * The choice of what will be returned is passed in the first parameter \c type.
		 *
		 * The joint matrix is returned in the following format:
		 * 
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets, \c y is the number of outputs and \c pij is 
		 * the joint probability p(\c i,\c j) of a secret \c i and an output \c j.
		 *
		 * The outer distribution is returned in the following format:
		 *
		 * 	p1 p2 ... py
		 *
		 * where \c y is the number of outputs. Each two numbers are separated by a " " (space).
		 *
		 * The inners distributions matrix is returned in the following format:
		 *
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets, \c y is the number of outputs and \c pij is 
		 * the conditional probability p(\c i|\c j), which is the probability of the secret being \c i when the output is \c j.
		 *
		 * Each two numbers in any choice are are separated by a " " (space).
		 *
		 * \param type The name of what will be returned. It must be "joint", "outer" or "inners", to return the joint matrix, outer distribution
		 * or the inners distribution matrix, respectively.
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		std::string toString(std::string type, int precision);

		/**
		 * \brief Print the joint matrix, outer distribution or the inners distributions in a file.
		 *
		 * The choice of what will be printed is passed in the first parameter \c type.
		 *
		 * The joint matrix is printed in the following format:
		 *
		 * 	n y
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets and \c y is the number of outputs of the channel.
		 * Each \c pij is the joint probability p(\c i,\c j), for a secret \c i and an output \c j.
		 *
		 * The outer distribution is printed in the following format:
		 *
		 * 	y
		 * 	p1 p2 ... py
		 *
		 * where \c n is the number of outputs in the channel.
		 * Each \c pi is the probability of the ith output happens.
		 *
		 * The inners' matrix is printed in the following format:
		 *
		 * 	n y
		 * 	p11 p12 ... p1y
		 * 	p21 p22 ... p2y
		 * 	...
		 * 	pn1 pn2 ... pny
		 *
		 * where \c n is the number of secrets and \c y is the number of outputs of the channel.
		 * Each \c pij is the conditional probability p(\c i|\c j) which is the probability of the secret being \c i when the output is \c j.
		 *
		 * Each two numbers in any printing are separated by a " " (space).
		 *
		 * \param type The name of what will be printed. It must be "joint", "outer" or "inners", to print the joint matrix, outer distribution
		 * or the inners distribution matrix, respectively.
		 * \param file A file's name.
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string type, std::string file, int precision);
};

#endif