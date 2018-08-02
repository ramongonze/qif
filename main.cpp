#include <iostream>
#include <time.h>

#ifndef _structs
#include "structs.hpp"
#endif

using namespace std;

int main(){

	srand(time(NULL));
	int size = 30;
	Distribution D("dist");

	for(int i = 0; i < D.n; i++){
		cout << D.probability[i] << endl;
	}
	
	cout << "Shannon Entropy: " << shannonEntropy(D) << endl;


/*
	while(1){

		cout << "V(D): " << bayesVulnerability(D) << endl;

		long double tot_sum = 0;
		for(int i = 0; i < size; i++)
			tot_sum += D.probability[i];

		if(1-EPS > tot_sum || tot_sum > 1+EPS){
			printf("It is not a probability distribution!!\n");
		}
	}
*/
	

	return 0;
}