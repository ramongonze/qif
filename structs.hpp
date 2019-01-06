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
 * \brief A class used to represent a probability distribution.
 * 
 * A probability distribution is made up by the number of elements in the distribution (attribute @ref num_el) and a vector of probabilities (attribute @ref prob).
 */

class Distribution{

	public:
		/**
		 * \brief Default constructor.
		 *
		 * It instances a probability distribution with \c num_el = 0 and an empty vector \c prob.
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
		 * \param num_el Number of elements in the distribution.
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
		 * It is a vector which contains all the elements' probabilities of a probability distribution.
		 * The vector is indexed from the position 0 (1st element) to the position @ref num_el - 1 (the last element). So the vector's size is @ref num_el.
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
		 * \brief Give back a string with the probability distribution.
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
		 * \param D A probability distribution
		 * \param file The file's name.
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(Distribution &D, std::string file, int precision);
};

class Actions{
	
	public:
		Actions();

		/* Construtor used when the matrix of Gain x Secret is in a file.
		 *
		 * The file must be in CSV format, following above:
		 *
		 * w,n
		 * p11,p12,...,p1n
		 * p21,p22,...,p2n
		 * ...
		 * pw1,pw2,...,pwn
		 *
		 * where w is the number of actions and n is the number of elements in 
		 * the prior distribution.
		 *
		 * -> The rows correspond to actions
		 * -> The columns correspond to secrets
		 *
		 * Each cell (w,x) in the matrix is a gain g(w,x) that specifies the 
		 * gain that the adversary achieves by taking action w when the actual 
		 * value of the secret is x.
		 *
		 * @Parameter _prior: Prior distribution
		 * @Parameter file: File's name which contains the matrix of actions. */
		Actions(Distribution &_prior, std::string file);

		/* Constructor used to generate random gains for a given prior distribution.
		 *
		 * @Parameter _prior: Prior distribution
		 * @Parameter _num_ac: Number of actions
		 * @Parameter MIN: Lower bound for the gain of an action
		 * @Parameter MAX: Upper bound for the gain of an action */
		Actions(Distribution &_prior, int _num_ac, int MIN, int MAX);

		/* Number of possible actions */
		int num_ac;

		/* A probability distribution of a set X of secrets. */
		Distribution *prior;
		
		/* Matrix |W| x |X| representing actions and their gains for each 
		 * secret. */
		std::vector<std::vector<long double> > gain;

		/* Generates a string with the matrix G in the same format that the 
		 * constructor Actions(Distribution &prior, string file) reads it in a 
		 * file.
		 *
		 * @Parameter precision: Decimal precision for float numbers. */
		std::string toString(int precision);

		/* Generates a string with a standard float numbers precision from 
		 * C++. */
		std::string toString();
};

class Channel{
	public:
		Channel();
		
		/* Construtor used when the matrix C of secret x output is in a file.
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
		 * where y is the number of outputs in the channel and n is the number 
		 * of elements in the distribution D.
		 *
		 * -> The rows correspond to secrets
		 * -> The columns correspond to outputs
		 *
		 * All entries in the channel matrix are between 0 and 1, and each row 
		 * sums to 1.
		 * The rows give the distribution on outputs corresponding to each 
		 * possible input.  */
		Channel(Distribution &prior, std::string file);

		/* Constructor used to generate random channels.
		 *
		 * @Parameter _prior: Prior distribution
		 * @Parameter _y: Number of outputs
		 * @Parameter max_prob: A float number in the interval [0,1]. It is 
		 *                      used as an upper bound of any outputs' 
		 *                      probabilities. Used to generate random 
		 *                      distributions. */
		Channel(Distribution &prior, int _y, long double max_prob);

		/* Number of outputs. */
		int num_out;
		
		/* A probability distribution of a set of X screts. */
		Distribution *prior;
		
		/* Channel matrix, indexed by X*Y, whose rows give the distribution 
		 * on outputs corresponding to each possible input. It has size n*y. */
		std::vector<std::vector<long double> > matrix;

		/* Verify if the channel matrix corresponds to a valid channel. */
		static bool isChannel(std::vector<std::vector<long double> > &matrix);
		
		/* Generates a string with the channel matrix C in the same format the 
		 * constructor Channel(Distribution &prior, string file) reads it in a 
		 * file.
		 *
		 * @Parameter precision: Decimal precision for float numbers. */
		std::string toString(int precision);

		/* Generates a string with a standard float numbers precision from 
		 * C++. */
		std::string toString();
};

class Hyper{
	private:
		void buildHyper(Distribution &prior, Channel &channel, std::vector<std::vector<long double> > &joint, Distribution &outer, std::vector<std::vector<long double> > &inners);
	public:
		Hyper();
		Hyper(std::string prior_file, std::string channel_file);
		Hyper(Distribution &prior, Channel &channel);
		
		/* A probability distribution of a set of X screts. */
		Distribution *prior;
		Channel *channel;

		/* Joint distribution matrix = Prior distribtion * channel. */
		std::vector<std::vector<long double> > joint;

		/* Random variable for outputs in Y. Each probability in the position 
		 * i represents the probability p(i) of output i occurs. */
		Distribution outer;

		/* A matrix for the inners distributions. Each position (x,y) in the 
		 * matrix is the probability of a secret x occurs given the output y, 
		 * that is, p(x|y). So each column is a posterior distribution. */
		std::vector<std::vector<long double> > inners;

		/* Generates a string with the joint distribution in the following format:
		*
		* n o
		* p11 p12 ... p1o
		* p21 p22 ... p2o
		* ...
		* pn1 pn2 ... pno
		*
		* where n is the number of secrets, o is the number of outputs and pij is 
		* the probability of secret i and output j occurs. */
		std::string toStringJoint();

		
		std::string toStringOuter();

		std::string toStringInners();
};

#endif