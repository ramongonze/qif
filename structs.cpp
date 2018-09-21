#ifndef _structs
#include "structs.hpp"
#endif

/*********** Distributions ***********/

Distribution::Distribution(string file){
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	fscanf(F, "%d", &n);
	probability.resize(n);
	for(int i = 0; i < n; i++){
		fscanf(F, "%Lf", &probability[i]);
	}

	fclose(F);

	if(!isDistribution(probability)){
		fprintf(stderr, "The numbers in the file %s are not a probability distribution!!\n", file.c_str());
		exit(EXIT_FAILURE);
	}
}	

Distribution::Distribution(int _n, string type, long double max_prob){

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
		fprintf(stderr, "Invalid argument to create a distribution! \n");
		exit(EXIT_FAILURE);
	}
}

bool Distribution::isDistribution(vector<long double> &D){
	long double S = 0;

	for(unsigned int i = 0; i < D.size(); i++){
		if(D[i] < 0 || D[i] > 1+EPS){
			return false;
		}

		S += D[i];
	}

	return 1-EPS <= S && S <= 1+EPS;
}

string Distribution::toString(int precision){
	ostringstream out;
	out << setprecision(precision) << n << "\n";
	

	for(int i = 0; i < n-1; i++){
		out << probability[i] << ",";
	}
	out << probability[n-1] << "\n";

	return out.str();
}

string Distribution::toString(){
	ostringstream out;
	out << n << "\n";
	
	for(int i = 0; i < n-1; i++){
		out << probability[i] << ",";
	}
	out << probability[n-1] << "\n";

	return out.str();
}

/*********** Actions ***********/

Actions::Actions(Distribution &_prior, string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	fscanf(F, "%d,%d", &w, &k);
	if(k != _prior.n || w <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
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

string Actions::toString(int precision){
	ostringstream out;
	out << setprecision(precision) << w << ","  << prior->n << "\n";
	
	for(int i = 0; i < w; i++){
		for(int j = 0; j < prior->n-1; j++){
			out << G[i][j] << ",";
		}
		out << G[i][prior->n-1] << "\n";
	}

	return out.str();
}

string Actions::toString(){
	ostringstream out;
	out << w << ","  << prior->n << "\n";
	
	for(int i = 0; i < w; i++){
		for(int j = 0; j < prior->n-1; j++){
			out << G[i][j] << ",";
		}
		out << G[i][prior->n-1] << "\n";
	}

	return out.str();
}

/*********** Channels ***********/

bool Channel::isChannel(vector<vector<long double> > &C){
	for(unsigned int i = 0; i < C.size(); i++){
		if(!Distribution::isDistribution(C[i])){
			return false;
		}
	}

	return true;
}

Channel::Channel(Distribution &_prior, string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	fscanf(F, "%d,%d", &k, &y);
	if(k != _prior.n || y <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
	}

	prior = &_prior;

	C.resize(prior->n, vector<long double>(y));

	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y; j++){
			fscanf(F, "%Lf,", &(C[i][j]));
		}
	}
	fclose(F);

	if(!Channel::isChannel(C)){
		fprintf(stderr, "Error reading a channel. One of the rows is not a probability distribution!\n");
		exit(EXIT_FAILURE);
	}else{
		Channel::buildHyper();
	}
}

Channel::Channel(Distribution &_prior, int _y, long double max_prob){

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

	Channel::buildHyper();
}

string Channel::toString(int precision){
	ostringstream out;
	out << setprecision(precision) << prior->n << "," << y << "\n";
	
	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y-1; j++){
			out << C[i][j] << ",";
		}
		out << C[i][y-1] << "\n";
	}

	return out.str();
}

string Channel::toString(){
	ostringstream out;
	out << prior->n << "," << y << "\n";
	
	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y-1; j++){
			out << C[i][j] << ",";
		}
		out << C[i][y-1] << "\n";
	}

	return out.str();
}

void Channel::buildHyper(){
	J.resize(prior->n, vector<long double>(y));
	outer.resize(y,0);

	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y; j++){
			J[i][j] = C[i][j] * prior->probability[i];
			outer[j] += J[i][j];
		}
	}

	inners.resize(prior->n, vector<long double>(y));

	for(int i = 0; i < prior->n; i++){
		for(int j = 0; j < y; j++){
			if(outer[j] == 0){
				inners[i][j] = 0;
			}else{
				inners[i][j] = J[i][j] / outer[j];
			}
		}
	}
}
