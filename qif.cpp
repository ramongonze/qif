#include "qif.hpp"

/** 
 * \mainpage
 *
 * \section description Library description
 *
 * This library has the goal to model QIF - Quantitative Information Flow.
 *
 * It can be used to model systems and measure entropies, vulnerabilities and leakages of them.
 *
 * This library models the base of QIF, such as probability distributions, informational channels, gain functions, hyper-distributions,
 * and some functions to measure information flow.
 *
 * \line l1
 *
 * \section usage Usage
 *
 * To use the library follow these steps:
 *
 * - Copy the folder **qif** to your project's directory.
 *
 * - Open the folder **qif** in the terminal and type **make** to compile all the modules. 
 * A binary file named **qif.o** will be generated.
 *
 * - Include the library in your C++ project using \c #include \c "./qif/qif.hpp"
 *
 * - Compile your project adding **qif.o** in the command line used to compile your C++ project. 
 * Example: \c g++ \c main.cpp \c qif.o
 *
 * Following the steps above you will be able to use all the classes and functions of this library.
 *
 * \line l2
 *
 * \section References
 * \link https://www.springer.com/us/book/9783319961293 [1] \endlink Alvim M.S; Chatzikokolakis K; McIver A; Morgan C; Palamidessi C; Smith G.S.
 * **The Science of Quantitative Information Flow**. Springer, 1 edition, 2019.
 *
 * \line l3
 *
 * \author Gonze, R.
 * \version 1.0
 * \date 2019-01
 */