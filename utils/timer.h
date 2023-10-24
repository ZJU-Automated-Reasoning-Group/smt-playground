#pragma once

#include <functional>
#include <time.h>

class timer {
private:
	double duration;
	size_t steps, steps_counter;
	double suspend_time;
	bool suspended;
	time_t start_time;
	time_t suspend_start_time;

	std::function<void()> task_after_timeout;

public:
	/// The constructor should set the duration \p D of the timer
	/// and the task \p T that should take after timeout happens.
	/// The last parameter \S is an optimization which controls
	/// the frequency you check timeout. For example, if \p S is
	/// set to 5, you will really check timeout once you call
	/// timer::check() five times.
	template<typename Task>
	timer(double D, Task T, unsigned S = 0) :
			duration(D), steps(S), steps_counter(S), suspend_time(0.0), suspended(
					false) {
		time(&start_time);
		time(&suspend_start_time);
		task_after_timeout = [T]() {
			T();
		};
	}

	// Suspend/resume timer, the time consumption between suspend() and resume()
	// shall be ignored
	void suspend();
	void resume();

	// return true if time out exists
	bool is_timeout();

	// Check timeout and perform the corresponding actions
	void check();
};
