#ifndef Random_h
#define Random_h

#include <cstdio>
#include <random>
#include <cmath>

enum RandomType {
	UNIFORM,
	EXPONENTIAL,
	POISSON,
	LOGNORMAL,
	GAMMA,
	WEIBULL,
};

static std::default_random_engine eng(std::random_device{}());

class Random {
public:
	static double getRandom(RandomType type, double parameter1, double parameter2 = 0) {
		switch (type) {
			case UNIFORM:
				return uniform(parameter1);
				break;
			case EXPONENTIAL:
				return exponential(parameter1);
				break;
			case POISSON:
				return poisson(parameter1);
				break;
			case LOGNORMAL:
				return lognormal(parameter1, parameter2);
				break;
			case GAMMA:
				return gamma(parameter1, parameter2);
				break;
			case WEIBULL:
				return weibull(parameter1, parameter2);
				break;
			default:
				return 0;
				break;
		}
	}

	static double uniform(double max = 1) {
		std::uniform_real_distribution<double> u(0, max);
		return u(eng);
	}

	static double exponential(double lambda) {
		std::exponential_distribution<double> e(lambda);
		return e(eng);
	}

	static double poisson(double lambda) {
		std::poisson_distribution<int> p(lambda);
		return p(eng);
	}

	static double lognormal(double m, double s) {
		std::lognormal_distribution<double> l(m, s);
		return l(eng);
	}

	static double gamma(double alpha, double beta) {
		std::gamma_distribution<double> g(alpha, beta);
		return g(eng);
	}

	static double weibull(double a, double b) {
		std::weibull_distribution<double> w(a, b);
		return w(eng);
	}
};

#endif