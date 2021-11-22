#include "timer.h"

static void clock_gettime(timespec &spec)
{
	time_t clock_time = clock();
	spec.tv_sec = clock_time / 1000;
	spec.tv_nsec = clock_time % 1000 * 1000000;
}

cTimer::cTimer() {
	clock_gettime(process_start);
	frame_start = process_start;
}

cTimer::~cTimer() {
	
}

double cTimer::elapsed(bool frame) {
	clock_gettime(current);
	double elapsed = frame ? (current.tv_sec + current.tv_nsec / 1000000000.0 -   frame_start.tv_sec -   frame_start.tv_nsec / 1000000000.0) :
				 (current.tv_sec + current.tv_nsec / 1000000000.0 - process_start.tv_sec - process_start.tv_nsec / 1000000000.0);
	frame_start = current;
	return elapsed;
}