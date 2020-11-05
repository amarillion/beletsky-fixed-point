#include <allegro5/allegro.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

/******************************************
 *  Variant 1: Using double floating point numbers  
 ******************************************/
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

/******************************************
 *  Variant 2: Using allegro fixed point math  
 ******************************************/

// intermediate calculation
al_fixed s_func2(al_fixed t) {
	return (al_fixsub(al_itofix(1), al_fixcos(t)));
}

al_fixed second_derivative_x2(al_fixed x, al_fixed d_x, al_fixed e, al_fixed n, al_fixed t) {
	al_fixed s = s_func2(x);
	al_fixed sin_t = al_fixsin(t);
	al_fixed sin_x = al_fixsin(x);

	// 2*s*sin_t*d_x
	al_fixed term1 = al_fixmul(
		al_fixmul(al_itofix(2), s), 
		al_fixmul(sin_t, d_x)
	);
	//	s*n*n*sin_x
	al_fixed term2 = al_fixmul(
		al_fixmul(n, n),
		al_fixmul(s, sin_x)
	);
	// 4*s*e*sin_t
	al_fixed term3 = al_fixmul(
		al_fixmul(al_itofix(4), s),
		al_fixmul(e, sin_t)
	);
	return al_fixadd(term1, al_fixadd(term2, term3));
}

al_fixed first_derivative_x2(al_fixed x, al_fixed dd_x, al_fixed e, al_fixed n, al_fixed t) {
	al_fixed s = s_func2(t);
	al_fixed b = al_fixmul(
		al_fixmul(al_itofix(-2), e), 
		al_fixsin(x)
	);
	al_fixed sin_t = al_fixsin(t);

	// 4*b*e*sin_t
	al_fixed term1 = al_fixmul(
		al_fixmul(al_itofix(4), b),
		al_fixmul(e, sin_t)
	);
	// -1 * b*s*dd_x + 
	al_fixed term2 = al_fixmul(
		al_fixmul(al_itofix(-1), b),
		al_fixmul(s, dd_x)
	);
	// b*n*n*sin_t;
	al_fixed term3 = al_fixmul(
		al_fixmul(b, sin_t),
		al_fixmul(n, n)
	);

	return al_fixadd(term1, al_fixadd(term2, term3));
}

al_fixed mod2pi(al_fixed x) {
	return x & 0xFFFFFF;
}

// Euler integration
void euler2() {
	al_fixed t = 0;
	al_fixed x = al_ftofix(0.324);
	al_fixed d_x = al_ftofix(0.00932);
	al_fixed dd_x = al_ftofix(0.0000000932);
	al_fixed e = al_ftofix(0.1); // eccentricity, between 0 and 1
	al_fixed n = al_ftofix(0.3); // degree of non-symmetry, 0 is symmetric
	al_fixed dt = al_itofix(1);
	
	printf ("ddx: %e", al_fixtof(dd_x));

	for (int i = 0; i < 100; ++i) {
		al_fixed old_x = x;
		al_fixed old_d_x = d_x;

		al_fixed new_dd_x = second_derivative_x2(old_x, old_d_x, e, n, t);
		al_fixed new_d_x = old_d_x + first_derivative_x2(old_x, new_dd_x, e, n, t);
		al_fixed new_x = old_x + new_d_x;

		x = new_x;
		d_x = new_d_x;
		t = al_fixadd(t, dt);
		printf("%i %.5f %.5f %.5f %.5f\n", i, 
			al_fixtof(mod2pi(t)), 
			al_fixtof(mod2pi(x)), 
			al_fixtof(d_x), 
			al_fixtof(new_dd_x)
		);
	}
}

int main(int argc, char **argv) {
	printf("Program starts\n");
	euler2();
	return 0;
}
