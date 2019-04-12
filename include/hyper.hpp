#ifndef _hyper
#define _hyper

#include "../include/distribution.hpp"
#include "../include/channel.hpp"
#include <math.h>
#include <map>
#include <set>

/**
 * \brief Class used to represent a hyper-distribution.
 *
 * Definition of hyper-distribution:
 *
 * If **X** is a finite set (of possible secret values), π is the prior distribution on **X**,
 * and C is a channel, a hyper-distribution _[π>C]_ resulted from C on π is a 
 * distribution on distributions on **X**. Each output in an informational channel is a possible "world",
 * and each possible world is a new distribution on the set of secrets. We call the possible worlds
 * the *inner distributions*. Each possible world has a probability to occur, and we call the 
 * distribution on the possible worlds as the *outer distribution*.
 *
 * The @ref Hyper object has 7 attributes:
 *
 * - @ref prior : A pointer to a @ref Distribution object, which represents
 * the prior probability distribution on the set of secrets.
 *
 * - @ref channel : A pointer to a @ref Channel object.
 *
 * - @ref joint : Joint dstribution matrix, where each position _joint[x][y]_ is the probability _p(x,y)_.
 *
 * - @ref inners : The inner distributions matrix. It is the set of possible "worlds" of a hyper-distribution.
 * Each column of this matrix is a posterior distribution on the set of secrets.
 *
 * - @ref outer : The probability distribution on the inners.
 *
 * - @ref num_post : Number of posterior distributons.
 *
 * - @ref labels : A map from an integer to a set. The number of posterior distributions can differ from the
 * number of channel outputs, because when we have two or more posteriors that are equals, we simplify them merging
 * into a single posterior, and sum their outer probabilities. So _labels_ maps from new posteriors
 * to a subset of channel outputs.
 */
class Hyper{
	private:
		void buildJoint();
		void buildOuter();
		void buildInners();
		void buildHyper();
		void reduceHyper();

	public:
		/**
		 * \brief Default constructor.
		 *
		 * This constructor creates a @ref Hyper object with @ref prior and @ref channel equals to NULL,
		 * @ref num_post = 0 and @ref joint matrix, @ref inners matrix, @ref outer and @ref labels empty.
		 */
		Hyper();

		/**
		 * \brief Constructor used when the prior distribution on the set of secrets and the channel are in a file.
		 *
		 * \param prior_file File name that contains a probability distribution.
		 * \param channel_file File name that contains a channel matrix.
		 *
		 * \warning The number of rows in the channel matrix must be equals to the number of
		 * elements in the prior distribution.
		 */
		Hyper(std::string prior_file, std::string channel_file);

		/**
		 * \brief Constructor used when there is already a @ref Channel object in a variable.
		 *
		 * As a channel has a pointer to a distribution (corresponding to the prior distribution on the set of secrets),
		 * it is not necessary to give a @ref Distribution as a parameter.
		 *
		 * \param channel A @ref Channel object.
		 */
		Hyper(Channel &channel);

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
		 * \brief Number of posterior distributions
		 */
		int num_post;

		/**
		 * \brief A matrix for the inner distributions.
		 *
		 * Each position (\c x,\c y) in the matrix is the conditional
		 * probability p(\c x|\c y) which is the probability of the secret being \c x when the output is \c y.
		 * Each column is a posterior distribution.
		 */
		std::vector<std::vector<long double> > inners;

		/**
		 * \brief A map from a output index to a set of indexes
		 *
		 * When we look at the hyper-distrubtion, we have the outer distribution and the inner distributions.
		 * Depending on the prior distribution and the channel, the hyper-distribution may have two different
		 * outputs which yield the same posterior distribution on the set of secrets, and in this case, there is no
		 * difference for the adversary choosing one or another output.
		 *
		 * Indeed we can simplify the representation of a hyper-distribution matrix mergin all equal columns
		 * in a single column, and sum their outer probabilities.
		 *
		 * The map key is the index of an output in the reduced hyper-distribution matrix.
		 *
		 * The value of a key is the set of columns indexes which gave rise this single column.
		 *
		 * Example: If the columns 2 and 4 were merged in a single column which its index is 3, when you call
		 * \c labels[3] the result will be the set {2,4}.
		 *
		 */
		std::map<int, std::set<int> > labels;


		/** \brief Generates a string with the joint matrix, the outer distribution, the inner distributions or 
		 * the @ref labels.
		 *
		 * \return Possible returns:
		 *
		 * - The joint matrix is returned in the following format:
		 *
		 * 	  p11 p12 ... p1y
		 * 	  p21 p22 ... p2y
		 * 	  ...
		 * 	  pn1 pn2 ... pny
		 *
		 * 	 where \c n is the number of secrets, \c y is the number of channel outputs and \c pij is
		 * 	 the joint probability _p(i,j)_.
		 *
		 * - The outer distribution is returned in the following format:
		 *
		 * 	  p1 p2 ... py
		 *
		 *   where \c y is the number of outputs.
		 *
		 * - The inner distributions matrix is returned in the following format:
		 *
		 * 	  p11 p12 ... p1y
		 * 	  p21 p22 ... p2y
		 * 	  ...
		 * 	  pn1 pn2 ... pny
		 *
		 *   where \c n is the number of secrets, \c y is the number of outputs and \c pij is
		 *   the conditional probability _p(i|j)_.
		 *
		 * - The labels are returned in the following format:
		 *
		 * 	  1: x11 x12 ...
		 * 	  2: x21 x22 ...
		 * 	  ...
		 * 	  3: xn1 xn2 ...
		 *
		 *   where \c n is the number of posteriors (@ref num_post) and \c xij is an integer between 0 and @ref channel -> num_out.
		 *
		 * Each two numbers in any choice are are separated by a space (" ").
		 *
		 * \param type The name of what will be returned. It must be "labels" to return the @ref labels, "joint",
		 * "outer" or "inners", to return the @ref joint matrix, @ref outer distribution
		 * or the @ref inners' matrix, respectively.
		 *
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		std::string toString(std::string type, int precision);

		/**
		 * \brief Print the joint matrix, outer distribution or the inner distributions in a file.
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
		 * or the inners' matrix, respectively.
		 * \param file A file's name.
		 * \param precision Decimal precision for float numbers. For example, use the value 3 to print 0.322, use 5 to print 0.32197, and so on.
		 */
		void printToFile(std::string type, std::string file, int precision);

		/**
		 * \brief A method used to rebuild the joint, outer and inner distributions.
		 *
		 * It calculates:
		 *
		 * - The @ref joint distribution - multiplying the @ref channel and the @ref prior distribution;
		 *
		 * - The @ref outer distribution - summing all the joint probabilities for each posterior distribution;
		 *
		 * - The @ref inners - normalizing each column of the joint matrix by the respectively outer probability.
		 *
		 * \param prior Prior distribution on a set of secrets
		 *
		 * \warning The number of rows in the channel matrix must be as equals to the number of elements in the prior distribution.
		 */
		void rebuildHyper(Distribution &prior);
		void rebuildHyper(Channel &channel);
};

#endif
