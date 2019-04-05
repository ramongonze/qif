#include <stdio.h>
#include <iostream>
#include "qif.hpp"

using namespace std;

int main(){

	vector<vector<long double> > ch = {
		{   1,   0,    0},
		{0.25, 0.5, 0.25},
		{ 0.5, 0.33, 0.17}
	};

	vector<long double> p = {0.25, 0.25, 0.5};

	Distribution prior(p);

	Channel channel(prior, ch);

	return 0;
}