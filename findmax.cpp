#include <cstdio>
#include <queue>
#include <vector>
#include "findmax.h"
#include <algorithm>
#include <omp.h>

void findmax(double(*func)(double), double& max_val, double a, double b, double eps, double s) {
	// job queue
	std::priority_queue<Interval, std::vector<Interval>, boundcomp> intv_queue;
	// add the initial interval
	double fa = func(a), fb = func(b);
	Interval intv{ a, b, fa, fb, (fa + fb + s*(b - a) / 2) };
	intv_queue.push(intv);
	int num_thread;
	int tid;
	int status = 0;
	double current_max_val = std::max(fa, fb);
	double mid;
	double fmid;
	Interval job_to_do;


#pragma omp parallel shared(num_thread, current_max_val, status) private(mid, fmid, job_to_do, tid)
	{
		// initialize the global variables
#pragma omp single
		{
			num_thread = omp_get_num_threads();
		}
		
		tid = omp_get_thread_num();
		
		do {
			// update the queue
			#pragma omp flush(intv_queue,status)
			if (intv_queue.size() > 0) {
				status_on(tid, status);
				#pragma omp flush(status)
				// get one job
				job_to_do = get_one_job(intv_queue);
				// do the job
				if (job_to_do.possible_max > current_max_val + eps) {
					mid = (job_to_do.a + job_to_do.b) / 2;
					fmid = func(mid);
					// split into two subintervals
					Interval intv1{ job_to_do.a, mid,
						job_to_do.fa, fmid, 0.5*(job_to_do.fa + fmid + s*(mid - job_to_do.a)) };
					Interval intv2{ mid, job_to_do.b,
						fmid, job_to_do.fb, 0.5*(fmid + job_to_do.fb + s*(job_to_do.b - mid)) };
					// insert the jobs
					add_one_job(intv_queue, intv1);
					add_one_job(intv_queue, intv2);
					// update current max_val
#pragma omp critical 
					{
					current_max_val = std::max(current_max_val, fmid);
					printf("Current max val = %.3f, status = %d\n", current_max_val, status);
					}
				}
				else {
					// do nothing
				}
			}
			else {
				status_off(tid, status);
				#pragma omp flush(status)
			}
#pragma omp flush(intv_queue,status)

		} while (status > 0);
	}

	max_val = current_max_val;
}

// wrapper function to get a job
Interval get_one_job(std::priority_queue<Interval, std::vector<Interval>, boundcomp> intv_queue) {
	Interval job;
	#pragma omp critical
	{
		job = intv_queue.top();
		intv_queue.pop();
	}
	return job;
}

// wrappter function to add a job
void add_one_job(std::priority_queue<Interval, std::vector<Interval>, boundcomp> intv_queue,
	Interval job) {
	#pragma omp critical
	{
		intv_queue.push(job);
	}
}

void status_off(int tid, int& status) {
	// query the status
	int tstatus = query_status(tid, status);
	if (tstatus > 0) {
#pragma omp atomic
		status -= pow(10, tid);
	}
	else {
		// do nothing
	}
}

void status_on(int tid, int& status) {
	// query the status
	int tstatus = query_status(tid, status);
	if (tstatus > 0) {
		// do nothing
	}
	else {
#pragma omp atomic
		status += pow(10, tid);
	}
}

int query_status(int tid, int status) {
	int tstatus;
	int x = status / pow(10, tid);
	int y;
	if (x == 0)
		return 0;
	else 
		y = status / (10 * x);
	tstatus = x - y * 10;
	return tstatus;
}