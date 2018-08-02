#ifndef _structs
#include "structs.hpp"
#endif

/*
 * Bayes Vulnerability of a distribution D:
 * Adversary's probability of guessing a secret X in one try.
*/
long double bayesVulnerability(Distribution D);

/*
 * Shannon Entropy of a distribution D:
 * A way to measure the secrecy of a set X of secrets.
 * The value returned is in the interval [0,log2(D.n)],
 * where D.n is the number of secrets.
*/
long double shannonEntropy(Distribution D);