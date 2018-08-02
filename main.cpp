#include <iostream>
#include <time.h>

#ifndef _structs
#include "structs.hpp"
#endif

#ifndef _functions
#include "functions.hpp"
#endif

using namespace std;

int main(){

	srand(8);
	Distribution D(5,"uniform",1);

	D.toString();

	Actions W(D,10,-5,5);

	cout << "Vg(W) = " << gVulnerability(W) << endl;

	return 0;
}