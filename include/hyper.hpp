#ifndef _hyper
#define _hyper

#include "../include/distribution.hpp"
#include "../include/channel.hpp"

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
		/* A method used to build the joint, outer and inner distributions. */
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
		Distribution *prior = NULL;

		/**
		 * \brief A pointer to a channel.
		 */
		Channel *channel = NULL;

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