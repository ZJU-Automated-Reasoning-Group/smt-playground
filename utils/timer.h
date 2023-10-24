#pragma once

#include <ctime>
#include <sys/time.h>
class calu_time {
private:
	struct timeval t_start_, t_end_, t_result_;
	double result_;
public:
	calu_time();
	~calu_time();
	void timer_begin();
	void timer_end();
	double timer_span();
};
