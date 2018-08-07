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

long double gVulnerability(Actions W){
	long double MAX;
	long double acc = 0;

	for(int j = 0; j < W.prior->n; j++){
		acc += (W.prior->probability[j] * W.G[0][j]); 
	}

	MAX = acc;
	for(int i = 1; i < W.w; i++){
		acc = 0;
		for(int j = 0; j < W.prior->n; j++){
			acc += (W.prior->probability[j] * W.G[0][j]);
		}
		if(acc > MAX)
			MAX = acc;
	}

	return MAX;
}