#include "../include/channel.hpp"

void Channel::buildReducedMatrix(){
    int n = matrix.size();

    reducedMatrix.resize(prior->num_el, std::vector<long double>(num_out));

	for(int i = 0; i < prior->num_el; i++){
		for(int j = 0; j < num_out; j++){
			reducedMatrix[i][j] = matrix[i][j];
		}
	}

    for (int i=0; i<n; i++) {
        // Search for maximum in this column
        double maxEl = abs(reducedMatrix[i][i]);
        int maxRow = i;
        for (int k=i+1; k<n; k++) {
            if (abs(reducedMatrix[k][i]) > maxEl) {
                maxEl = abs(reducedMatrix[k][i]);
                maxRow = k;
            }
        }

        // Swap maximum row with current row (column by column)
        for (int k=i; k<n+1;k++) {
            double tmp = reducedMatrix[maxRow][k];
            reducedMatrix[maxRow][k] = reducedMatrix[i][k];
            reducedMatrix[i][k] = tmp;
        }

        // Make all rows below this one 0 in current column
        for (int k=i+1; k<n; k++) {
            double c = -reducedMatrix[k][i]/reducedMatrix[i][i];
            for (int j=i; j<n+1; j++) {
                if (i==j) {
                    reducedMatrix[k][j] = 0;
                } else {
                    reducedMatrix[k][j] += c * reducedMatrix[i][j];
                }
            }
        }
    }

    printf("reducedMatrix\n");
    for(unsigned int i = 0; i < reducedMatrix.size(); i++){
		for(unsigned int j = 0; j < reducedMatrix[0].size(); j++){
			printf("%.2Lf ", reducedMatrix[i][j]);
		}
		printf("\n");
	}


    // // Solve equation Ax=b for an upper triangular matrix A
    // vector<double> x(n);
    // for (int i=n-1; i>=0; i--) {
    //     x[i] = A[i][n]/A[i][i];
    //     for (int k=i-1;k>=0; k--) {
    //         A[k][n] -= A[k][i] * x[i];
    //     }
    // }
    // return x;
}


Channel::Channel(){
	num_out = 0;
	prior = NULL;
	matrix.resize(0, std::vector<long double>(0));
	reducedMatrix.resize(0, std::vector<long double>(0));
}

Channel::Channel(Distribution &prior, std::string file){
	int k;
	FILE *F;

	F = fopen(file.c_str(), "r");

	if(F == NULL){
		fprintf(stderr, "Error reading file!\n");
		exit(EXIT_FAILURE);
	}

	if(!fscanf(F, "%d %d", &k, &num_out)){
		exit(EXIT_FAILURE);
	}

	if(k != prior.num_el || num_out <= 0){
		fprintf(stderr, "Invalid matrix size!\n");
		exit(EXIT_FAILURE);
	}

	this->prior = &prior;

	matrix.resize(this->prior->num_el, std::vector<long double>(num_out));

	for(int i = 0; i < this->prior->num_el; i++){
		for(int j = 0; j < num_out; j++){
			if(!fscanf(F, "%Lf", &(matrix[i][j]))){
				exit(EXIT_FAILURE);
			}
		}
	}
	fclose(F);

	if(!Channel::isChannel(matrix)){
		fprintf(stderr, "Error reading a channel. One of the rows is not a probability distribution!\n");
		exit(EXIT_FAILURE);
	}

	/* Build the reduced matrix */
	Channel::buildReducedMatrix();
}

Channel::Channel(Distribution &prior, std::vector<std::vector<long double> > &matrix){
	this->prior = &prior;
	this->num_out = matrix[0].size();

	if((unsigned int)prior.num_el != matrix.size()){
		fprintf(stderr, "The number of secrets in the prior distribution is different of the number of rows in the channel matrix!\n");
		exit(EXIT_FAILURE);
	}

	this->matrix.resize(this->prior->num_el, std::vector<long double>(this->num_out));

	for(int i = 0; i < prior.num_el; i++){
		for(int j = 0; j < this->num_out; j++){
			this->matrix[i][j] = matrix[i][j];
		}
	}

	if(!isChannel(this->matrix)){
		fprintf(stderr, "The channel matrix is not valid!\n");
		exit(EXIT_FAILURE);
	}

	/* Build the reduced matrix */
	Channel::buildReducedMatrix();
}

Channel::Channel(Distribution &prior, int num_out){

	this->prior = &prior;
	this->num_out = num_out;

	matrix.resize(this->prior->num_el, std::vector<long double>(this->num_out));

	for(int i = 0; i < this->prior->num_el; i++){
		int threshold = RAND_MAX;
		int x;

		for(int j = 0; j < this->num_out-1; j++){
			x = rand() % threshold;

			matrix[i][j] = (long double)x/RAND_MAX;
			threshold -= x;
		}

		matrix[i][this->num_out-1] = (long double)threshold/RAND_MAX;
	}

	/* Build the reduced matrix */
	Channel::buildReducedMatrix();
}

bool Channel::isChannel(std::vector<std::vector<long double> > &matrix){
	for(unsigned int i = 0; i < matrix.size(); i++){
		if(!Distribution::isDistribution(matrix[i])){
			return false;
		}
	}

	return true;
}

std::string Channel::toString(int precision){
	std::ostringstream out;
	out << std::fixed << std::setprecision(precision);
	
	for(int i = 0; i < prior->num_el; i++){
		for(int j = 0; j < num_out-1; j++){
			out << matrix[i][j] << " ";
		}
		out << matrix[i][num_out-1] << "\n";
	}

	return out.str();
}

void Channel::printToFile(std::string file, int precision){
	std::ofstream F(file);

	if(F.is_open() == false){
		fprintf(stderr, "Error opening the file ""%s""!\n", file.c_str());
		exit(EXIT_FAILURE);
	}

	F << prior->num_el << " " << num_out << "\n";
	F << std::fixed << std::setprecision(precision);
	for(int i = 0; i < prior->num_el; i++){
		for(int j = 0; j < num_out-1; j++){
			F << matrix[i][j] << " ";
		}
		F << matrix[i][num_out-1] << "\n";
	}

	F.close();
}