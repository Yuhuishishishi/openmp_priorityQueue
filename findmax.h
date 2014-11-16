#include <queue>

struct Interval {
	double a;
	double b;
	double fa;
	double fb;
	double possible_max;
};

struct boundcomp {
	bool operator() (Interval itv1, Interval itv2) {
		return itv1.possible_max > itv2.possible_max;
	}
};

void findmax(double(*func)(double), double& max_val, double a, double b, double eps, double s);
Interval get_one_job(std::priority_queue<Interval, std::vector<Interval>, boundcomp> intv_queue);
void add_one_job(std::priority_queue<Interval, std::vector<Interval>, boundcomp> intv_queue, Interval job);
void status_off(int tid, int& status);
void status_on(int tid, int& status);
int query_status(int tid, int status);