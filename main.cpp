#include <stdio.h>
#include <iostream>
#include "qif.hpp"

using namespace std;

int main(){

	vector<vector<long double> > ch = {
		{1.0/2, 1.0/6, 1.0/3,   0},
		{  0, 1.0/3, 2.0/3,   0},
		{  0, 1.0/2,   0, 1.0/2},
		{ 1.0/4, 1.0/4, 1.0/2,   0},
	};

	vector<long double> p = {0.33, 0.33, 0, 0.34};

	Distribution prior(p);

	Channel channel(prior, ch);

	Hyper hyper(prior, channel);

	std::cout << prior.toString(4) << std::endl;

	std::cout << std::endl << channel.toString(4) << std::endl;
	std::cout << hyper.toString("inners", 4) << std::endl;

	std::cout << std::endl << hyper.toString("labels", 4) << endl;

	return 0;
}