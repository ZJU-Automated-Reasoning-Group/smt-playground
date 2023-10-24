#include "timer.h"

void timer::suspend() {
	if (!suspended) {
		time(&suspend_start_time);
		suspended = true;
	}
}

void timer::resume() {
	if (suspended) {
		time_t curr_time;
		time(&curr_time);
		double time_elapsed = difftime(curr_time, suspend_start_time);
		suspend_time += time_elapsed;
		suspended = false;
	}
}

bool timer::is_timeout() {
	if (steps_counter == 0) {
		steps_counter = steps;

		double time_elapsed;

		if (suspended) {
			time_elapsed = difftime(suspend_start_time, start_time);
		} else {
			time_t curr_time;
			time(&curr_time);
			time_elapsed = difftime(curr_time, start_time);
		}

		if (time_elapsed > (duration + suspend_time)) {
			return true;
		}
	} else {
		steps_counter--;
	}

	return false;
}

void timer::check() {
	if (is_timeout()) {
		task_after_timeout();
	}
}
