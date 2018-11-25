#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace std;

// https://en.wikipedia.org/wiki/Kahan_summation_algorithm

//////////////////////////////////////////////////////////////
float mirand(float hasta){
	return (hasta * (rand() / (RAND_MAX + 1.0)));
}
//////////////////////////////////////////////////////////////

bool fun(float i,float j)
{
   return fabs(i)<fabs(j);
}

int nums = 10000000;

int main(){
	srand(314);
	vector<float> numeros;
	for(int i=0;i<nums;i++){
		numeros.push_back(pow(2,-rand()%10));
		// numeros.push_back(-pow(2,-rand()%10));
		// ~ numeros.push_back(pow(2,-20));
	}
	//~ numeros[50] = 16.;
	 
	float suma = 0.0;
	float kahan = 0.0;
	float c = 0.0;
	for(int i=0;i<nums;i++){
		//~ printf("%.10lf\n", numeros[i]);
		suma += numeros[i];

		float y = numeros[i]-c; // la parte grande de numeros[i]
		float t = kahan + y; // lo que tenia + la parte grande
		// c = t - (kahan + y);
		c = (t - kahan) - y; // el nuevo kahan menos el viejo (t - kahan) deberia ser cercano a y, menos la parte gande de 
							// numeros[i]
		kahan = t;
	}
	
	
	
	
	
	//~ sort(numeros, numeros+nums);
	sort(numeros.begin(),numeros.end());
	float suma2 = 0.0;
	float suma3 = 0.0;
	for(int i=0;i<nums;i++){
		suma2+=numeros[i];
		suma3+=numeros[nums-1-i];
	}

	sort(numeros.begin(), numeros.end(), fun);
	float suma4 = 0.0;
	for (auto x:numeros){
		suma4 += x;
	}
	
	printf("Como viene: %.15lf\nOrden Ascendente: %.15lf\nOrden Descendente: %.15lf\nSUPER KAHAN!: %.15lf\n", suma, suma2, suma3, kahan);
	printf("Orden ascendente de val abs: %.15lf\n", suma4);
	return 0;
}
