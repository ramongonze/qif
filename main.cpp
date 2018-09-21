#include <iostream>
#include <time.h>
#include "structs.hpp"
#include "functions.hpp"

using namespace std;

int main(){

	srand(8);
	Distribution prior("prior");

	Channel C(prior, "channel");

	return 0;
}