//
// Created by rainoftime on 17-4-17.
//

#include <ctime>
#include <iostream>
#include <sys/time.h>

#include "timer.h"

calu_time::calu_time() {

}

void calu_time::timer_begin() {
	gettimeofday(&t_start_, NULL);
	//std::cout << "b: " << t_start_.tv_sec << std::endl;
}

void calu_time::timer_end() {
	gettimeofday(&t_end_, NULL);
	//std::cout << "e: " << t_end_.tv_sec << std::endl;
}

double calu_time::timer_span() {
	timersub(&t_end_, &t_start_, &t_result_);
	result_ = t_result_.tv_sec + (1.0 * t_result_.tv_usec) / 1000000;
	return result_;
}

calu_time::~calu_time() {

}
