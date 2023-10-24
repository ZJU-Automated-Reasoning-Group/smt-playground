#include <string>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include "popen.h"

const int READ = 0;
const int WRITE = 1;

pid_t Popen::popen2(int *infp, int *outfp, const char *file,
		char *const args[]) {
	int p_stdin[2], p_stdout[2];
	pid_t pid;
	if (pipe(p_stdin) != 0 || pipe(p_stdout) != 0)
		return -1;
	pid = fork();
	if (pid < 0)
		return pid;
	else if (pid == 0) {
		close(p_stdin[WRITE]);
		dup2(p_stdin[READ], READ);
		close(p_stdout[READ]);
		dup2(p_stdout[WRITE], WRITE);
		execvp(file, args);
		perror("execv");
		exit(1);
	}
	if (infp == NULL)
		close(p_stdin[WRITE]);
	else
		*infp = p_stdin[WRITE];
	if (outfp == NULL)
		close(p_stdout[READ]);
	else
		*outfp = p_stdout[READ];
	return pid;
}

std::string Popen::getOutput() {
	std::ostringstream out;
	out << *this;
	auto result = out.str();
	while (!isEnd(result)) {
		out = std::ostringstream();
		out << *this;
		result += out.str();
	}
	result = result.substr(0, result.size() - 1);
	if (result.substr(0, 6) == "(error") {
		throw result + ";debug info\n" + debugInfo.str();
	}
	return result;
}

bool Popen::isEnd(std::string str) const {
	auto size = str.size();
	if (size < 2) {
		return false;
	}
	if (str.substr(size - 2) == "\n\n") {
		return true;
	}
	return false;
}
