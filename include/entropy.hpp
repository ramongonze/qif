#ifndef _entropy
#define _entropy

#include "../include/hyper.hpp"
#include <cmath>

/**
 * \brief Guessing entropy of a prior probability distribtuion.
 *
 * It is the expected number of tries of the adversary trying to guess the secret correctly.
 *
 * \param dist A prior probability distribution on a set of secrets.
 *
 * \return Guessing entropy of the distribution in the parameter @ref **dist**.
 */
long double guessingEntropy(Distribution &dist);

/**
 * \brief Guessing entropy of a hyper-distribution.
 *
 * It is the expected number of tries of the adversary trying to guess the secret correctly,
 * looking to all possible worlds (inner distributions). 
 *
 * \param hyper A hyper-distribution.
 *
 * \return Guessing entropy of the hyper-distribution in the parameter @ref **hyper**.
 */
long double guessingEntropy(Hyper &hyper);

/**
 * \brief Shannon entropy of a prior probability distribution.
 *
 * A way to measure the secrecy of a secret, looking to a prior probability distribution.
 * The returned value is in the interval [0,log2(\c n)], where \c n is the number of secrets.
 *
 * \param dist A prior probability distribution on a set of secrets.
 *
 * \return Shannon entropy of the distribution in the parameter @ref **dist**.
 */
long double shannonEntropy(Distribution &dist);

/**
 * \brief Shannon entropy of a hyper-distribution.
 *
 * A way to measure the secrecy of a secret, looking to a hyper-distribution.
 * The returned value is in the interval [0,log2(\c y)], where \c y is the
 * number of outputs of a channel.
 *
 * \param hyper A hyper-distribution.
 *
 * \return Shannon entropy of the hyper-distribution in the parameter @ref **hyper**.
 */
long double shannonEntropy(Hyper &hyper);

#endif
