#ifndef _distribution
#define _distribution

#include <sstream>
#include <vector>
#include <string>
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
		 * \param type Must be "uniform" for generating a uniform distribution or "random" for a random distribution.
		 */
		Distribution(int num_el, std::string type);
		
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

#endif