#ifndef _leakage
#define _leakage

#include "../include/hyper.hpp"
#include "../include/gain.hpp"
#include "../include/vulnerability.hpp"

/**
 * \brief A definition for positive infinite.
 */
#define POSITIVE_INFINITE -7

/* Leakage measures */

/**
 * \brief The absolute difference between the posterior g-vulnerability and the prior g-vulnerability.
 *
 * \param hyper A hyper-distribution.
 * \param gain A gain function for a set of secrets.
 *
 * \return gVulnerability(hyper-distribtuion) - gVulnerability(prior distribtuion)
 */
long double additiveLeakage(Hyper &hyper, Gain &gain);

/**
 * \brief The relative difference between the posterior g-vulnerability and the prior g-vulnerability.
 *
 * \param hyper A hyper-distribution.
 * \param gain A gain function for a set of secrets.
 *
 * \return If gVulnerability(prior distribution) is 0 and gVulnerability(hyper-distribtuion) is also 0, it returns 1.
 *
 * \return If gVulnerability(prior distribtuion) is 0 and gVulnerability(hyper-distribtuion) is different of 0, it returns @ref POSITIVE_INFINITE.
 *
 * \return If both gVulnerability(prior distribtuion) and gVulnerability(hyper-distribtuion) are different of 0, it returns gVulnerability(hyper-distribtuion) / gVulnerability(prior distribtuion).
 *
 * See the section 5.2 in the reference [1] for more details about the return of multiplicative leakage.
 *
 */
long double multiplicativeLeakage(Hyper &hyper, Gain &gain);

#endif
