#include "structs.hpp"

/*********** Distribution ***********/

Distribution::Distribution(){
	num_el = 0;
	prob.resize(0);
}

Distribution::Distribution(std::string file){
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	if(!fscanf(F, "%d", &num_el)){
		exit(EXIT_FAILURE);
	}

	prob.resize(num_el);
	for(int i = 0; i < num_el; i++){
		if(!fscanf(F, "%Lf", &prob[i])){
			exit(EXIT_FAILURE);
		}
	}

	fclose(F);

	if(!isDistribution(prob)){
		fprintf(stderr, "The numbers in the file %s are not a probability distribution!\n", file.c_str());
		exit(EXIT_FAILURE);
	}
}	

Distribution::Distribution(std::vector<long double> &prob){
	if(!isDistribution(prob)){
		fprintf(stderr, "The numbers in the vector does not make up a probability distribution!\n");
		exit(EXIT_FAILURE);
	}

	num_el = prob.size();
	this->prob.resize(0);

	for(int i = 0; i < num_el; i++){
		this->prob.push_back(prob[i]);		
	}
}

Distribution::Distribution(int num_el, std::string type, long double max_prob){

	this->num_el = num_el;
	
	if(type == "uniform"){
		prob.resize(this->num_el, 1/(long double)this->num_el);
	}else if(type == "random"){
		// Random distribution
		int threshold = RAND_MAX;
		int x;

		for(int i = 0; i < num_el-1; i++){
			x = rand() % threshold;

			if((long double)x/RAND_MAX > max_prob){
				x = RAND_MAX * max_prob;
			}
			
			prob.push_back((long double)x/RAND_MAX);
			threshold -= x;
		}
		prob.push_back((long double)threshold/RAND_MAX);
	}else{
		fprintf(stderr, "Invalid argument to create a distribution! \n");
		exit(EXIT_FAILURE);
	}
}

bool Distribution::isDistribution(std::vector<long double> &prob){
	long double S = 0;

	for(unsigned int i = 0; i < prob.size(); i++){
		if(prob[i] < 0 || prob[i] > 1+EPS){
			return false;
		}

		S += prob[i];
	}

	return 1-EPS <= S && S <= 1+EPS;
}

std::string Distribution::toString(int precision){
	std::ostringstream out;

	out << std::fixed << std::setprecision(precision);
	
	for(int i = 0; i < num_el-1; i++){
		out << prob[i] << " ";
	}
	out << prob[num_el-1];

	return out.str();
}

void printToFile(Distribution &D, std::string file, int precision){
	std::ofstream F;

	F.open(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << D.num_el << "\n";
	F << std::fixed << std::setprecision(precision);
	for(int i = 0; i < D.num_el; i++){
		F << D.prob[i] << "\n";
	}

	F.close();
}

/*********** Actions ***********/

Actions::Actions(){
	num_ac = 0;
	prior = NULL;
	gain.resize(0, std::vector<long double>(0));
}

Actions::Actions(Distribution &_prior, std::string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	if(!fscanf(F, "%d,%d", &num_ac, &k)){
		exit(EXIT_FAILURE);
	}

	if(k != _prior.num_el || num_ac <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
	}

	prior = &_prior;
	gain.resize(num_ac, std::vector<long double>(prior->num_el));

	for(int i = 0; i < num_ac; i++){
		for(int j = 0; j < prior->num_el; j++){
			if(!fscanf(F, "%Lf,", &gain[i][j])){
				exit(EXIT_FAILURE);
			}
		}
	}

	fclose(F);
}

Actions::Actions(Distribution &_prior, int _num_ac, int MIN, int MAX){
	prior = &_prior;
	num_ac = _num_ac;
	gain.resize(num_ac, std::vector<long double>(prior->num_el));

	for(int i = 0; i < num_ac; i++){
		for(int j = 0; j < prior->num_el; j++){
			gain[i][j] = rand()%(MAX-MIN+1) + MIN;
		}
	}
}

std::string Actions::toString(int precision){
	std::ostringstream out;
	out << std::setprecision(precision) << num_ac << " "  << prior->num_el << "\n";
	
	for(int i = 0; i < num_ac; i++){
		for(int j = 0; j < prior->num_el-1; j++){
			out << gain[i][j] << " ";
		}
		out << gain[i][prior->num_el-1];
	}

	return out.str();
}

std::string Actions::toString(){
	std::ostringstream out;
	out << num_ac << " " << prior->num_el << "\n";
	
	for(int i = 0; i < num_ac; i++){
		for(int j = 0; j < prior->num_el-1; j++){
			out << gain[i][j] << " ";
		}
		out << gain[i][prior->num_el-1];
	}

	return out.str();
}

/*********** Channels ***********/

bool Channel::isChannel(std::vector<std::vector<long double> > &matrix){
	for(unsigned int i = 0; i < matrix.size(); i++){
		if(!Distribution::isDistribution(matrix[i])){
			return false;
		}
	}

	return true;
}

Channel::Channel(){
	num_out = 0;
	prior = NULL;
	matrix.resize(0, std::vector<long double>(0));
}

Channel::Channel(Distribution &_prior, std::string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	if(!fscanf(F, "%d,%d", &k, &num_out)){
		exit(EXIT_FAILURE);
	}

	if(k != _prior.num_el || num_out <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
	}

	prior = &_prior;

	matrix.resize(prior->num_el, std::vector<long double>(num_out));

	for(int i = 0; i < prior->num_el; i++){
		for(int j = 0; j < num_out; j++){
			if(!fscanf(F, "%Lf,", &(matrix[i][j]))){
				exit(EXIT_FAILURE);
			}
		}
	}
	fclose(F);

	if(!Channel::isChannel(matrix)){
		fprintf(stderr, "Error reading a channel. One of the rows is not a probability distribution!\n");
		exit(EXIT_FAILURE);
	}
}

Channel::Channel(Distribution &_prior, int _num_out, long double max_prob){

	prior = &_prior;
	num_out = _num_out;

	matrix.resize(prior->num_el, std::vector<long double>(num_out));

	for(int i = 0; i < prior->num_el; i++){
		int threshold = RAND_MAX;
		int x;

		for(int j = 0; j < num_out-1; j++){
			x = rand() % threshold;

			if((long double)x/RAND_MAX > max_prob){
				x = RAND_MAX * max_prob;
			}
			
			matrix[i][j] = (long double)x/RAND_MAX;
			threshold -= x;
		}

		matrix[i][num_out-1] = (long double)threshold/RAND_MAX;
	}
}

std::string Channel::toString(int precision){
	std::ostringstream out;
	out << std::setprecision(precision) << prior->num_el << " " << num_out << "\n";
	
	for(int i = 0; i < prior->num_el; i++){
		for(int j = 0; j < num_out-1; j++){
			out << matrix[i][j] << " ";
		}
		out << matrix[i][num_out-1];
	}

	return out.str();
}

std::string Channel::toString(){
	std::ostringstream out;
	out << prior->num_el << " " << num_out << "\n";
	
	for(int i = 0; i < prior->num_el; i++){
		for(int j = 0; j < num_out-1; j++){
			out << matrix[i][j] << " ";
		}
		out << matrix[i][num_out-1];
	}

	return out.str();
}

/************ Hyper *************/

void Hyper::buildHyper(Distribution &prior, Channel &channel, std::vector<std::vector<long double> > &joint, Distribution &outer, std::vector<std::vector<long double> > &inners){

	joint.resize(prior.num_el, std::vector<long double>(channel.num_out));

	outer.num_el = channel.num_out;
	outer.prob.resize(outer.num_el, 0);

	for(int i = 0; i < prior.num_el; i++){
		for(int j = 0; j < channel.num_out; j++){
			joint[i][j] = channel.matrix[i][j] * prior.prob[i];
			outer.prob[j] += joint[i][j];
		}
	}

	inners.resize(prior.num_el, std::vector<long double>(channel.num_out));

	for(int i = 0; i < prior.num_el; i++){
		for(int j = 0; j < channel.num_out; j++){
			if(outer.prob[j] == 0){
				inners[i][j] = 0;
			}else{
				inners[i][j] = joint[i][j]/outer.prob[j];
			}
		}
	}
}

Hyper::Hyper(){
	prior = NULL;
	channel = NULL;
	joint.resize(0, std::vector<long double>(0));
	inners.resize(0, std::vector<long double>(0));
}

Hyper::Hyper(std::string prior_file, std::string channel_file){
	Distribution prior(prior_file);
	Channel channel(prior, channel_file);

	Hyper::buildHyper(prior, channel, joint, outer, inners);
}

Hyper::Hyper(Distribution &prior, Channel &channel){
	Hyper::buildHyper(prior, channel, joint, outer, inners);	
}