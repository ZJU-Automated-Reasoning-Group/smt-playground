
#pragma once

#include <string.h>
#include <z3++.h>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "utils/z3plus.h"

using namespace std;
using namespace z3;

struct region_sampler {
	std::string path;
	std::string input_file;
	std::vector<std::string> input_files;
	//  bool filemode;

	struct timespec start_time;
	double solver_time = 0.0;
	double check_time = 0.0;
	int max_samples;
	double max_time;

	int m_samples = 0;
	int m_success = 0;
	int m_unique = 0;
	double m_sample_time = 0.0;

	z3::context c;
	z3::expr smt_formula;
	z3::expr_vector m_vars;

	// std::vector<z3::expr> smt_formulas;
	std::vector<int> lower_bounds;
	std::vector<int> upper_bounds;
	std::vector<bool> should_fix;

	std::vector<std::vector<int> > unique_models;

	region_sampler(std::string input, int max_samples, double max_time) :
			path(input), max_samples(max_samples), max_time(max_time), smt_formula(
					c), m_vars(c) {
		struct stat info;
		if (stat(path.c_str(), &info) != 0) {
			if (info.st_mode & S_IFDIR) {
				DIR *dirp = opendir(input.c_str());
				struct dirent *dp;
				while ((dp = readdir(dirp)) != NULL) {
					std::string tmp(dp->d_name);
					input_files.push_back(tmp);
				}
			} else {
				input_files.push_back(input);
			}
		}
		input_file = input;
	}

	void parse_smt() {
		expr_vector evec = c.parse_file(input_file.c_str());
		smt_formula = mk_and(evec);
	}

	void get_bounds() {
		params p(c);
		p.set("priority", c.str_symbol("box"));
		p.set("timeout", (unsigned) 15000); // 15 seconds
		// 'core_maxsat', 'wmax', 'maxres', 'pd-maxres' ?
		//p.set("maxsat_engine", c.str_symbol("maxres"));

		optimize opt_sol_min(c);
		opt_sol_min.set(p);
		opt_sol_min.add(smt_formula);

		optimize opt_sol_max(c);
		opt_sol_max.set(p);
		opt_sol_max.add(smt_formula);

		// Find min
		std::vector<optimize::handle> handlers_min;
		for (unsigned i = 0; i < m_vars.size(); i++) {
			handlers_min.push_back(opt_sol_min.minimize(m_vars[i]));
		}
		auto min_res = opt_sol_min.check();
		for (unsigned i = 0; i < m_vars.size(); i++) {
			//std::cout << m_vars[i] <<": " << opt_sol_min.upper(handlers_min[i]) << "\n";
			if (min_res == sat) {
				lower_bounds.push_back(
						opt_sol_min.upper(handlers_min[i]).get_numeral_int());
			} else {
				lower_bounds.push_back(0);
			}
		}

		// Find max
		std::vector<optimize::handle> handlers_max;
		for (unsigned i = 0; i < m_vars.size(); i++) {
			handlers_max.push_back(opt_sol_max.maximize(m_vars[i]));
		}
		auto max_res = opt_sol_max.check();
		for (unsigned i = 0; i < m_vars.size(); i++) {
			//std::cout << m_vars[i] <<": " << opt_sol_max.lower(handlers_max[i]) << "\n";
			if (max_res == sat) {
				upper_bounds.push_back(
						opt_sol_max.lower(handlers_max[i]).get_numeral_int());
			} else {
				unsigned sz = m_vars[i].get_sort().bv_size();
				//max integer number of size sz
				upper_bounds.push_back((1 << sz) - 1);
			}
		}
	}

	std::vector<int> sample_once() {
		m_samples++;
		std::vector<int> sample;
		for (unsigned i = 0; i < m_vars.size(); i++) {
			if (should_fix[i]) {
				sample.push_back(lower_bounds[i]);
			} else {
				int output = lower_bounds[i]
						+ (rand()
								% static_cast<int>(upper_bounds[i]
										- lower_bounds[i] + 1));
				sample.push_back(output);
			}
		}
		return sample;
	}

	bool check_random_model(std::vector<int> &assignments) {
		model rand_model(c);
		for (unsigned i = 0; i < m_vars.size(); i++) {
			z3::func_decl decl = m_vars[i].decl();
			z3::expr val_i = c.bv_val(assignments[i],
					m_vars[i].get_sort().bv_size());
			rand_model.add_const_interp(decl, val_i);
		}

		if (rand_model.eval(smt_formula).is_true()) {
			if (unique_models.size() == 0) {
				unique_models.push_back(assignments);
				return true;
			}
			bool is_unique = true;
			for (unsigned i = 0; i < unique_models.size(); i++) {
				std::vector<int> model = unique_models[i];
				bool same_model = true;
				for (unsigned j = 0; j < m_vars.size(); j++) {
					if (model[j] != assignments[j]) {
						same_model = false;
						break;
					}
				}
				if (same_model) {
					is_unique = false;
					break;
				}
			}
			if (is_unique) {
				m_unique++;
				unique_models.push_back(assignments);
			}
			return true;
		} else {
			return false;
		}
	}

	void run() {
//        clock_gettime(CLOCK_REALTIME, &start_time);
//        srand(start_time.tv_sec);
		// parse_cnf();
		for (auto file : input_files) {
			input_file = file;
			parse_smt();
			std::cout << "parse finished... \n";

			get_expr_vars(smt_formula, m_vars);
			std::cout << "get vars finished... start counting\n";

			auto init = std::chrono::high_resolution_clock::now();
//        struct timespec start;
//        clock_gettime(CLOCK_REALTIME, &start);
			get_bounds();
			for (unsigned i = 0; i < m_vars.size(); i++) {
				if (lower_bounds[i] == upper_bounds[i]) {
					should_fix.push_back(true);
				} else {
					should_fix.push_back(false);
				}
			}
			std::cout << "get bounds finished...\n";

			auto finish = std::chrono::high_resolution_clock::now();
//        struct timespec end;
//
//
//        clock_gettime(CLOCK_REALTIME, &end);
			solver_time += std::chrono::duration<double, std::milli>(
					finish - init).count();

			init = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < max_samples; i++) {
				if (i % 5000 == 0)
					print_stats();
//            struct timespec samp;
//            clock_gettime(CLOCK_REALTIME, &samp);
				auto iter_start = std::chrono::high_resolution_clock::now();
				double elapsed = std::chrono::duration<double, std::milli>(
						iter_start - init).count();
				if (elapsed >= max_time) {
					std::cout << "Stopping: timeout\n";
					this->finish();
				}

				std::vector<int> sample = sample_once();
				if (check_random_model(sample)) {
					m_success++;
				}
				finish = std::chrono::high_resolution_clock::now();
				m_sample_time += std::chrono::duration<double, std::milli>(
						finish - init).count();
			}
		}

	}

	double duration(struct timespec *a, struct timespec *b) {
		return (b->tv_sec - a->tv_sec) + 1.0e-9 * (b->tv_nsec - a->tv_nsec);
	}

	void finish() {
		std::cout << "-------Good Bye!-----\n";
		print_stats();
		// TODO: write results to some file
		exit(0);
	}

	void reset_state() {
		solver_time = 0;
		m_sample_time = 0;
		m_samples = 0;
		m_success = 0;
		unique_models.clear();
	}

	void print_stats() {

		std::cout << "solver time: " << solver_time << "\n";
		std::cout << "sample total time: " << m_sample_time << "\n";
		std::cout << "samples number: " << m_samples << "\n";
		std::cout << "samples success: " << m_success << "\n";
		std::cout << "unique modesl: " << unique_models.size() << "\n";
		std::cout << "------------------------------------------\n";

		std::ofstream of("res.log", std::ofstream::app);
		of << "solver time: " << solver_time << "\n";
		of << "sample total time: " << m_sample_time << "\n";
		of << "samples number: " << m_samples << "\n";
		of << "samples success: " << m_success << "\n";
		of << "unique modesl: " << unique_models.size() << "\n";
		of << "------------------------------------------\n";

		of.close();
	}
};

