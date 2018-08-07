#include <iostream>
#include <time.h>
#include "structs.hpp"
#include "functions.hpp"

using namespace std;

int main(){

	srand(8);
	Distribution D("prior");

	Channels C = Channels(D,"channel");

	for(int i = 0; i < C.prior->n; i++){
		for(int j = 0; j < C.y; j++){
			printf("%.2Lf ", C.H[i][j]);
		}printf("\n");
	}

	return 0;
}