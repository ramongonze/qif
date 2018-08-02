#ifndef _structs
#include "structs.hpp"
#endif

Distribution::Distribution(string file){
	long double tot_sum = 0;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		printf("Error reading file!\n");
		return;
	}

	fscanf(F, "%d", &n);
	probability.resize(n);
	for(int i = 0; i < n; i++){
		fscanf(F, "%Lf", &probability[i]);
		tot_sum += probability[i];
	}

	fclose(F);

	// Check if the sum of all elements are 1, that is, is a probability distribution
	if(1-EPS > tot_sum || tot_sum > 1+EPS){
		printf("The numbers in the file %s are not a probability distribution!!\n", file.c_str());
	}
}

Distribution::Distribution(int _n, string type, float max_prob){

	n = _n;
	
	if(type == "uniform"){
		probability.resize(n, 1/(long double)n);
	}else if(type == "random"){
		// Random
		int threshold = RAND_MAX;
		int x;

		for(int i = 0; i < _n-1; i++){
			x = rand() % threshold;
			if((long double)x/RAND_MAX > max_prob)
				x = RAND_MAX * max_prob;
			
			probability.push_back((long double)x/RAND_MAX);
			threshold -= x;
		}
		probability.push_back((long double)threshold/RAND_MAX);
	}else{
		printf("Invalid argument! \n");
	}
}

void Distribution::toString(){
	printf("Number of elements: %d\n", n);
	printf("Probability distribution: (");
	for(int i = 0; i < n-1; i++){
		printf("%.2Lf,", probability[i]);
	}
	printf("%.2Lf)\n", probability[n-1]);
}

Actions::Actions(Distribution &_D, string file){
	int k;
	long double p;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		printf("Error reading file!\n");
		return;
	}

	fscanf(F, "%d,%d", &w, &k);
	if(k != _D.n || w < 0){
		printf("Invalid matrix size!\n");
		return;
	}

	D = &_D;
	G.resize(w, vector<long double>(D->n));

	for(int i = 0; i < w; i++){
		for(int j = 0; j < D->n; j++){
			fscanf(F, "%Lf,", &G[i][j]);
		}
	}

	fclose(F);
}

Actions::Actions(Distribution &_D, int _w, int MIN, int MAX){
	D = &_D;
	w = _w;
	G.resize(w, vector<long double>(D->n));

	for(int i = 0; i < w; i++){
		for(int j = 0; j < D->n; j++){
			G[i][j] = rand()%(MAX-MIN+1) + MIN;
		}
	}
}