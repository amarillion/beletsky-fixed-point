#include <stdio.h>
#include <stdarg.h>
#include <math.h>

const double PI2 = M_PIl * 2;

// questions
// 1. inverse_b_func is never used, is that ok?
// 2. dt is never used, I assume it should be t += dt instead of t += 1.0?
// 3. in s_func, e is never used. is that ok?

// intermediate calculation
double s_func(double e, double t) {
	return (1 - cos(t));
}

double second_derivative_x(double x, double d_x, double e, double n, double t) {
	double s = s_func(e, x);
	double sin_t = sin(t);
	double sin_x = sin(x);
	return 2*s*sin_t*d_x + s*n*n*sin_x + 4*s*e*sin_t;
}

double first_derivative_x(double x, double dd_x, double e, double n, double t) {
	double s = s_func(e, t);
	double b = (-2 * e * sin(x));
	double sin_t = sin(t);
	return 4*b*e*sin_t - b*s*dd_x + b*n*n*sin_t;
}

double mod2pi(double x) {
	return fmod(x, PI2);
}

// Euler integration
void euler() {
	double t = 0;
	double x = 0.324;
	double d_x = 0.00932;
	double dd_x = 0.0000000932;
	double e = 0.1; // eccentricity, between 0 and 1
	double n = 0.3; // degree of non-symmetry, 0 is symmetric
	double dt = 1;
	
	for (int i = 0; i < 100; ++i) {
		double old_x = x;
		double old_d_x = d_x;

		double new_dd_x = second_derivative_x(old_x, old_d_x, e, n, t);
		double new_d_x = old_d_x + first_derivative_x(old_x, new_dd_x, e, n, t);
		double new_x = old_x + new_d_x;

		x = new_x;
		d_x = new_d_x;
		t += dt;
		printf("%i %.5f %.5f %.5f %.5f\n", i, mod2pi(t), mod2pi(x), d_x, new_dd_x);
	}
}

int main(int argc, char **argv) {
	printf("Program starts\n");
	euler();
	return 0;
}
