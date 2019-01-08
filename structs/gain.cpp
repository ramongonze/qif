#include "gain.hpp"

Gain::Gain(){
	num_ac = 0;
	prior = NULL;
	gain.resize(0, std::vector<long double>(0));
}

Gain::Gain(Distribution &prior, std::string file){
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

	if(k != prior.num_el || num_ac <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
	}

	this->prior = &prior;
	gain.resize(num_ac, std::vector<long double>(this->prior->num_el));

	for(int i = 0; i < num_ac; i++){
		for(int j = 0; j < this->prior->num_el; j++){
			if(!fscanf(F, "%Lf,", &gain[i][j])){
				exit(EXIT_FAILURE);
			}
		}
	}

	fclose(F);
}

Gain::Gain(Distribution &prior, int num_ac, int MIN, int MAX){
	this->prior = &prior;
	this->num_ac = num_ac;

	gain.resize(this->num_ac, std::vector<long double>(this->prior->num_el));

	for(int i = 0; i < this->num_ac; i++){
		for(int j = 0; j < this->prior->num_el; j++){
			gain[i][j] = rand()%(MAX-MIN+1) + MIN;
		}
	}
}

std::string Gain::toString(int precision){
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision);
	
	out << num_ac << " " << prior->num_el << "\n";
	for(int i = 0; i < num_ac; i++){
		for(int j = 0; j < prior->num_el-1; j++){
			out << gain[i][j] << " ";
		}
		out << gain[i][prior->num_el-1] << "\n";
	}

	return out.str();
}

void Gain::printToFile(std::string file, int precision){
	std::ofstream F(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << num_ac << " " << prior->num_el << "\n";
	F << std::fixed << std::setprecision(precision);
	for(int i = 0; i < num_ac; i++){
		for(int j = 0; j < prior->num_el-1; j++){
			F << gain[i][j] << " ";
		}
		F << gain[i][prior->num_el-1] << "\n";
	}

	F.close();
}