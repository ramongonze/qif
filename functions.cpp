#include "functions.hpp"

long double bayesVulnerability(Distribution D){
	return *max_element(D.probability.begin(), D.probability.end());
}

long double shannonEntropy(Distribution D){
	long double sha = 0;
	for(int i = 0; i < D.n; i++){
		sha += D.probability[i] * log2(D.probability[i]);
	}

	return -sha;
}