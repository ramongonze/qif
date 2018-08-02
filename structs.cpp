#ifndef _structs
#include "structs.hpp"
#endif

Distribution::Distribution(string file){
	long double p;
	long double tot_sum = 0;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		printf("Error reading file!\n");
		return;
	}

	n = 0;
	while(fscanf(F, "%Lf", &p) != EOF){
		probability.push_back(p);
		n++;
		tot_sum += p;
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