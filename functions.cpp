#include "functions.hpp"

long double bayesVulnerability(Distribution &D){
	return *max_element(D.probability.begin(), D.probability.end());
}

long double posteriorBayesVulnerability(Channel &C){
	long double vulnerability = 0;

	for(int i = 0; i < C.y; i++){
		long double MAX = C.inners[0][i];
		for(int j = 1; j < C.prior->n; j++){
			if(MAX < C.inners[j][i]){
				MAX = C.inners[j][i];
			}
		}
		vulnerability += (C.outer[i]*MAX);
	}

	return vulnerability;
}

long double shannonEntropy(Distribution &D){
	long double entropy = 0;

	for(int i = 0; i < D.n; i++){
		if(D.probability[i] > 0){
			entropy += (D.probability[i] * log2(D.probability[i]));
		}
	}

	return -entropy;
}

long double posteriorShannonEntropy(Channel &C){
	long double entropy = 0;

	for(int i = 0; i < C.y; i++){ /* For every output */
		long double posteriorEntropy = 0;
		for(int j = 0; j < C.prior->n; j++){
			if(C.inners[j][i] > 0){
				posteriorEntropy += (C.inners[j][i] * log2(C.inners[j][i]));
			}
		}
		entropy -= (C.outer[i] * posteriorEntropy);
	}

	return entropy;
}

long double gVulnerability(Actions &W){
	long double MAX;
	long double acc = 0;

	for(int i = 0; i < W.prior->n; i++){
		acc += (W.prior->probability[i] * W.G[0][i]); 
	}

	MAX = acc;
	for(int i = 1; i < W.w; i++){
		acc = 0;
		for(int j = 0; j < W.prior->n; j++){
			acc += (W.prior->probability[j] * W.G[i][j]);
		}
		if(acc > MAX){
			MAX = acc;
		}
	}

	return MAX;
}

long double posteriorGVulnerability(Channel &C, Actions &W){

	if(C.prior != W.prior){
		fprintf(stderr, "The prior distribution from channel and actions are not the same!\n");
		exit(EXIT_FAILURE);
	}

	long double vulnerability = 0;

	for(int i = 0; i < C.y; i++){
		/* For each posterior distribution.*/

		long double MAX = -1;
		long double acc;

		for(int k = 0; k < W.w; k++){
			/* For each action k.*/
			acc = 0;
			for(int j = 0; j < C.prior->n; j++){
				/* For each secret j.*/
				acc += (C.J[j][i] * W.G[k][j]);
			}

			if(MAX < acc){
				MAX = acc;
			}
		}
		
		vulnerability += MAX;
	}

	return vulnerability;
}

long double kTries(Distribution &D, int k){
	long double S = 0;

	vector<long double> ordered_distribution = D.probability;
	sort(ordered_distribution.rbegin(), ordered_distribution.rend());

	for(int i = 0; i < k; i++){
		S += ordered_distribution[i];
	}

	return S;
}

long double posteriorKTries(Channel &C, int k){
	long double S = 0;

	for(int i = 0; i < C.y; i++){
		vector<long double> ordered_d;
		
		for(int j = 0; j < C.prior->n; j++){
			ordered_d.push_back(C.inners[j][i]);
		}

		sort(ordered_d.rbegin(), ordered_d.rend());

		long double pS = 0;
		for(int j = 0; j < k; j++){
			pS += ordered_d[j];
		}

		S += (C.outer[i] * pS);
	}

	return S;
}

long double guessingEntropy(Distribution &D){
	long double gentropy = 0;

	vector<long double> ordered_distribution = D.probability;
	sort(ordered_distribution.rbegin(), ordered_distribution.rend());

	for(int i = 0; i < D.n; i++){
		gentropy += ((i+1) * ordered_distribution[i]);
	}

	return gentropy;
}

long double posteriorGuessingEntropy(Channel &C){
	long double gentropy = 0;

	vector<long double> ordered_outer = C.outer;
	sort(ordered_outer.begin(), ordered_outer.end());

	for(int i = 0; i < C.y; i++){
		gentropy += ((i+1) * C.outer[i]);
	}

	return gentropy;
}

/* Leakage measures */

long double additiveLeakage(Channel &C, Actions &W){
	return posteriorGVulnerability(C,W) - gVulnerability(W);
}

long double multiplicativeLeakage(Channel &C, Actions &W){
	return posteriorGVulnerability(C,W) / gVulnerability(W);
}