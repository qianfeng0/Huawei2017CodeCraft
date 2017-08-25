#include <time.h>

static time_t time_start = 0;
static time_t time_end = 0;


void timeStart() {
	time(&time_start);
}

time_t elapsedTime() {
	time(&time_end);
	return time_end - time_start;
}