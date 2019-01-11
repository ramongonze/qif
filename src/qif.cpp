#include "../qif.hpp"

/** 
 * \mainpage
 *
 * \section description Description
 *
 * This library has the goal to model QIF - Quantitative Information Flow.
 *
 * It can be used to model systems and measure entropies, vulnerabilities and leakages of them.
 *
 * This library models the basics of QIF such as probability distributions, informational channels, gain functions, hyper-distributions,
 * and some functions to measure information flow.
 *
 * \line l1
 *
 * \section req System Requirements
 *
 * - G++ compiler - Available on most UNIX disrtibutions.
 *
 * - GNU **ar** (_archive_). It is also available on most UNIX disrtibutions. For more details acess [ar documentation](http://man7.org/linux/man-pages/man1/ar.1.html).
 *
 * \line l9
 *
 * \section usage Usage
 *
 * To use the library follow these steps:
 *
 * 1. Copy the folder **qif** to your project's directory.
 *
 * 2. Open the folder **qif** in the terminal and type **make** to compile all the modules. 
 * A binary file named **qif.a** will be generated.
 *
 * 3. Include the library in your C++ project using \c #include \c "qif/qif.hpp"
 *
 * 4. Compile your project adding \c **qif/qif.a** in the command line used to compile your C++ project. 
 * Example: \c g++ \c main.cpp  \c qif/qif.a
 *
 * Following the steps above you will be able to use all the classes and functions from this library.
 *
 * \line l2
 *
 * \section References
 * [[1]] (https://www.springer.com/us/book/9783319961293) Alvim M.S; Chatzikokolakis K; McIver A; Morgan C; Palamidessi C; Smith G.S.
 * **The Science of Quantitative Information Flow**. Springer, 1 edition, 2019.
 *
 * \line l3
 *
 * \author Gonze, R.
 * \version 1.0
 * \date 2019-01
 */