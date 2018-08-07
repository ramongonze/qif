#ifndef _structs
#include "structs.hpp"
#endif

bool isOne(long double n){
	return 1-EPS <= n && n <= 1+EPS;
}

Distribution::Distribution(string file){
	long double tot_sum = 0;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
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
	if(!isOne(tot_sum)){
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

			if((long double)x/RAND_MAX > max_prob){
				x = RAND_MAX * max_prob;
			}
			
			probability.push_back((long double)x/RAND_MAX);
			threshold -= x;
		}
		probability.push_back((long double)threshold/RAND_MAX);
	}else{
		fprintf(stderr, "Invalid argument! \n");
	}
}

string Distribution::toString(){
	setprecision(PRECISION);
	string out = to_string(n) + "\n";
	
	for(int i = 0; i < n; i++){
		out = out + to_string(probability[i]) + "\n";
	}

	return out;
}

Actions::Actions(Distribution &_prior, string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		return;
	}

	fscanf(F, "%d,%d", &w, &k);
	if(k != _prior.n || w <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		return;
	}

	prior = &_prior;
	G.resize(w, vector<long double>(prior->n));

	for(int i = 0; i < w; i++){
		for(int j = 0; j < prior->n; j++){
			fscanf(F, "%Lf,", &G[i][j]);
		}
	}

	fclose(F);
}

Actions::Actions(Distribution &_prior, int _w, int MIN, int MAX){
	prior = &_prior;
	w = _w;
	G.resize(w, vector<long double>(prior->n));

	for(int i = 0; i < w; i++){
		for(int j = 0; j < prior->n; j++){
			G[i][j] = rand()%(MAX-MIN+1) + MIN;
		}
	}
}

string Actions::toString(){
	setprecision(PRECISION);
	string out = to_string(w) + ","  + to_string(prior->n) + "\n";
	
	for(int i = 0; i < w; i++){
		for(int j = 0; j < prior->n-1; j++){
			out = out + to_string(G[i][j]) + ",";
		}
		out = out + to_string(G[i][prior->n-1]) + "\n";
	}

	return out;
}

Channels::Channels(Distribution &_prior, string file){
	int k;
	bool test_distribution;
	long double row_sum;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		return;
	}

	fscanf(F, "%d,%d", &k, &y);
	if(k != _prior.n || y <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		return;
	}

	prior = &_prior;

	C.resize(prior->n, vector<long double>(y));

	test_distribution = true;
	for(int i = 0; i < prior->n; i++){
		row_sum = 0;
		for(int j = 0; j < y; j++){
			fscanf(F, "%Lf,", &(C[i][j]));
			row_sum += C[i][j];
		}
			if(!isOne(row_sum)){
			test_distribution = false;
			break;
		}
	}

	if(!test_distribution){
		fprintf(stderr, "Error reading a channel. One of the rows is not a probability distribution!\n");
	}

	fclose(F);

	Channels::buildHyper();
}

Channels::Channels(Distribution &_prior, int _y, float max_prob){

	prior = &_prior;
	y = _y;

	C.resize(prior->n, vector<long double>(y));
	// Random

	for(int i = 0; i < prior->n; i++){
		int threshold = RAND_MAX;
		int x;

		for(int j = 0; j < y-1; j++){
			x = rand() % threshold;

			if((long double)x/RAND_MAX > max_prob){
				x = RAND_MAX * max_prob;
			}
			
			C[i][j] = (long double)x/RAND_MAX;
			threshold -= x;
		}

		C[i][y-1] = (long double)threshold/RAND_MAX;
	}

	Channels::buildHyper();
}

string Channels::toString(){
	setprecision(PRECISION);
	string out = to_string(prior->n) + "," + to_string(y) + "\n";
	
	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y-1; j++){
			out = out + to_string(C[i][j]) + ",";
		}
		out = out + to_string(C[i][y-1]) + "\n";
	}

	return out;
}

void Channels::buildHyper(){
	J.resize(prior->n, vector<long double>(y));
	PY.resize(y,0);

	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y; j++){
			J[i][j] = C[i][j] * prior->probability[i];
			PY[j] += J[i][j];
		}
	}

	H.resize(prior->n, vector<long double>(y));

	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y; j++){
			if(PY[j] == 0){
				H[i][j] = 0;
			}else{
				H[i][j] = J[i][j] / PY[j];
			}
		}
	}
}
