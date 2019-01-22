#include "../include/hyper.hpp"

void Hyper::buildHyper(Distribution &prior, Channel &channel){

	joint.resize(prior.num_el, std::vector<long double>(channel.num_out));

	outer.num_el = channel.num_out;
	outer.prob.resize(outer.num_el);

	for(int i = 0; i < outer.num_el; i++)
		outer.prob[i] = 0.0f;

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
	prior = new Distribution(prior_file);
	channel = new Channel(*prior, channel_file);

	buildHyper(*prior, *channel);
}

Hyper::Hyper(Distribution &prior, Channel &channel){
	this->prior = &prior;
	this->channel = &channel;

	buildHyper(prior, channel);
}

std::string Hyper::toString(std::string type, int precision){
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision);
	
	if(type == "joint"){
		for(int i = 0; i < prior->num_el; i++){
			for(int j = 0; j < channel->num_out-1; j++){
				out << joint[i][j] << " ";
			}
			out << joint[i][channel->num_out-1] << "\n";
		}
	}else if(type == "outer"){
		for(int i = 0; i < channel->num_out-1; i++){
			out << outer.prob[i] << " ";
		}

		out << outer.prob[channel->num_out-1] << "\n";
	}else if(type == "inners"){
		for(int i = 0; i < prior->num_el; i++){
			for(int j = 0; j < channel->num_out-1; j++){
				out << inners[i][j] << " ";
			}
			out << inners[i][channel->num_out-1] << "\n";
		}
	}else{
		fprintf(stderr, "Invalid parameter type! It must be ""joint"", ""outer"" or ""inners""\n");
		exit(EXIT_FAILURE);
	}

	return out.str();
}

void Hyper::printToFile(std::string type, std::string file, int precision){
	std::ofstream F(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << std::fixed << std::setprecision(precision);

	if(type == "joint"){
		F << prior->num_el << " " << channel->num_out << "\n";
			
		for(int i = 0; i < prior->num_el; i++){
			for(int j = 0; j < channel->num_out-1; j++){
				F << joint[i][j] << " ";
			}
			F << joint[i][channel->num_out-1] << "\n";
		}
	}else if(type == "outer"){
		F << channel->num_out << "\n";
			
		for(int i = 0; i < channel->num_out-1; i++){
			F << outer.prob[i] << " ";
		}

		F << outer.prob[channel->num_out-1] << "\n";
	}else if(type == "inners"){
		F << prior->num_el << " " << channel->num_out << "\n";
			
		for(int i = 0; i < prior->num_el; i++){
			for(int j = 0; j < channel->num_out-1; j++){
				F << inners[i][j] << " ";
			}
			F << inners[i][channel->num_out-1] << "\n";
		}
	}else{
		fprintf(stderr, "Invalid parameter type! It must be ""joint"", ""outer"" or ""inners""\n");
		exit(EXIT_FAILURE);
	}

	F.close();
}