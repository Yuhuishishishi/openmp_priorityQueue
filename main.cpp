#include <cstdio>
#include <cstdlib>
#include "findmax.h"
#include <cmath>

double f(double); // function to evaluate

int main(int argc, char* argv[]) {
	double a = 1, b = 100, eps = 1e-6, s = 12;
	double max_val;
	findmax(f, max_val, a, b, eps, s);
	printf("max val = %.3f\n", max_val);

	return 0;
}

double f(double x) {
	double f_val = 0, inner_sum;
	for (int i = 100; i > 0; i--) {
		inner_sum = 0;
		for (int j = i; j > 0; j--) {
			inner_sum += pow(x + j, -3.1);
		}
		f_val += sin(x + inner_sum) / pow(1.2, i);
	}
	return f_val;
}